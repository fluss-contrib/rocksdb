//  Copyright (c) Meta Platforms, Inc. and affiliates.
// Copyright (c) 2024-present, platinumhamburg. All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include "utilities/compaction_filters/floor_set_compaction_filter.h"

#include <cstdint>
#include <limits>
#include <string>
#include <unordered_set>
#include <utility>

#include "test_util/testharness.h"

namespace ROCKSDB_NAMESPACE {
namespace {

void AppendBigEndianInt64(int64_t value, std::string* target) {
  uint64_t bits = static_cast<uint64_t>(value);
  for (int shift = 56; shift >= 0; shift -= 8) {
    target->push_back(static_cast<char>((bits >> shift) & 0xff));
  }
}

std::string EncodeValue(int64_t tag) {
  std::string value = "ab";
  AppendBigEndianInt64(tag, &value);
  value.append("payload");
  return value;
}

bool ShouldFilter(const FloorSetCompactionFilter& filter,
                  const std::string& value) {
  std::string new_value;
  bool value_changed = false;
  return filter.Filter(0, Slice("key"), Slice(value), &new_value,
                       &value_changed);
}

TEST(FloorSetCompactionFilterTest, FiltersByFloorOrExplicitSet) {
  FloorSetCompactionFilter filter(2, 50, {100, 200});

  struct TestCase {
    int64_t tag;
    bool filtered;
  };
  const TestCase test_cases[] = {
      {30, true},  {50, true},  {51, false},
      {100, true}, {200, true}, {300, false},
  };

  for (const auto& test_case : test_cases) {
    SCOPED_TRACE("tag=" + std::to_string(test_case.tag));
    EXPECT_EQ(test_case.filtered,
              ShouldFilter(filter, EncodeValue(test_case.tag)));
  }
}

TEST(FloorSetCompactionFilterTest, DecodesSignedBigEndianTags) {
  FloorSetCompactionFilter filter(2, -1, {7});

  struct TestCase {
    int64_t tag;
    bool filtered;
  };
  const TestCase test_cases[] = {
      {std::numeric_limits<int64_t>::min(), true},
      {-2, true},
      {-1, true},
      {0, false},
      {7, true},
      {8, false},
      {std::numeric_limits<int64_t>::max(), false},
  };

  for (const auto& test_case : test_cases) {
    SCOPED_TRACE("tag=" + std::to_string(test_case.tag));
    EXPECT_EQ(test_case.filtered,
              ShouldFilter(filter, EncodeValue(test_case.tag)));
  }
}

TEST(FloorSetCompactionFilterTest, KeepsValuesWithoutACompleteTag) {
  FloorSetCompactionFilter filter(2, std::numeric_limits<int64_t>::max(), {});

  for (size_t length = 0; length < 10; ++length) {
    SCOPED_TRACE("length=" + std::to_string(length));
    EXPECT_FALSE(ShouldFilter(filter, std::string(length, 'x')));
  }

  EXPECT_TRUE(ShouldFilter(filter, EncodeValue(42).substr(0, 10)));
}

TEST(FloorSetCompactionFilterTest, KeepsValuesForInvalidOffsets) {
  FloorSetCompactionFilter negative_offset_filter(
      -1, std::numeric_limits<int64_t>::max(), {});
  FloorSetCompactionFilter oversized_offset_filter(
      std::numeric_limits<int>::max(), std::numeric_limits<int64_t>::max(), {});
  const std::string value = EncodeValue(42);

  EXPECT_FALSE(ShouldFilter(negative_offset_filter, value));
  EXPECT_FALSE(ShouldFilter(oversized_offset_filter, value));
}

TEST(FloorSetCompactionFilterTest, ReportsStableName) {
  FloorSetCompactionFilter filter(2, 0, {});
  EXPECT_STREQ("FloorSetCompactionFilter", filter.Name());
}

}  // namespace
}  // namespace ROCKSDB_NAMESPACE

int main(int argc, char** argv) {
  ROCKSDB_NAMESPACE::port::InstallStackTraceHandler();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
