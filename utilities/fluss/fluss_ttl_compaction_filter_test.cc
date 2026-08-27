//  Copyright (c) Meta Platforms, Inc. and affiliates.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2017-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include "utilities/fluss/fluss_ttl_compaction_filter.h"

#include <random>

#include "test_util/testharness.h"

namespace ROCKSDB_NAMESPACE {
namespace fluss {

#define DISABLED FlussTtlCompactionFilter::StateType::Disabled
#define VALUE FlussTtlCompactionFilter::StateType::Value
#define LIST FlussTtlCompactionFilter::StateType::List

#define KVALUE CompactionFilter::ValueType::kValue
#define KMERGE CompactionFilter::ValueType::kMergeOperand
#define KBLOB CompactionFilter::ValueType::kBlobIndex

#define KKEEP CompactionFilter::Decision::kKeep
#define KREMOVE CompactionFilter::Decision::kRemove
#define KCHANGE CompactionFilter::Decision::kChangeValue

#define EXPIRE (time += ttl + 20)

#define EXPECT_ARR_EQ(arr1, arr2, num) \
  EXPECT_TRUE(0 == memcmp(arr1, arr2, num));

static const std::size_t TEST_TIMESTAMP_OFFSET = static_cast<std::size_t>(2);

static const std::size_t LIST_ELEM_FIXED_LEN = static_cast<std::size_t>(8 + 4);

static const int64_t QUERY_TIME_AFTER_NUM_ENTRIES = static_cast<int64_t>(10);

class ConsoleLogger : public Logger {
 public:
  using Logger::Logv;
  ConsoleLogger() : Logger(InfoLogLevel::DEBUG_LEVEL) {}

  void Logv(const char* format, va_list ap) override {
    vprintf(format, ap);
    printf("\n");
  }
};

int64_t time = 0;

class TestTimeProvider : public FlussTtlCompactionFilter::TimeProvider {
 public:
  bool CurrentTimestamp(int64_t* current_timestamp) const override {
    *current_timestamp = time;
    return true;
  }
};

std::random_device rd;  // NOLINT
std::mt19937 mt(rd());  // NOLINT
std::uniform_int_distribution<int64_t> rnd(JAVA_MIN_LONG,
                                           JAVA_MAX_LONG);  // NOLINT

int64_t ttl = 100;

Slice key = Slice("key");  // NOLINT
char data[24];
std::string new_list = "";  // NOLINT
std::string stub = "";      // NOLINT

FlussTtlCompactionFilter::StateType state_type;
CompactionFilter::ValueType value_type;
FlussTtlCompactionFilter* filter;  // NOLINT

void SetTimestamp(int64_t timestamp, size_t offset = 0, char* value = data) {
  for (unsigned long i = 0; i < sizeof(uint64_t); i++) {
    value[offset + i] =
        static_cast<char>(static_cast<uint64_t>(timestamp) >>
                          ((sizeof(int64_t) - 1 - i) * BITS_PER_BYTE));
  }
}

CompactionFilter::Decision decide(size_t data_size = sizeof(data)) {
  return filter->FilterV2(0, key, value_type, Slice(data, data_size), &new_list,
                          &stub);
}

void Init(FlussTtlCompactionFilter::StateType stype,
          CompactionFilter::ValueType vtype,
          FlussTtlCompactionFilter::ListElementFilterFactory*
              fixed_len_filter_factory,
          size_t timestamp_offset, bool expired = false) {
  time = expired ? time + ttl + 20 : time;
  state_type = stype;
  value_type = vtype;

  auto config_holder =
      std::make_shared<FlussTtlCompactionFilter::ConfigHolder>();
  auto time_provider = new TestTimeProvider();
  auto logger = std::make_shared<ConsoleLogger>();

  filter = new FlussTtlCompactionFilter(
      config_holder,
      std::unique_ptr<FlussTtlCompactionFilter::TimeProvider>(time_provider),
      logger);
  auto config = std::make_unique<FlussTtlCompactionFilter::Config>(
      state_type, timestamp_offset, ttl, QUERY_TIME_AFTER_NUM_ENTRIES,
      std::unique_ptr<FlussTtlCompactionFilter::ListElementFilterFactory>(
          fixed_len_filter_factory));
  EXPECT_EQ(decide(), KKEEP);  // test disabled config
  EXPECT_TRUE(config_holder->Configure(std::move(config)));
  auto duplicate = std::make_unique<FlussTtlCompactionFilter::Config>(
      state_type, timestamp_offset, ttl, QUERY_TIME_AFTER_NUM_ENTRIES, nullptr);
  EXPECT_FALSE(config_holder->Configure(std::move(duplicate)));
}

void InitValue(FlussTtlCompactionFilter::StateType stype,
               CompactionFilter::ValueType vtype, bool expired = false,
               size_t timestamp_offset = TEST_TIMESTAMP_OFFSET) {
  time = rnd(mt);
  SetTimestamp(time, timestamp_offset);
  Init(stype, vtype, nullptr, timestamp_offset, expired);
}

void InitList(CompactionFilter::ValueType vtype, bool all_expired = false,
              bool first_elem_expired = false, size_t timestamp_offset = 0) {
  time = rnd(mt);
  SetTimestamp(first_elem_expired ? time - ttl - 20 : time,
               timestamp_offset);                              // elem 1 ts
  SetTimestamp(time, LIST_ELEM_FIXED_LEN + timestamp_offset);  // elem 2 ts
  auto fixed_len_filter_factory =
      new FlussTtlCompactionFilter::FixedListElementFilterFactory(
          LIST_ELEM_FIXED_LEN, static_cast<std::size_t>(0));
  Init(LIST, vtype, fixed_len_filter_factory, timestamp_offset, all_expired);
}

void Deinit() { delete filter; }

TEST(FlussStateTtlTest, CheckStateTypeEnumOrder) {  // NOLINT
  // if the order changes it also needs to be adjusted in Java client:
  // in org.fluss.rocksdb.FlussTtlCompactionFilter
  // and in org.fluss.rocksdb.FlussTtlCompactionFilterTest
  EXPECT_EQ(DISABLED, 0);
  EXPECT_EQ(VALUE, 1);
  EXPECT_EQ(LIST, 2);
}

TEST(FlussStateTtlTest, SkipShortDataWithoutTimestamp) {  // NOLINT
  InitValue(VALUE, KVALUE, true);
  EXPECT_EQ(decide(TIMESTAMP_BYTE_SIZE - 1), KKEEP);
  Deinit();
}

TEST(FlussValueStateTtlTest, Unexpired) {  // NOLINT
  InitValue(VALUE, KVALUE);
  EXPECT_EQ(decide(), KKEEP);
  Deinit();
}

TEST(FlussValueStateTtlTest, Expired) {  // NOLINT
  InitValue(VALUE, KVALUE, true);
  EXPECT_EQ(decide(), KREMOVE);
  Deinit();
}

TEST(FlussValueStateTtlTest, CachedTimeUpdate) {  // NOLINT
  InitValue(VALUE, KVALUE);
  EXPECT_EQ(decide(), KKEEP);  // also implicitly cache current timestamp
  EXPIRE;  // advance current timestamp to expire but cached should be used
  // QUERY_TIME_AFTER_NUM_ENTRIES - 2:
  // -1 -> for decide disabled in InitValue
  // and -1 -> for decide right after InitValue
  for (int64_t i = 0; i < QUERY_TIME_AFTER_NUM_ENTRIES - 2; i++) {
    EXPECT_EQ(decide(), KKEEP);
  }
  EXPECT_EQ(decide(), KREMOVE);  // advanced current timestamp should be updated
                                 // in cache and expire state
  Deinit();
}

TEST(FlussValueStateTtlTest, WrongFilterValueType) {  // NOLINT
  InitValue(VALUE, KMERGE, true);
  EXPECT_EQ(decide(), KKEEP);
  Deinit();
}

TEST(FlussListStateTtlTest, Unexpired) {  // NOLINT
  InitList(KMERGE);
  EXPECT_EQ(decide(), KKEEP);
  Deinit();

  InitList(KVALUE);
  EXPECT_EQ(decide(), KKEEP);
  Deinit();
}

TEST(FlussListStateTtlTest, Expired) {  // NOLINT
  InitList(KMERGE, true);
  EXPECT_EQ(decide(), KREMOVE);
  Deinit();

  InitList(KVALUE, true);
  EXPECT_EQ(decide(), KREMOVE);
  Deinit();
}

TEST(FlussListStateTtlTest, HalfExpired) {  // NOLINT
  InitList(KMERGE, false, true);
  EXPECT_EQ(decide(), KCHANGE);
  EXPECT_ARR_EQ(new_list.data(), data + LIST_ELEM_FIXED_LEN,
                LIST_ELEM_FIXED_LEN);
  Deinit();

  InitList(KVALUE, false, true);
  EXPECT_EQ(decide(), KCHANGE);
  EXPECT_ARR_EQ(new_list.data(), data + LIST_ELEM_FIXED_LEN,
                LIST_ELEM_FIXED_LEN);
  Deinit();
}

TEST(FlussListStateTtlTest, WrongFilterValueType) {  // NOLINT
  InitList(KBLOB, true);
  EXPECT_EQ(decide(), KKEEP);
  Deinit();
}

class RetryTimeProvider : public FlussTtlCompactionFilter::TimeProvider {
 public:
  bool CurrentTimestamp(int64_t* current_timestamp) const override {
    ++calls_;
    if (calls_ == 1) {
      return false;
    }
    *current_timestamp = time + ttl + 20;
    return true;
  }

  mutable int calls_ = 0;
};

class OffsetListElementFilter
    : public FlussTtlCompactionFilter::ListElementFilter {
 public:
  OffsetListElementFilter(bool succeeds, int64_t offset)
      : succeeds_(succeeds), offset_(offset) {}

  bool NextUnexpiredOffset(const Slice&, int64_t, int64_t,
                           int64_t* offset) const override {
    *offset = offset_;
    return succeeds_;
  }

 private:
  bool succeeds_;
  int64_t offset_;
};

class OffsetListElementFilterFactory
    : public FlussTtlCompactionFilter::ListElementFilterFactory {
 public:
  OffsetListElementFilterFactory(bool succeeds, int64_t offset)
      : succeeds_(succeeds), offset_(offset) {}

  std::unique_ptr<FlussTtlCompactionFilter::ListElementFilter>
  CreateListElementFilter(std::shared_ptr<Logger>) const override {
    return std::make_unique<OffsetListElementFilter>(succeeds_, offset_);
  }

 private:
  bool succeeds_;
  int64_t offset_;
};

std::unique_ptr<FlussTtlCompactionFilter> CreateFailureSafetyFilter(
    std::unique_ptr<FlussTtlCompactionFilter::TimeProvider> time_provider,
    bool callback_succeeds, int64_t callback_offset) {
  auto config_holder =
      std::make_shared<FlussTtlCompactionFilter::ConfigHolder>();
  auto config = std::make_unique<FlussTtlCompactionFilter::Config>(
      LIST, 0, ttl, 0,
      std::make_unique<OffsetListElementFilterFactory>(callback_succeeds,
                                                       callback_offset));
  EXPECT_TRUE(config_holder->Configure(std::move(config)));
  return std::make_unique<FlussTtlCompactionFilter>(std::move(config_holder),
                                                    std::move(time_provider));
}

TEST(FlussStateTtlFailureSafetyTest, TimeFailureKeepsAndRetries) {  // NOLINT
  time = 1000;
  SetTimestamp(time - ttl - 20);
  auto time_provider = std::make_unique<RetryTimeProvider>();
  auto* time_provider_ptr = time_provider.get();
  auto failure_safe_filter =
      CreateFailureSafetyFilter(std::move(time_provider), true, sizeof(data));

  EXPECT_EQ(failure_safe_filter->FilterV2(
                0, key, KVALUE, Slice(data, sizeof(data)), &new_list, &stub),
            KKEEP);
  EXPECT_EQ(failure_safe_filter->FilterV2(
                0, key, KVALUE, Slice(data, sizeof(data)), &new_list, &stub),
            KREMOVE);
  EXPECT_EQ(time_provider_ptr->calls_, 2);
}

TEST(FlussListStateTtlFailureSafetyTest, CallbackFailureKeeps) {  // NOLINT
  time = 1000;
  SetTimestamp(time - ttl - 20);
  auto failure_safe_filter = CreateFailureSafetyFilter(
      std::make_unique<TestTimeProvider>(), false, sizeof(data));
  EXPECT_EQ(failure_safe_filter->FilterV2(
                0, key, KVALUE, Slice(data, sizeof(data)), &new_list, &stub),
            KKEEP);
}

TEST(FlussListStateTtlFailureSafetyTest, InvalidOffsetsKeep) {  // NOLINT
  time = 1000;
  SetTimestamp(time - ttl - 20);
  for (const int64_t invalid_offset :
       {int64_t{-1}, int64_t{0}, static_cast<int64_t>(sizeof(data) + 1)}) {
    auto failure_safe_filter = CreateFailureSafetyFilter(
        std::make_unique<TestTimeProvider>(), true, invalid_offset);
    EXPECT_EQ(failure_safe_filter->FilterV2(
                  0, key, KVALUE, Slice(data, sizeof(data)), &new_list, &stub),
              KKEEP);
  }
}

TEST(FlussListStateTtlFailureSafetyTest,
     OffsetAtValueLengthRemovesList) {  // NOLINT
  time = 1000;
  SetTimestamp(time - ttl - 20);
  auto failure_safe_filter = CreateFailureSafetyFilter(
      std::make_unique<TestTimeProvider>(), true, sizeof(data));
  EXPECT_EQ(failure_safe_filter->FilterV2(
                0, key, KVALUE, Slice(data, sizeof(data)), &new_list, &stub),
            KREMOVE);
}

}  // namespace fluss
}  // namespace ROCKSDB_NAMESPACE

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
