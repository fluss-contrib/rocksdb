//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "rocksdb/slice_transform.h"

#include <algorithm>
#include <atomic>
#include <functional>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "env/composite_env_wrapper.h"
#include "rocksdb/convenience.h"
#include "rocksdb/db.h"
#include "rocksdb/env.h"
#include "rocksdb/file_system.h"
#include "rocksdb/filter_policy.h"
#include "rocksdb/statistics.h"
#include "rocksdb/table.h"
#include "rocksdb/utilities/options_util.h"
#include "test_util/testharness.h"
#include "util/coding.h"
#include "util/random.h"

namespace ROCKSDB_NAMESPACE {

class SliceTransformTest : public testing::Test {};

TEST_F(SliceTransformTest, CapPrefixTransform) {
  std::string s;
  s = "abcdefge";

  std::unique_ptr<const SliceTransform> transform;

  transform.reset(NewCappedPrefixTransform(6));
  ASSERT_EQ(transform->Transform(s).ToString(), "abcdef");
  ASSERT_TRUE(transform->SameResultWhenAppended("123456"));
  ASSERT_TRUE(transform->SameResultWhenAppended("1234567"));
  ASSERT_TRUE(!transform->SameResultWhenAppended("12345"));

  transform.reset(NewCappedPrefixTransform(8));
  ASSERT_EQ(transform->Transform(s).ToString(), "abcdefge");

  transform.reset(NewCappedPrefixTransform(10));
  ASSERT_EQ(transform->Transform(s).ToString(), "abcdefge");

  transform.reset(NewCappedPrefixTransform(0));
  ASSERT_EQ(transform->Transform(s).ToString(), "");

  transform.reset(NewCappedPrefixTransform(0));
  ASSERT_EQ(transform->Transform("").ToString(), "");
}

namespace {
Status CreateLayout(const std::string& components,
                    std::shared_ptr<const SliceTransform>* result) {
  ConfigOptions config;
  config.ignore_unsupported_options = false;
  return SliceTransform::CreateFromString(
      config, "rocksdb.LayoutPrefix.v1:" + components, result);
}

class CountingPrefixTransform : public SliceTransform {
 public:
  explicit CountingPrefixTransform(
      std::shared_ptr<const SliceTransform> delegate)
      : delegate_(std::move(delegate)) {}
  const char* Name() const override { return delegate_->Name(); }
  std::string GetId() const override { return delegate_->GetId(); }
  bool InDomain(const Slice& key) const override {
    ++domain_calls;
    return delegate_->InDomain(key);
  }
  Slice Transform(const Slice& key) const override {
    ++transform_calls;
    return delegate_->Transform(key);
  }
  mutable std::atomic<unsigned> domain_calls{0};
  mutable std::atomic<unsigned> transform_calls{0};

 private:
  const std::shared_ptr<const SliceTransform> delegate_;
};

// Complete reads only when polled, independently of host async I/O support.
class DeferredReadFS : public FileSystemWrapper {
 public:
  DeferredReadFS() : FileSystemWrapper(FileSystem::Default()) {}
  const char* Name() const override { return "DeferredReadFS"; }
  void SupportedOps(int64_t& ops) override {
    target()->SupportedOps(ops);
    ops |= (1 << FSSupportedOps::kAsyncIO);
  }

  IOStatus NewRandomAccessFile(const std::string& name, const FileOptions& opts,
                               std::unique_ptr<FSRandomAccessFile>* result,
                               IODebugContext* dbg) override {
    std::unique_ptr<FSRandomAccessFile> file;
    IOStatus s = target()->NewRandomAccessFile(name, opts, &file, dbg);
    if (s.ok()) {
      result->reset(new DeferredReadFile(std::move(file)));
    }
    return s;
  }

  IOStatus Poll(std::vector<void*>& handles,
                size_t /*min_completions*/) override {
    for (void* handle : handles) {
      auto* pending = static_cast<PendingRead*>(handle);
      if (pending->complete) {
        auto complete = std::move(pending->complete);
        pending->complete = nullptr;
        complete();
        ++completed_reads;
      }
    }
    return IOStatus::OK();
  }

  IOStatus AbortIO(std::vector<void*>& handles) override {
    return Poll(handles, handles.size());
  }

  unsigned completed_reads = 0;

 private:
  struct PendingRead {
    std::function<void()> complete;
  };

  class DeferredReadFile : public FSRandomAccessFileOwnerWrapper {
   public:
    explicit DeferredReadFile(std::unique_ptr<FSRandomAccessFile> file)
        : FSRandomAccessFileOwnerWrapper(std::move(file)) {}

    IOStatus ReadAsync(FSReadRequest& req, const IOOptions& opts,
                       std::function<void(FSReadRequest&, void*)> cb,
                       void* cb_arg, void** handle, IOHandleDeleter* deleter,
                       IODebugContext* /*dbg*/) override {
      auto* pending = new PendingRead;
      pending->complete = [this, offset = req.offset, len = req.len,
                           scratch = req.scratch, opts, cb = std::move(cb),
                           cb_arg]() mutable {
        FSReadRequest request;
        request.offset = offset;
        request.len = len;
        request.scratch = scratch;
        request.status =
            target()->Read(request.offset, request.len, opts, &request.result,
                           request.scratch, nullptr);
        cb(request, cb_arg);
        request.status.PermitUncheckedError();
      };
      *handle = pending;
      *deleter = [](void* arg) { delete static_cast<PendingRead*>(arg); };
      return IOStatus::OK();
    }
  };
};

std::string EncodedPrefix(uint32_t number, const std::string& bytes) {
  std::string result;
  PutVarint32(&result, number);
  PutLengthPrefixedSlice(&result, bytes);
  return result;
}
}  // namespace

TEST_F(SliceTransformTest, LayoutComponentsAndDomain) {
  struct Case {
    const char* layout;
    std::string prefix;
  };
  const Case cases[] = {
      {"f2,v32", std::string("ab\x80\x01", 4)},
      {"v32", std::string("\xff\xff\xff\xff\x0f", 5)},
      {"v64", std::string(9, '\xff') + '\x01'},
      {"b32", std::string("\x03\x00\xff\x80", 4)},
      {"v32,b32", EncodedPrefix(128, "")},
      {"v32,b32", EncodedPrefix(16384, std::string(128, '\xff'))},
      {"f2,f3", "abcde"},
      {"v32", std::string("\x80\x00", 2)},
      {"v64", std::string(9, '\x80') + '\x00'},
      {"b32", std::string("\x80\x00", 2)},
      {"f1,f2,v32,f1,f2", std::string("abc\x80\x01"
                                      "def",
                                      8)},
  };
  for (const auto& c : cases) {
    SCOPED_TRACE(c.layout);
    std::shared_ptr<const SliceTransform> transform;
    ASSERT_OK(CreateLayout(c.layout, &transform));
    ASSERT_NE(transform, nullptr);
    ASSERT_TRUE(transform->InDomain(c.prefix));
    ASSERT_EQ(transform->Transform(c.prefix), Slice(c.prefix));
    ASSERT_TRUE(transform->SameResultWhenAppended(c.prefix));
    const std::string key = c.prefix + std::string("\x00\xff-tail", 7);
    ASSERT_EQ(transform->Transform(key), Slice(c.prefix));
    for (size_t length = 0; length < c.prefix.size(); ++length) {
      Slice short_key(c.prefix.data(), length);
      ASSERT_FALSE(transform->InDomain(short_key));
      ASSERT_FALSE(transform->SameResultWhenAppended(short_key));
    }
  }
}

TEST_F(SliceTransformTest, LayoutRejectsOverflowAndTruncation) {
  const std::pair<const char*, std::string> cases[] = {
      {"v32", std::string(5, '\x80')},
      {"v32", std::string("\xff\xff\xff\xff\x10", 5)},
      {"v64", std::string(10, '\x80')},
      {"v64", std::string(9, '\xff') + '\x02'},
      {"b32", std::string("\x04"
                          "abc",
                          4)},
      {"b32", std::string("\xff\xff\xff\xff\x0f", 5)},
      {"b32", std::string("\xff\xff\xff\xff\x10", 5)},
      {"f4,v32", "abc"},
  };
  for (const auto& c : cases) {
    std::shared_ptr<const SliceTransform> transform;
    ASSERT_OK(CreateLayout(c.first, &transform));
    ASSERT_FALSE(transform->InDomain(c.second));
  }
}

TEST_F(SliceTransformTest, LayoutIdentityAndOptionsRoundTrip) {
  std::shared_ptr<const SliceTransform> first, second;
  ASSERT_OK(CreateLayout("v32,b32", &first));
  ASSERT_OK(CreateLayout("v32,v64", &second));
  ASSERT_NE(first->AsString(), second->AsString());
  size_t length = 0;
  ASSERT_FALSE(first->FullLengthEnabled(&length));
  ConfigOptions config;
  config.ignore_unsupported_options = false;
  ASSERT_OK(
      SliceTransform::CreateFromString(config, first->AsString(), &second));
  ASSERT_EQ(first->AsString(), second->AsString());
  ColumnFamilyOptions options;
  ASSERT_OK(GetColumnFamilyOptionsFromString(
      config, ColumnFamilyOptions(), "prefix_extractor=" + first->AsString(),
      &options));
  ASSERT_EQ(options.prefix_extractor->AsString(), first->AsString());
  ASSERT_OK(CreateLayout("f1,f2,v32,f4,f8", &first));
  ASSERT_EQ(first->AsString(), "rocksdb.LayoutPrefix.v1:f1,f2,v32,f4,f8");
  ASSERT_OK(
      SliceTransform::CreateFromString(config, first->AsString(), &second));
  ASSERT_EQ(first->AsString(), second->AsString());
  ASSERT_OK(CreateLayout("f2,f3", &first));
  ASSERT_EQ(first->AsString(), "rocksdb.FixedPrefix.5");
  ASSERT_TRUE(first->FullLengthEnabled(&length));
  ASSERT_EQ(length, 5U);
  const char* invalid[] = {
      "",     "f0",      "f-1", "f2147483648", "f99999999999999999999",
      "f",    "v31",     "v65", "b64",         ",v32",
      "v32,", "v32,,b32"};
  for (const char* text : invalid) {
    ASSERT_NOK(CreateLayout(text, &first));
  }
  ASSERT_NOK(SliceTransform::CreateFromString(
      config, "rocksdb.LayoutPrefix.v2:v32", &first));
}

TEST_F(SliceTransformTest, LayoutRandomizedSuffixAndOrdering) {
  const int seed = test::RandomSeed();
  SCOPED_TRACE("seed=" + std::to_string(seed));
  Random random(seed);
  std::shared_ptr<const SliceTransform> transform;
  ASSERT_OK(CreateLayout("v32,b32,v64,f3", &transform));
  std::vector<std::pair<std::string, std::string>> entries;
  for (int i = 0; i < 1000; ++i) {
    std::string payload;
    for (uint32_t n = random.Uniform(256); n > 0; --n) {
      payload += char(random.Uniform(256));
    }
    std::string prefix = EncodedPrefix(random.Next(), payload);
    PutVarint64(&prefix, (uint64_t{random.Next()} << 32) | random.Next());
    prefix.append(3, char(random.Uniform(256)));
    std::string key = prefix;
    for (uint32_t n = random.Uniform(32); n > 0; --n) {
      key += char(random.Uniform(256));
    }
    ASSERT_TRUE(transform->InDomain(key));
    ASSERT_EQ(transform->Transform(key), Slice(prefix));
    ASSERT_EQ(transform->Transform(transform->Transform(key)), Slice(prefix));
    entries.emplace_back(key, prefix);
  }
  std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
    return Slice(a.first).compare(b.first) < 0;
  });
  for (size_t i = 1; i < entries.size(); ++i) {
    ASSERT_LE(Slice(entries[i - 1].second).compare(entries[i].second), 0);
  }
  // Arbitrary byte inputs also exercise malformed and incomplete components.
  for (int i = 0; i < 10000; ++i) {
    std::string key;
    for (uint32_t n = random.Uniform(64); n > 0; --n) {
      key += char(random.Uniform(256));
    }
    if (transform->InDomain(key)) {
      const std::string prefix = transform->Transform(key).ToString();
      ASSERT_TRUE(Slice(key).starts_with(prefix));
      ASSERT_TRUE(transform->InDomain(prefix));
      ASSERT_EQ(transform->Transform(key + "suffix"), Slice(prefix));
      ASSERT_EQ(transform->Transform(prefix), Slice(prefix));
    }
  }
}

class SliceTransformDBTest : public testing::Test {
 private:
  std::string dbname_;
  Env* env_;
  std::unique_ptr<DB> db_;

 public:
  SliceTransformDBTest() : env_(Env::Default()), db_(nullptr) {
    dbname_ = test::PerThreadDBPath("slice_transform_db_test");
    EXPECT_OK(DestroyDB(dbname_, last_options_));
  }

  ~SliceTransformDBTest() override {
    db_.reset();
    EXPECT_OK(DestroyDB(dbname_, last_options_));
  }

  DB* db() { return db_.get(); }

  // Return the current option configuration.
  Options* GetOptions() { return &last_options_; }

  void DestroyAndReopen() {
    // Destroy using last options
    Destroy();
    ASSERT_OK(TryReopen());
  }

  void Destroy() {
    db_.reset();
    ASSERT_OK(DestroyDB(dbname_, last_options_));
  }

  Status TryReopen() {
    db_.reset();
    last_options_.create_if_missing = true;

    return DB::Open(last_options_, dbname_, &db_);
  }

  std::vector<std::pair<std::string, std::string>> ReadPrefix(
      const std::string& query, bool prefix_mode,
      const Snapshot* snapshot = nullptr) {
    ReadOptions options;
    options.total_order_seek = true;
    options.prefix_same_as_start = prefix_mode;
    options.snapshot = snapshot;
    std::string upper = query;
    while (!upper.empty() && upper.back() == '\xff') {
      upper.pop_back();
    }
    Slice upper_slice;
    if (!upper.empty()) {
      ++upper.back();
      upper_slice = upper;
      options.iterate_upper_bound = &upper_slice;
    }
    std::vector<std::pair<std::string, std::string>> result;
    std::unique_ptr<Iterator> iterator(db()->NewIterator(options));
    for (iterator->Seek(query);
         iterator->Valid() && iterator->key().starts_with(query);
         iterator->Next()) {
      result.emplace_back(iterator->key().ToString(),
                          iterator->value().ToString());
    }
    EXPECT_OK(iterator->status());
    return result;
  }

  Options last_options_;
  std::unique_ptr<Env> custom_env_;
};

namespace {
uint64_t PopTicker(const Options& options, Tickers ticker_type) {
  return options.statistics->getAndResetTickerCount(ticker_type);
}
}  // namespace

TEST_F(SliceTransformDBTest, CapPrefix) {
  last_options_.prefix_extractor.reset(NewCappedPrefixTransform(8));
  last_options_.statistics = ROCKSDB_NAMESPACE::CreateDBStatistics();
  BlockBasedTableOptions bbto;
  bbto.filter_policy.reset(NewBloomFilterPolicy(10, false));
  bbto.whole_key_filtering = false;
  last_options_.table_factory.reset(NewBlockBasedTableFactory(bbto));
  ASSERT_OK(TryReopen());

  ReadOptions ro;
  FlushOptions fo;
  WriteOptions wo;

  ASSERT_OK(db()->Put(wo, "barbarbar", "foo"));
  ASSERT_OK(db()->Put(wo, "barbarbar2", "foo2"));
  ASSERT_OK(db()->Put(wo, "foo", "bar"));
  ASSERT_OK(db()->Put(wo, "foo3", "bar3"));
  ASSERT_OK(db()->Flush(fo));

  std::unique_ptr<Iterator> iter(db()->NewIterator(ro));

  iter->Seek("foo");
  ASSERT_OK(iter->status());
  ASSERT_TRUE(iter->Valid());
  ASSERT_EQ(iter->value().ToString(), "bar");
  EXPECT_EQ(PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTER_MATCH), 1U);
  EXPECT_EQ(PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTERED), 0U);

  iter->Seek("foo2");
  ASSERT_OK(iter->status());
  ASSERT_TRUE(!iter->Valid());
  EXPECT_EQ(PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTER_MATCH), 0U);
  EXPECT_EQ(PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTERED), 1U);

  iter->Seek("barbarbar");
  ASSERT_OK(iter->status());
  ASSERT_TRUE(iter->Valid());
  ASSERT_EQ(iter->value().ToString(), "foo");
  EXPECT_EQ(PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTER_MATCH), 1U);
  EXPECT_EQ(PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTERED), 0U);

  iter->Seek("barfoofoo");
  ASSERT_OK(iter->status());
  ASSERT_TRUE(!iter->Valid());
  EXPECT_EQ(PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTER_MATCH), 0U);
  EXPECT_EQ(PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTERED), 1U);

  iter->Seek("foobarbar");
  ASSERT_OK(iter->status());
  ASSERT_TRUE(!iter->Valid());
  EXPECT_EQ(PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTER_MATCH), 0U);
  EXPECT_EQ(PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTERED), 1U);
}

TEST_F(SliceTransformDBTest, LayoutMixedSstsSnapshotsAndReopen) {
  BlockBasedTableOptions table;
  table.filter_policy.reset(NewBloomFilterPolicy(10));
  table.whole_key_filtering = true;
  last_options_.table_factory.reset(NewBlockBasedTableFactory(table));
  last_options_.disable_auto_compactions = true;
  ASSERT_OK(TryReopen());
  const std::string a = EncodedPrefix(128, "a");
  const std::string b = EncodedPrefix(129, "b");
  const std::string c = EncodedPrefix(128, "c");
  WriteOptions wo;
  FlushOptions fo;
  fo.wait = true;
  ASSERT_OK(db()->Put(wo, a + "1", "old"));
  ASSERT_OK(db()->Put(wo, b + "1", "deleted"));
  ASSERT_OK(db()->Flush(fo));
  ASSERT_OK(CreateLayout("v32,b32", &last_options_.prefix_extractor));
  ASSERT_OK(TryReopen());
  ASSERT_OK(db()->Put(wo, a + "2", "new"));
  ASSERT_OK(db()->Put(wo, c + "1", "other"));
  ASSERT_OK(db()->Flush(fo));
  DBOptions stored_db_options;
  std::vector<ColumnFamilyDescriptor> stored_columns;
  ConfigOptions config;
  config.ignore_unknown_options = false;
  config.ignore_unsupported_options = false;
  ASSERT_OK(LoadLatestOptions(config, db()->GetName(), &stored_db_options,
                              &stored_columns));
  ASSERT_EQ(stored_columns.size(), 1U);
  ASSERT_EQ(stored_columns[0].options.prefix_extractor->AsString(),
            last_options_.prefix_extractor->AsString());
  const std::vector<std::string> queries = {"",
                                            std::string("\x80", 1),
                                            a,
                                            a + "1",
                                            b,
                                            c,
                                            EncodedPrefix(130, "missing"),
                                            std::string(5, '\xff')};
  auto verify = [&](const Snapshot* snapshot = nullptr) {
    for (const auto& query : queries) {
      ASSERT_EQ(ReadPrefix(query, false, snapshot),
                ReadPrefix(query, true, snapshot));
    }
  };
  verify();
  ASSERT_EQ(ReadPrefix(a, true).size(), 2U);
  const Snapshot* snapshot = db()->GetSnapshot();
  ASSERT_OK(db()->Put(wo, a + "1", "updated"));
  ASSERT_OK(db()->Delete(wo, b + "1"));
  ASSERT_OK(db()->Flush(fo));
  verify(snapshot);
  ASSERT_EQ(ReadPrefix(a + "1", true, snapshot).front().second, "old");
  db()->ReleaseSnapshot(snapshot);
  verify();
  std::string value;
  ASSERT_OK(db()->Get(ReadOptions(), a + "1", &value));
  ASSERT_EQ(value, "updated");
  const std::string point_a = a + "1";
  const std::string point_b = b + "1";
  std::vector<Slice> point_keys = {point_a, point_b};
  std::vector<std::string> point_values;
  const auto statuses =
      db()->MultiGet(ReadOptions(), point_keys, &point_values);
  ASSERT_OK(statuses[0]);
  ASSERT_EQ(point_values[0], "updated");
  ASSERT_TRUE(statuses[1].IsNotFound());

  ASSERT_TRUE(db()->Get(ReadOptions(), b + "1", &value).IsNotFound());
  ASSERT_OK(db()->CompactRange(CompactRangeOptions(), nullptr, nullptr));
  verify();
  ASSERT_OK(TryReopen());
  verify();
  // A different extractor must not query the old Bloom with its own prefixes.
  ASSERT_OK(CreateLayout("v32", &last_options_.prefix_extractor));
  ASSERT_OK(TryReopen());
  verify();
  ASSERT_EQ(ReadPrefix(a, true).size(), 2U);
  last_options_.prefix_extractor.reset();
  ASSERT_OK(TryReopen());
  verify();
}

TEST_F(SliceTransformDBTest, LayoutConcurrentFlushPreservesSnapshot) {
  ASSERT_OK(CreateLayout("v32,b32", &last_options_.prefix_extractor));
  BlockBasedTableOptions table;
  table.filter_policy.reset(NewBloomFilterPolicy(10));
  last_options_.table_factory.reset(NewBlockBasedTableFactory(table));
  ASSERT_OK(TryReopen());
  WriteOptions wo;
  const std::string prefix = EncodedPrefix(128, "shared");
  for (int i = 0; i < 100; ++i) {
    ASSERT_OK(db()->Put(wo, prefix + std::to_string(i), "initial"));
  }
  const Snapshot* snapshot = db()->GetSnapshot();
  Status writer_status;
  std::thread writer([&] {
    FlushOptions flush;
    flush.wait = true;
    for (int i = 0; i < 500; ++i) {
      writer_status = db()->Put(wo, prefix + std::to_string(i), "updated");
      if (!writer_status.ok()) {
        return;
      }
      if (i % 50 == 0) {
        writer_status = db()->Flush(flush);
        if (!writer_status.ok()) {
          return;
        }
      }
    }
  });
  for (int i = 0; i < 100; ++i) {
    const auto values = ReadPrefix(prefix, true, snapshot);
    EXPECT_EQ(values.size(), 100U);
    for (const auto& entry : values) {
      EXPECT_EQ(entry.second, "initial");
    }
  }
  writer.join();
  db()->ReleaseSnapshot(snapshot);
  ASSERT_OK(writer_status);
  ASSERT_EQ(ReadPrefix(prefix, true).size(), 500U);
}

TEST_F(SliceTransformDBTest, LayoutAsyncRepeatedSeeksMatchTotalOrder) {
  auto fs = std::make_shared<DeferredReadFS>();
  custom_env_.reset(new CompositeEnvWrapper(Env::Default(), fs));
  last_options_.env = custom_env_.get();
  last_options_.compression = kNoCompression;
  last_options_.disable_auto_compactions = true;
  ASSERT_OK(CreateLayout("v32,b32", &last_options_.prefix_extractor));
  BlockBasedTableOptions table;
  table.block_size = 512;
  table.filter_policy.reset(NewBloomFilterPolicy(20));
  last_options_.table_factory.reset(NewBlockBasedTableFactory(table));
  ASSERT_OK(TryReopen());
  const std::string first = EncodedPrefix(1, std::string(128, 'a'));
  const std::string second = EncodedPrefix(2, std::string(192, 'b'));
  for (const auto& prefix : {first, second}) {
    for (int i = 0; i < 40; ++i) {
      ASSERT_OK(db()->Put(WriteOptions(), prefix + std::to_string(i),
                          std::string(256, 'v')));
    }
  }
  FlushOptions flush;
  flush.wait = true;
  ASSERT_OK(db()->Flush(flush));
  ReadOptions reference_options;
  reference_options.total_order_seek = true;
  reference_options.fill_cache = false;
  ReadOptions async_options = reference_options;
  async_options.async_io = true;
  async_options.prefix_same_as_start = true;
  async_options.readahead_size = 512;
  std::unique_ptr<Iterator> reference(db()->NewIterator(reference_options));
  std::unique_ptr<Iterator> iterator(db()->NewIterator(async_options));
  bool verified_async_seek = false;
  auto scan = [&](Iterator* iter, const std::string& query) {
    std::vector<std::pair<std::string, std::string>> result;
    const unsigned completed_before_seek = fs->completed_reads;
    iter->Seek(query);
    if (iter == iterator.get() && !verified_async_seek) {
      // Completion must happen during Seek, before Next can submit any reads.
      EXPECT_GT(fs->completed_reads, completed_before_seek);
      verified_async_seek = true;
    }
    for (; iter->Valid() && iter->key().starts_with(query); iter->Next()) {
      result.emplace_back(iter->key().ToString(), iter->value().ToString());
    }
    EXPECT_OK(iter->status());
    return result;
  };
  for (const auto& query : {first, second, EncodedPrefix(1, "missing"),
                            first + "1", std::string("\x80", 1), first}) {
    ASSERT_EQ(scan(reference.get(), query), scan(iterator.get(), query));
  }
  // This FS only invokes completion callbacks from Poll, so synchronous
  // fallback cannot satisfy this assertion.
  ASSERT_GT(fs->completed_reads, 0U);
}

TEST_F(SliceTransformDBTest, LayoutLongPrefixesAcrossRepeatedSeeks) {
  ASSERT_OK(CreateLayout("v32,b32", &last_options_.prefix_extractor));
  BlockBasedTableOptions table;
  table.block_size = 256;
  table.filter_policy.reset(NewBloomFilterPolicy(20));
  last_options_.table_factory.reset(NewBlockBasedTableFactory(table));
  ASSERT_OK(TryReopen());
  const std::string first = EncodedPrefix(1, std::string(256, 'a'));
  const std::string second = EncodedPrefix(2, std::string(384, 'b'));
  WriteOptions write;
  for (const auto& prefix : {first, second}) {
    ASSERT_OK(db()->Put(write, prefix + "0", "v"));
    ASSERT_OK(db()->Put(write, prefix + "1", "v"));
  }
  FlushOptions flush;
  flush.wait = true;
  ASSERT_OK(db()->Flush(flush));
  ReadOptions read;
  read.total_order_seek = true;
  read.prefix_same_as_start = true;
  std::unique_ptr<Iterator> iterator(db()->NewIterator(read));
  const std::pair<std::string, unsigned> queries[] = {
      {first, 2},
      {second, 2},
      {EncodedPrefix(1, "absent"), 0},
      {first, 2},
      {EncodedPrefix(1, ""), 0},
      {second, 2}};
  for (const auto& query : queries) {
    unsigned count = 0;
    for (iterator->Seek(query.first); iterator->Valid(); iterator->Next()) {
      ASSERT_TRUE(iterator->key().starts_with(query.first));
      ++count;
    }
    ASSERT_OK(iterator->status());
    ASSERT_EQ(count, query.second);
  }
}

TEST_F(SliceTransformDBTest, LayoutBloomRejectionAvoidsReadaheadExtraction) {
  std::shared_ptr<const SliceTransform> layout;
  ASSERT_OK(CreateLayout("v32,b32", &layout));
  auto counted = std::make_shared<CountingPrefixTransform>(layout);
  last_options_.prefix_extractor = counted;
  last_options_.statistics = CreateDBStatistics();
  last_options_.disable_auto_compactions = true;
  BlockBasedTableOptions table;
  table.filter_policy.reset(NewBloomFilterPolicy(20));
  last_options_.table_factory.reset(NewBlockBasedTableFactory(table));
  ASSERT_OK(TryReopen());
  WriteOptions write;
  FlushOptions flush;
  flush.wait = true;
  ASSERT_OK(db()->Put(write, EncodedPrefix(0, "low") + "0", "v"));
  ASSERT_OK(db()->Put(write, EncodedPrefix(127, "high") + "0", "v"));
  ASSERT_OK(db()->Flush(flush));
  bool rejected = false;
  for (uint32_t candidate = 1; candidate < 127; ++candidate) {
    counted->domain_calls = 0;
    counted->transform_calls = 0;
    PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTERED);
    ASSERT_TRUE(ReadPrefix(EncodedPrefix(candidate, "absent"), true).empty());
    if (PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTERED) > 0) {
      // A rejected SST only needs the filter's checked extraction.
      EXPECT_LE(counted->domain_calls.load(), 1U);
      EXPECT_LE(counted->transform_calls.load(), 1U);
      rejected = true;
      break;
    }
  }
  ASSERT_TRUE(rejected);
}

TEST_F(SliceTransformDBTest, LayoutBloomFiltersOverlappingSsts) {
  ASSERT_OK(CreateLayout("v32,b32", &last_options_.prefix_extractor));
  last_options_.statistics = CreateDBStatistics();
  last_options_.disable_auto_compactions = true;
  BlockBasedTableOptions table;
  table.filter_policy.reset(NewBloomFilterPolicy(10));
  table.whole_key_filtering = true;
  last_options_.table_factory.reset(NewBlockBasedTableFactory(table));
  ASSERT_OK(TryReopen());
  WriteOptions wo;
  FlushOptions fo;
  fo.wait = true;
  for (uint32_t file = 1; file <= 8; ++file) {
    ASSERT_OK(db()->Put(wo, EncodedPrefix(0, "low") + "0", "sentinel"));
    for (int row = 0; row < 32; ++row) {
      ASSERT_OK(db()->Put(
          wo, EncodedPrefix(file, "group") + std::to_string(row), "v"));
    }
    ASSERT_OK(db()->Put(wo, EncodedPrefix(127, "high") + "0", "sentinel"));
    ASSERT_OK(db()->Flush(fo));
  }
  PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTERED);
  ASSERT_EQ(ReadPrefix(EncodedPrefix(3, "group"), true).size(), 32U);
  ASSERT_TRUE(ReadPrefix(EncodedPrefix(99, "absent"), true).empty());
  // Bloom false positives are allowed; exact filtering counts are not a
  // contract.
  ASSERT_GT(PopTicker(last_options_, NON_LAST_LEVEL_SEEK_FILTERED), 0U);
  ASSERT_EQ(ReadPrefix(EncodedPrefix(3, "group"), false),
            ReadPrefix(EncodedPrefix(3, "group"), true));
}

}  // namespace ROCKSDB_NAMESPACE

int main(int argc, char** argv) {
  ROCKSDB_NAMESPACE::port::InstallStackTraceHandler();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
