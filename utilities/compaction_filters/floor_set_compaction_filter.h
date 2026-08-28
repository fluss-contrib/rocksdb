//  Copyright (c) Meta Platforms, Inc. and affiliates.
// Copyright (c) 2024-present, platinumhamburg. All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <unordered_set>
#include <utility>

#include "rocksdb/compaction_filter.h"
#include "rocksdb/slice.h"

namespace ROCKSDB_NAMESPACE {

// Removes entries whose signed, big-endian int64 tag is less than or equal
// to the floor or is present in the explicit set. The tag starts at the
// caller-provided byte offset. Values without a complete tag are kept.
class FloorSetCompactionFilter : public CompactionFilter {
 public:
  FloorSetCompactionFilter(int tag_offset, int64_t floor,
                           std::unordered_set<int64_t> explicit_set)
      : tag_offset_(tag_offset),
        floor_(floor),
        explicit_set_(std::move(explicit_set)) {}

  bool Filter(int /*level*/, const Slice& /*key*/, const Slice& existing_value,
              std::string* /*new_value*/,
              bool* /*value_changed*/) const override {
    if (tag_offset_ < 0) {
      return false;
    }

    const std::size_t tag_offset = static_cast<std::size_t>(tag_offset_);
    if (existing_value.size() < tag_offset ||
        existing_value.size() - tag_offset < sizeof(uint64_t)) {
      return false;
    }

    const int64_t tag =
        DecodeBigEndianInt64(existing_value.data() + tag_offset);
    return tag <= floor_ || explicit_set_.count(tag) != 0;
  }

  static const char* kClassName() { return "FloorSetCompactionFilter"; }

  const char* Name() const override { return kClassName(); }

 private:
  static int64_t DecodeBigEndianInt64(const char* data) {
    uint64_t bits = 0;
    for (std::size_t i = 0; i < sizeof(uint64_t); ++i) {
      bits = (bits << 8) |
             static_cast<uint64_t>(static_cast<unsigned char>(data[i]));
    }
    if (bits <= static_cast<uint64_t>(std::numeric_limits<int64_t>::max())) {
      return static_cast<int64_t>(bits);
    }
    return -1 - static_cast<int64_t>(~bits);
  }

  const int tag_offset_;
  const int64_t floor_;
  const std::unordered_set<int64_t> explicit_set_;
};

}  // namespace ROCKSDB_NAMESPACE
