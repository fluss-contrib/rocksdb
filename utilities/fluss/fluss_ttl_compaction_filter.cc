//  Copyright (c) Meta Platforms, Inc. and affiliates.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include "utilities/fluss/fluss_ttl_compaction_filter.h"

#include <algorithm>
#include <cinttypes>

namespace ROCKSDB_NAMESPACE {
namespace fluss {
namespace {

int64_t DeserializeTimestamp(const char* source, std::size_t offset) {
  uint64_t result = 0;
  for (std::size_t i = 0; i < sizeof(uint64_t); ++i) {
    result |=
        static_cast<uint64_t>(static_cast<unsigned char>(source[offset + i]))
        << ((sizeof(uint64_t) - 1 - i) * BITS_PER_BYTE);
  }
  return static_cast<int64_t>(result);
}

CompactionFilter::Decision DecideTimestamp(
    const char* timestamp_bytes, int64_t ttl, std::size_t timestamp_offset,
    int64_t current_timestamp, const std::shared_ptr<Logger>& logger) {
  const int64_t timestamp =
      DeserializeTimestamp(timestamp_bytes, timestamp_offset);
  const int64_t ttl_without_overflow =
      timestamp > 0 ? std::min(JAVA_MAX_LONG - timestamp, ttl) : ttl;
  Debug(logger.get(),
        "Last access timestamp: %" PRId64 " ms, ttlWithoutOverflow: %" PRId64
        " ms, Current timestamp: %" PRId64 " ms",
        timestamp, ttl_without_overflow, current_timestamp);
  return timestamp + ttl_without_overflow <= current_timestamp
             ? CompactionFilter::Decision::kRemove
             : CompactionFilter::Decision::kKeep;
}

bool ContainsTimestamp(const Slice& value, std::size_t offset) {
  return offset <= value.size() && TIMESTAMP_BYTE_SIZE <= value.size() - offset;
}

}  // namespace

const FlussTtlCompactionFilter::Config&
FlussTtlCompactionFilter::DisabledConfig() {
  static const Config disabled(Disabled, 0, JAVA_MAX_LONG, JAVA_MAX_LONG,
                               nullptr);
  return disabled;
}

FlussTtlCompactionFilter::ConfigHolder::ConfigHolder()
    : config_(&DisabledConfig()) {}

FlussTtlCompactionFilter::ConfigHolder::~ConfigHolder() {
  const Config* config = config_.load(std::memory_order_acquire);
  if (config != &DisabledConfig()) {
    delete config;
  }
}

bool FlussTtlCompactionFilter::ConfigHolder::Configure(
    std::unique_ptr<Config> config) {
  if (config == nullptr || config->query_time_after_num_entries_ < 0) {
    return false;
  }
  const Config* expected = &DisabledConfig();
  const Config* desired = config.get();
  if (!config_.compare_exchange_strong(expected, desired,
                                       std::memory_order_release,
                                       std::memory_order_acquire)) {
    return false;
  }
  config.release();
  return true;
}

const FlussTtlCompactionFilter::Config*
FlussTtlCompactionFilter::ConfigHolder::GetConfig() const {
  return config_.load(std::memory_order_acquire);
}

bool FlussTtlCompactionFilter::FixedListElementFilter::NextUnexpiredOffset(
    const Slice& list, int64_t ttl, int64_t current_timestamp,
    int64_t* result_offset) const {
  if (result_offset == nullptr || fixed_size_ == 0) {
    return false;
  }
  std::size_t offset = 0;
  while (offset < list.size()) {
    if (timestamp_offset_ >= fixed_size_ ||
        !ContainsTimestamp(list, offset + timestamp_offset_)) {
      return false;
    }
    if (DecideTimestamp(list.data(), ttl, offset + timestamp_offset_,
                        current_timestamp, logger_) == Decision::kKeep) {
      break;
    }
    if (fixed_size_ > list.size() - offset) {
      return false;
    }
    offset += fixed_size_;
  }
  if (offset > static_cast<std::size_t>(JAVA_MAX_LONG)) {
    return false;
  }
  *result_offset = static_cast<int64_t>(offset);
  return true;
}

FlussTtlCompactionFilter::FlussTtlCompactionFilter(
    std::shared_ptr<ConfigHolder> config_holder,
    std::unique_ptr<TimeProvider> time_provider)
    : FlussTtlCompactionFilter(std::move(config_holder),
                               std::move(time_provider), nullptr) {}

FlussTtlCompactionFilter::FlussTtlCompactionFilter(
    std::shared_ptr<ConfigHolder> config_holder,
    std::unique_ptr<TimeProvider> time_provider, std::shared_ptr<Logger> logger)
    : config_holder_(std::move(config_holder)),
      time_provider_(std::move(time_provider)),
      logger_(std::move(logger)),
      config_cached_(&DisabledConfig()) {}

const char* FlussTtlCompactionFilter::Name() const {
  return "FlussTtlCompactionFilter";
}

void FlussTtlCompactionFilter::InitConfigIfNotYet() const {
  if (config_cached_ == &DisabledConfig()) {
    const_cast<FlussTtlCompactionFilter*>(this)->config_cached_ =
        config_holder_->GetConfig();
  }
}

void FlussTtlCompactionFilter::CreateListElementFilterIfNull() const {
  if (list_element_filter_ == nullptr &&
      config_cached_->list_element_filter_factory_ != nullptr) {
    const_cast<FlussTtlCompactionFilter*>(this)->list_element_filter_ =
        config_cached_->list_element_filter_factory_->CreateListElementFilter(
            logger_);
  }
}

bool FlussTtlCompactionFilter::UpdateCurrentTimestampIfStale() const {
  if (record_counter_ >= config_cached_->query_time_after_num_entries_) {
    int64_t current_timestamp;
    if (time_provider_ == nullptr ||
        !time_provider_->CurrentTimestamp(&current_timestamp)) {
      Error(logger_.get(), "Unable to obtain current timestamp");
      return false;
    }
    auto* self = const_cast<FlussTtlCompactionFilter*>(this);
    self->current_timestamp_ = current_timestamp;
    self->record_counter_ = 0;
  }
  const_cast<FlussTtlCompactionFilter*>(this)->record_counter_++;
  return true;
}

CompactionFilter::Decision FlussTtlCompactionFilter::FilterV2(
    int /*level*/, const Slice& key, ValueType value_type,
    const Slice& existing_value, std::string* new_value,
    std::string* /*skip_until*/) const {
  InitConfigIfNotYet();
  CreateListElementFilterIfNull();
  if (!UpdateCurrentTimestampIfStale()) {
    return Decision::kKeep;
  }

  if (logger_ != nullptr &&
      logger_->GetInfoLogLevel() <= InfoLogLevel::DEBUG_LEVEL) {
    Debug(logger_.get(),
          "Call FlussTtlCompactionFilter::FilterV2 - Key: %s, Data: %s, "
          "Value type: %d, State type: %d, TTL: %" PRId64
          " ms, timestamp_offset: %zu",
          key.ToString().c_str(), existing_value.ToString(true).c_str(),
          static_cast<int>(value_type),
          static_cast<int>(config_cached_->state_type_), config_cached_->ttl_,
          config_cached_->timestamp_offset_);
  }

  const bool value_or_merge =
      value_type == ValueType::kValue || value_type == ValueType::kMergeOperand;
  const bool value_state = config_cached_->state_type_ == StateType::Value &&
                           value_type == ValueType::kValue;
  const bool list_entry =
      config_cached_->state_type_ == StateType::List && value_or_merge;

  Decision decision = Decision::kKeep;
  if (ContainsTimestamp(existing_value, config_cached_->timestamp_offset_)) {
    if (list_entry && list_element_filter_ != nullptr) {
      decision = ListDecide(existing_value, new_value);
    } else if (value_state) {
      decision = DecideTimestamp(existing_value.data(), config_cached_->ttl_,
                                 config_cached_->timestamp_offset_,
                                 current_timestamp_, logger_);
    }
  }
  Debug(logger_.get(), "Decision: %d", static_cast<int>(decision));
  return decision;
}

CompactionFilter::Decision FlussTtlCompactionFilter::ListDecide(
    const Slice& existing_value, std::string* new_value) const {
  if (DecideTimestamp(existing_value.data(), config_cached_->ttl_,
                      config_cached_->timestamp_offset_, current_timestamp_,
                      logger_) == Decision::kKeep) {
    return Decision::kKeep;
  }

  int64_t next_offset = 0;
  if (!list_element_filter_->NextUnexpiredOffset(
          existing_value, config_cached_->ttl_, current_timestamp_,
          &next_offset) ||
      next_offset <= 0 ||
      static_cast<uint64_t>(next_offset) > existing_value.size()) {
    Error(logger_.get(), "Invalid next offset in list filter: %" PRId64,
          next_offset);
    return Decision::kKeep;
  }

  const std::size_t offset = static_cast<std::size_t>(next_offset);
  if (offset == existing_value.size()) {
    return Decision::kRemove;
  }
  new_value->assign(existing_value.data() + offset,
                    existing_value.size() - offset);
  if (logger_ != nullptr &&
      logger_->GetInfoLogLevel() <= InfoLogLevel::DEBUG_LEVEL) {
    Debug(logger_.get(), "New list value: %s",
          Slice(*new_value).ToString(true).c_str());
  }
  return Decision::kChangeValue;
}

}  // namespace fluss
}  // namespace ROCKSDB_NAMESPACE
