/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
