//  Copyright (c) Meta Platforms, Inc. and affiliates.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <utility>

#include "rocksdb/compaction_filter.h"
#include "rocksdb/env.h"
#include "rocksdb/slice.h"

namespace ROCKSDB_NAMESPACE {
namespace fluss {

inline constexpr std::size_t BITS_PER_BYTE = 8;
inline constexpr std::size_t TIMESTAMP_BYTE_SIZE = 8;
inline constexpr int64_t JAVA_MIN_LONG = std::numeric_limits<int64_t>::min();
inline constexpr int64_t JAVA_MAX_LONG = std::numeric_limits<int64_t>::max();
inline constexpr std::size_t JAVA_MAX_SIZE = 0x7fffffff;

class FlussTtlCompactionFilter : public CompactionFilter {
 public:
  enum StateType { Disabled, Value, List };

  class TimeProvider {
   public:
    virtual ~TimeProvider() = default;
    virtual bool CurrentTimestamp(int64_t* current_timestamp) const = 0;
  };

  class ListElementFilter {
   public:
    virtual ~ListElementFilter() = default;
    virtual bool NextUnexpiredOffset(const Slice& list, int64_t ttl,
                                     int64_t current_timestamp,
                                     int64_t* offset) const = 0;
  };

  class FixedListElementFilter : public ListElementFilter {
   public:
    FixedListElementFilter(std::size_t fixed_size, std::size_t timestamp_offset,
                           std::shared_ptr<Logger> logger)
        : fixed_size_(fixed_size),
          timestamp_offset_(timestamp_offset),
          logger_(std::move(logger)) {}

    bool NextUnexpiredOffset(const Slice& list, int64_t ttl,
                             int64_t current_timestamp,
                             int64_t* offset) const override;

   private:
    std::size_t fixed_size_;
    std::size_t timestamp_offset_;
    std::shared_ptr<Logger> logger_;
  };

  class ListElementFilterFactory {
   public:
    virtual ~ListElementFilterFactory() = default;
    virtual std::unique_ptr<ListElementFilter> CreateListElementFilter(
        std::shared_ptr<Logger> logger) const = 0;
  };

  class FixedListElementFilterFactory : public ListElementFilterFactory {
   public:
    FixedListElementFilterFactory(std::size_t fixed_size,
                                  std::size_t timestamp_offset)
        : fixed_size_(fixed_size), timestamp_offset_(timestamp_offset) {}

    std::unique_ptr<ListElementFilter> CreateListElementFilter(
        std::shared_ptr<Logger> logger) const override {
      return std::make_unique<FixedListElementFilter>(
          fixed_size_, timestamp_offset_, std::move(logger));
    }

   private:
    std::size_t fixed_size_;
    std::size_t timestamp_offset_;
  };

  struct Config {
    Config(
        StateType state_type, std::size_t timestamp_offset, int64_t ttl,
        int64_t query_time_after_num_entries,
        std::unique_ptr<ListElementFilterFactory> list_element_filter_factory)
        : state_type_(state_type),
          timestamp_offset_(timestamp_offset),
          ttl_(ttl),
          query_time_after_num_entries_(query_time_after_num_entries),
          list_element_filter_factory_(std::move(list_element_filter_factory)) {
    }

    const StateType state_type_;
    const std::size_t timestamp_offset_;
    const int64_t ttl_;
    const int64_t query_time_after_num_entries_;
    const std::unique_ptr<ListElementFilterFactory>
        list_element_filter_factory_;
  };

  class ConfigHolder {
   public:
    ConfigHolder();
    ~ConfigHolder();

    bool Configure(std::unique_ptr<Config> config);
    const Config* GetConfig() const;

   private:
    std::atomic<const Config*> config_;
  };

  FlussTtlCompactionFilter(std::shared_ptr<ConfigHolder> config_holder,
                           std::unique_ptr<TimeProvider> time_provider);
  FlussTtlCompactionFilter(std::shared_ptr<ConfigHolder> config_holder,
                           std::unique_ptr<TimeProvider> time_provider,
                           std::shared_ptr<Logger> logger);

  const char* Name() const override;
  Decision FilterV2(int level, const Slice& key, ValueType value_type,
                    const Slice& existing_value, std::string* new_value,
                    std::string* skip_until) const override;
  bool IgnoreSnapshots() const override { return true; }

 private:
  static const Config& DisabledConfig();

  void InitConfigIfNotYet() const;
  bool UpdateCurrentTimestampIfStale() const;
  void CreateListElementFilterIfNull() const;
  Decision ListDecide(const Slice& existing_value,
                      std::string* new_value) const;

  std::shared_ptr<ConfigHolder> config_holder_;
  std::unique_ptr<TimeProvider> time_provider_;
  std::shared_ptr<Logger> logger_;
  const Config* config_cached_;
  std::unique_ptr<ListElementFilter> list_element_filter_;
  int64_t current_timestamp_ = std::numeric_limits<int64_t>::max();
  int64_t record_counter_ = std::numeric_limits<int64_t>::max();
};

}  // namespace fluss
}  // namespace ROCKSDB_NAMESPACE
