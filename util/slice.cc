//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2012 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "rocksdb/slice.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <limits>

#include "rocksdb/convenience.h"
#include "rocksdb/slice_transform.h"
#include "rocksdb/utilities/object_registry.h"
#include "rocksdb/utilities/options_type.h"
#include "util/string_util.h"

namespace ROCKSDB_NAMESPACE {

namespace {

class FixedPrefixTransform : public SliceTransform {
 private:
  size_t prefix_len_;
  std::string id_;

 public:
  explicit FixedPrefixTransform(size_t prefix_len) : prefix_len_(prefix_len) {
    id_ = std::string(kClassName()) + "." + std::to_string(prefix_len_);
  }

  static const char* kClassName() { return "rocksdb.FixedPrefix"; }
  static const char* kNickName() { return "fixed"; }
  const char* Name() const override { return kClassName(); }
  const char* NickName() const override { return kNickName(); }

  bool IsInstanceOf(const std::string& name) const override {
    if (name == id_) {
      return true;
    } else if (StartsWith(name, kNickName())) {
      std::string alt_id =
          std::string(kNickName()) + ":" + std::to_string(prefix_len_);
      if (name == alt_id) {
        return true;
      }
    }
    return SliceTransform::IsInstanceOf(name);
  }

  std::string GetId() const override { return id_; }

  Slice Transform(const Slice& src) const override {
    assert(InDomain(src));
    return Slice(src.data(), prefix_len_);
  }

  bool InDomain(const Slice& src) const override {
    return (src.size() >= prefix_len_);
  }

  bool FullLengthEnabled(size_t* len) const override {
    *len = prefix_len_;
    return true;
  }

  bool SameResultWhenAppended(const Slice& prefix) const override {
    return InDomain(prefix);
  }
};

class CappedPrefixTransform : public SliceTransform {
 private:
  size_t cap_len_;
  std::string id_;

 public:
  explicit CappedPrefixTransform(size_t cap_len) : cap_len_(cap_len) {
    id_ = std::string(kClassName()) + "." + std::to_string(cap_len_);
  }

  static const char* kClassName() { return "rocksdb.CappedPrefix"; }
  static const char* kNickName() { return "capped"; }
  const char* Name() const override { return kClassName(); }
  const char* NickName() const override { return kNickName(); }
  std::string GetId() const override { return id_; }

  bool IsInstanceOf(const std::string& name) const override {
    if (name == id_) {
      return true;
    } else if (StartsWith(name, kNickName())) {
      std::string alt_id =
          std::string(kNickName()) + ":" + std::to_string(cap_len_);
      if (name == alt_id) {
        return true;
      }
    }
    return SliceTransform::IsInstanceOf(name);
  }

  Slice Transform(const Slice& src) const override {
    assert(InDomain(src));
    return Slice(src.data(), std::min(cap_len_, src.size()));
  }

  bool InDomain(const Slice& /*src*/) const override { return true; }

  bool FullLengthEnabled(size_t* len) const override {
    *len = cap_len_;
    return true;
  }

  bool SameResultWhenAppended(const Slice& prefix) const override {
    return prefix.size() >= cap_len_;
  }
};

// Each component has a boundary independent of the bytes following it.
// Consequently, successful parsing remains unchanged when a suffix is appended.
class LayoutPrefixTransform : public SliceTransform {
 public:
  static const char* kClassName() { return "rocksdb.LayoutPrefix"; }
  const char* Name() const override { return kClassName(); }
  std::string GetId() const override { return id_; }

  static const SliceTransform* Create(
      const std::string& uri, std::unique_ptr<const SliceTransform>* guard,
      std::string* errmsg) {
    const std::string prefix = std::string(kClassName()) + ".v1:";
    if (!StartsWith(uri, prefix) || uri.size() == prefix.size()) {
      *errmsg = "Expected rocksdb.LayoutPrefix.v1:<components>";
      return nullptr;
    }
    std::vector<Component> components;
    std::string id = prefix;
    size_t fixed_length = 0;
    bool all_fixed = true;
    size_t start = prefix.size();
    while (start < uri.size()) {
      const size_t comma = uri.find(',', start);
      const size_t end = comma == std::string::npos ? uri.size() : comma;
      const std::string token = uri.substr(start, end - start);
      Component component;
      if (token == "v32") {
        component = {Kind::kVarint32, 0};
      } else if (token == "v64") {
        component = {Kind::kVarint64, 0};
      } else if (token == "b32") {
        component = {Kind::kBytesVarint32, 0};
      } else if (token.size() > 1 && token[0] == 'f') {
        size_t length = 0;
        for (size_t i = 1; i < token.size(); ++i) {
          if (token[i] < '0' || token[i] > '9') {
            *errmsg = "Invalid fixed-length component: " + token;
            return nullptr;
          }
          const size_t digit = token[i] - '0';
          if (length > (std::numeric_limits<int32_t>::max() - digit) / 10) {
            *errmsg = "Fixed-length component exceeds int32: " + token;
            return nullptr;
          }
          length = length * 10 + digit;
        }
        if (length == 0 ||
            length > std::numeric_limits<size_t>::max() - fixed_length) {
          *errmsg = "Invalid total fixed length";
          return nullptr;
        }
        fixed_length += length;
        component = {Kind::kFixed, length};
      } else {
        *errmsg = "Unknown layout component: " + token;
        return nullptr;
      }
      all_fixed &= component.kind == Kind::kFixed;
      if (!components.empty()) {
        id += ',';
      }
      id += component.kind == Kind::kFixed
                ? "f" + std::to_string(component.length)
                : token;
      // Normalize the execution plan without changing the persisted identity.
      if (component.kind == Kind::kFixed && !components.empty() &&
          components.back().kind == Kind::kFixed) {
        components.back().length += component.length;
      } else {
        components.push_back(component);
      }
      if (comma == std::string::npos) {
        break;
      }
      start = comma + 1;
      if (start == uri.size()) {
        *errmsg = "Empty layout component";
        return nullptr;
      }
    }
    if (all_fixed) {
      guard->reset(NewFixedPrefixTransform(fixed_length));
    } else {
      guard->reset(
          new LayoutPrefixTransform(std::move(components), std::move(id)));
    }
    return guard->get();
  }

  bool InDomain(const Slice& key) const override {
    size_t end;
    return FindPrefixEnd(key, &end);
  }

  Slice Transform(const Slice& key) const override {
    size_t end = 0;
    const bool valid = FindPrefixEnd(key, &end);
    assert(valid);
    return Slice(key.data(), valid ? end : 0);
  }

  bool SameResultWhenAppended(const Slice& key) const override {
    return InDomain(key);
  }

 private:
  enum class Kind { kFixed, kVarint32, kVarint64, kBytesVarint32 };
  struct Component {
    Kind kind;
    size_t length;
  };

  LayoutPrefixTransform(std::vector<Component> components, std::string id)
      : components_(std::move(components)), id_(std::move(id)) {}

  template <unsigned Width, bool DecodeLength = false>
  static bool ConsumeVarint(const Slice& key, size_t* position,
                            uint32_t* length = nullptr) noexcept {
    static_assert(Width == 5 || Width == 10);
    static_assert(!DecodeLength || Width == 5);
    if (*position == key.size()) {
      return false;
    }
    unsigned char byte = key[(*position)++];
    if (byte < 0x80) {
      if constexpr (DecodeLength) {
        *length = byte;
      }
      return true;
    }
    [[maybe_unused]] uint32_t result = byte & 0x7f;
    for (unsigned i = 1; i < Width; ++i) {
      if (*position == key.size()) {
        return false;
      }
      byte = key[(*position)++];
      if (i == Width - 1 && byte > (Width == 5 ? 0x0f : 0x01)) {
        return false;
      }
      if constexpr (DecodeLength) {
        const uint32_t payload = byte & 0x7f;
        result |= payload << (7 * i);
      }
      if (byte < 0x80) {
        if constexpr (DecodeLength) {
          *length = result;
        }
        return true;
      }
    }
    return false;
  }

  bool FindPrefixEnd(const Slice& key, size_t* end) const noexcept {
    size_t position = 0;
    for (const auto& component : components_) {
      if (component.kind == Kind::kFixed) {
        if (component.length > key.size() - position) {
          return false;
        }
        position += component.length;
      } else if (component.kind == Kind::kBytesVarint32) {
        uint32_t length;
        if (!ConsumeVarint<5, true>(key, &position, &length) ||
            length > key.size() - position) {
          return false;
        }
        position += length;
      } else {
        const bool valid = component.kind == Kind::kVarint64
                               ? ConsumeVarint<10>(key, &position)
                               : ConsumeVarint<5>(key, &position);
        if (!valid) {
          return false;
        }
      }
    }
    *end = position;
    return true;
  }

  const std::vector<Component> components_;
  const std::string id_;
};

class NoopTransform : public SliceTransform {
 public:
  explicit NoopTransform() = default;

  static const char* kClassName() { return "rocksdb.Noop"; }
  const char* Name() const override { return kClassName(); }

  Slice Transform(const Slice& src) const override { return src; }

  bool InDomain(const Slice& /*src*/) const override { return true; }

  bool SameResultWhenAppended(const Slice& /*prefix*/) const override {
    return false;
  }
};

}  // end namespace

const SliceTransform* NewFixedPrefixTransform(size_t prefix_len) {
  return new FixedPrefixTransform(prefix_len);
}

const SliceTransform* NewCappedPrefixTransform(size_t cap_len) {
  return new CappedPrefixTransform(cap_len);
}

const SliceTransform* NewNoopTransform() { return new NoopTransform; }

static int RegisterBuiltinSliceTransform(ObjectLibrary& library,
                                         const std::string& /*arg*/) {
  // For the builtin transforms, the format is typically
  // [Name].[0-9]+ or [NickName]:[0-9]+
  library.AddFactory<const SliceTransform>(
      NoopTransform::kClassName(),
      [](const std::string& /*uri*/,
         std::unique_ptr<const SliceTransform>* guard,
         std::string* /*errmsg*/) {
        guard->reset(NewNoopTransform());
        return guard->get();
      });
  library.AddFactory<const SliceTransform>(
      ObjectLibrary::PatternEntry(LayoutPrefixTransform::kClassName())
          .AddSeparator(".", false),
      LayoutPrefixTransform::Create);
  library.AddFactory<const SliceTransform>(
      ObjectLibrary::PatternEntry(FixedPrefixTransform::kNickName(), false)
          .AddNumber(":"),
      [](const std::string& uri, std::unique_ptr<const SliceTransform>* guard,
         std::string* /*errmsg*/) {
        auto colon = uri.find(':');
        auto len = ParseSizeT(uri.substr(colon + 1));
        guard->reset(NewFixedPrefixTransform(len));
        return guard->get();
      });
  library.AddFactory<const SliceTransform>(
      ObjectLibrary::PatternEntry(FixedPrefixTransform::kClassName(), false)
          .AddNumber("."),
      [](const std::string& uri, std::unique_ptr<const SliceTransform>* guard,
         std::string* /*errmsg*/) {
        auto len = ParseSizeT(
            uri.substr(strlen(FixedPrefixTransform::kClassName()) + 1));
        guard->reset(NewFixedPrefixTransform(len));
        return guard->get();
      });
  library.AddFactory<const SliceTransform>(
      ObjectLibrary::PatternEntry(CappedPrefixTransform::kNickName(), false)
          .AddNumber(":"),
      [](const std::string& uri, std::unique_ptr<const SliceTransform>* guard,
         std::string* /*errmsg*/) {
        auto colon = uri.find(':');
        auto len = ParseSizeT(uri.substr(colon + 1));
        guard->reset(NewCappedPrefixTransform(len));
        return guard->get();
      });
  library.AddFactory<const SliceTransform>(
      ObjectLibrary::PatternEntry(CappedPrefixTransform::kClassName(), false)
          .AddNumber("."),
      [](const std::string& uri, std::unique_ptr<const SliceTransform>* guard,
         std::string* /*errmsg*/) {
        auto len = ParseSizeT(
            uri.substr(strlen(CappedPrefixTransform::kClassName()) + 1));
        guard->reset(NewCappedPrefixTransform(len));
        return guard->get();
      });
  size_t num_types;
  return static_cast<int>(library.GetFactoryCount(&num_types));
}

Status SliceTransform::CreateFromString(
    const ConfigOptions& config_options, const std::string& value,
    std::shared_ptr<const SliceTransform>* result) {
  static std::once_flag once;
  std::call_once(once, [&]() {
    RegisterBuiltinSliceTransform(*(ObjectLibrary::Default().get()), "");
  });
  std::string id;
  std::unordered_map<std::string, std::string> opt_map;
  Status status = Customizable::GetOptionsMap(config_options, result->get(),
                                              value, &id, &opt_map);
  if (!status.ok()) {  // GetOptionsMap failed
    return status;
  } else if (id.empty() && opt_map.empty()) {
    result->reset();
  } else {
    status = config_options.registry->NewSharedObject(id, result);
    if (config_options.ignore_unsupported_options && status.IsNotSupported()) {
      return Status::OK();
    } else if (status.ok()) {
      SliceTransform* transform = const_cast<SliceTransform*>(result->get());
      status =
          Customizable::ConfigureNewObject(config_options, transform, opt_map);
    }
  }
  return status;
}

std::string SliceTransform::AsString() const {
  if (HasRegisteredOptions()) {
    ConfigOptions opts;
    opts.delimiter = ";";
    return ToString(opts);
  }
  return GetId();
}

// 2 small internal utility functions, for efficient hex conversions
// and no need for snprintf, toupper etc...
// Originally from wdt/util/EncryptionUtils.cpp - for
// std::to_string(true)/DecodeHex:
char toHex(unsigned char v) {
  if (v <= 9) {
    return '0' + v;
  }
  return 'A' + v - 10;
}
// most of the code is for validation/error check
int fromHex(char c) {
  // toupper:
  if (c >= 'a' && c <= 'f') {
    c -= ('a' - 'A');  // aka 0x20
  }
  // validation
  if (c < '0' || (c > '9' && (c < 'A' || c > 'F'))) {
    return -1;  // invalid not 0-9A-F hex char
  }
  if (c <= '9') {
    return c - '0';
  }
  return c - 'A' + 10;
}

Slice::Slice(const SliceParts& parts, std::string* buf) {
  size_t length = 0;
  for (int i = 0; i < parts.num_parts; ++i) {
    length += parts.parts[i].size();
  }
  buf->reserve(length);

  for (int i = 0; i < parts.num_parts; ++i) {
    buf->append(parts.parts[i].data(), parts.parts[i].size());
  }
  data_ = buf->data();
  size_ = buf->size();
}

// Return a string that contains the copy of the referenced data.
std::string Slice::ToString(bool hex) const {
  std::string result;  // RVO/NRVO/move
  if (hex) {
    result.reserve(2 * size_);
    for (size_t i = 0; i < size_; ++i) {
      unsigned char c = data_[i];
      result.push_back(toHex(c >> 4));
      result.push_back(toHex(c & 0xf));
    }
    return result;
  } else {
    result.assign(data_, size_);
    return result;
  }
}

// Originally from rocksdb/utilities/ldb_cmd.h
bool Slice::DecodeHex(std::string* result) const {
  std::string::size_type len = size_;
  if (len % 2) {
    // Hex string must be even number of hex digits to get complete bytes back
    return false;
  }
  if (!result) {
    return false;
  }
  result->clear();
  result->reserve(len / 2);

  for (size_t i = 0; i < len;) {
    int h1 = fromHex(data_[i++]);
    if (h1 < 0) {
      return false;
    }
    int h2 = fromHex(data_[i++]);
    if (h2 < 0) {
      return false;
    }
    result->push_back(static_cast<char>((h1 << 4) | h2));
  }
  return true;
}

PinnableSlice::PinnableSlice(PinnableSlice&& other) : PinnableSlice() {
  *this = std::move(other);
}

PinnableSlice& PinnableSlice::operator=(PinnableSlice&& other) {
  if (this != &other) {
    Cleanable::Reset();
    Cleanable::operator=(std::move(other));
    size_ = other.size_;
    pinned_ = other.pinned_;
    if (pinned_) {
      data_ = other.data_;
      // When it's pinned, buf should no longer be of use.
    } else {
      if (other.buf_ == &other.self_space_) {
        self_space_ = std::move(other.self_space_);
        buf_ = &self_space_;
        data_ = buf_->data();
      } else {
        buf_ = other.buf_;
        data_ = other.data_;
      }
    }
    other.self_space_.clear();
    other.buf_ = &other.self_space_;
    other.pinned_ = false;
    other.PinSelf();
  }
  return *this;
}

}  // namespace ROCKSDB_NAMESPACE
