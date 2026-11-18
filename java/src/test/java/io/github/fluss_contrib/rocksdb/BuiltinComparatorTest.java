// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

package io.github.fluss_contrib.rocksdb;

import static io.github.fluss_contrib.rocksdb.util.TestUtil.u64Timestamp;
import static org.assertj.core.api.Assertions.assertThat;

import java.util.Collections;
import org.junit.ClassRule;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;

public class BuiltinComparatorTest {

  @ClassRule
  public static final RocksNativeLibraryResource ROCKS_NATIVE_LIBRARY_RESOURCE =
      new RocksNativeLibraryResource();

  @Rule
  public TemporaryFolder dbFolder = new TemporaryFolder();

  @Test
  public void timestampedWritesPreserveHistory() throws Exception {
    for (final BuiltinComparator comparator :
        new BuiltinComparator[] {BuiltinComparator.BYTEWISE_COMPARATOR_WITH_U64_TS,
            BuiltinComparator.REVERSE_BYTEWISE_COMPARATOR_WITH_U64_TS}) {
      try (final Options options = new Options().setCreateIfMissing(true).setComparator(comparator);
           final RocksDB db = RocksDB.open(options, dbFolder.newFolder().getAbsolutePath());
           final WriteOptions write = new WriteOptions()) {
        final byte[] key = new byte[] {1};
        db.put(write, key, u64Timestamp(3), new byte[] {3});
        db.put(db.getDefaultColumnFamily(), write, key, u64Timestamp(8), new byte[] {8});
        db.delete(write, key, u64Timestamp(15));
        db.put(write, key, u64Timestamp(20), new byte[] {20});
        final long[] times = {3, 5, 8, 12, 15, 17, 20, 22};
        final byte[][] expected = {{3}, {3}, {8}, {8}, null, null, {20}, {20}};
        for (int i = 0; i < times.length; i++) {
          try (final Slice ts = new Slice(u64Timestamp(times[i]));
               final ReadOptions read = new ReadOptions().setTimestamp(ts)) {
            assertThat(db.get(read, key)).isEqualTo(expected[i]);
            assertThat(db.multiGetAsList(read, Collections.singletonList(key)).get(0))
                .isEqualTo(expected[i]);
          }
        }
      }
    }
  }

  @Test
  public void namedColumnFamilySupportsTimestampedBatch() throws Exception {
    try (final Options options = new Options().setCreateIfMissing(true);
         final RocksDB db = RocksDB.open(options, dbFolder.getRoot().getAbsolutePath());
         final ColumnFamilyOptions cfOptions = new ColumnFamilyOptions().setComparator(
             BuiltinComparator.BYTEWISE_COMPARATOR_WITH_U64_TS);
         final ColumnFamilyHandle cf =
             db.createColumnFamily(new ColumnFamilyDescriptor(new byte[] {2}, cfOptions));
         final WriteOptions write = new WriteOptions(); final WriteBatch batch = new WriteBatch()) {
      final byte[] key = new byte[] {1};
      db.put(cf, write, key, u64Timestamp(3), new byte[] {3});
      batch.put(cf, key, u64Timestamp(8), new byte[] {8});
      batch.delete(cf, key, u64Timestamp(15));
      db.write(write, batch);
      try (final Slice ts = new Slice(u64Timestamp(12));
           final ReadOptions read = new ReadOptions().setTimestamp(ts)) {
        assertThat(db.get(cf, read, key)).containsExactly(8);
      }
      db.put(key, new byte[] {9});
      assertThat(db.get(key)).containsExactly(9);
    }
  }

  @Test
  public void timestampIteratorUsesUnsignedVersionsAndBounds() throws Exception {
    for (final BuiltinComparator comparator :
        new BuiltinComparator[] {BuiltinComparator.BYTEWISE_COMPARATOR_WITH_U64_TS,
            BuiltinComparator.REVERSE_BYTEWISE_COMPARATOR_WITH_U64_TS}) {
      final boolean reverse =
          comparator == BuiltinComparator.REVERSE_BYTEWISE_COMPARATOR_WITH_U64_TS;
      try (final Options options = new Options().setCreateIfMissing(true).setComparator(comparator);
           final RocksDB db = RocksDB.open(options, dbFolder.newFolder().getAbsolutePath());
           final WriteOptions write = new WriteOptions()) {
        for (byte key = 1; key <= 4; key++) {
          db.put(write, new byte[] {key}, u64Timestamp(3), new byte[] {3});
          db.put(write, new byte[] {key}, u64Timestamp(Long.MIN_VALUE), new byte[] {8});
        }
        db.delete(write, new byte[] {2}, u64Timestamp(Long.MIN_VALUE));
        final byte first = (byte) (reverse ? 4 : 1);
        try (
            final Slice ts = new Slice(u64Timestamp(Long.MIN_VALUE));
            final Slice lower = new Slice(new byte[] {first});
            final Slice upper = new Slice(new byte[] {(byte) (reverse ? 1 : 4)});
            final ReadOptions read =
                new ReadOptions().setTimestamp(ts).setIterateLowerBound(lower).setIterateUpperBound(
                    upper);
            final RocksIterator it = db.newIterator(read)) {
          it.seek(new byte[] {first});
          assertThat(it.key()).containsExactly(first);
          assertThat(it.value()).containsExactly(8);
          it.next();
          assertThat(it.key()).containsExactly(3);
          it.next();
          assertThat(it.isValid()).isFalse();
          it.seekToLast();
          assertThat(it.key()).containsExactly(3);
          it.prev();
          assertThat(it.key()).containsExactly(first);
          it.status();
        }
        try (final Slice ts = new Slice(u64Timestamp(Long.MAX_VALUE));
             final ReadOptions read = new ReadOptions().setTimestamp(ts)) {
          assertThat(db.get(read, new byte[] {1})).containsExactly(3);
        }
      }
    }
  }

  @Test
  public void timestampReadAlsoRespectsSnapshot() throws Exception {
    try (final Options options = new Options().setCreateIfMissing(true).setComparator(
             BuiltinComparator.BYTEWISE_COMPARATOR_WITH_U64_TS);
         final RocksDB db = RocksDB.open(options, dbFolder.getRoot().getAbsolutePath());
         final WriteOptions write = new WriteOptions()) {
      db.put(write, new byte[] {1}, u64Timestamp(3), new byte[] {3});
      final Snapshot snapshot = db.getSnapshot();
      try (final Slice ts = new Slice(u64Timestamp(8));
           final ReadOptions read = new ReadOptions().setTimestamp(ts).setSnapshot(snapshot)) {
        db.put(write, new byte[] {1}, u64Timestamp(8), new byte[] {8});
        assertThat(db.get(read, new byte[] {1})).containsExactly(3);
      } finally {
        db.releaseSnapshot(snapshot);
      }
    }
  }

  @Test
  public void builtinForwardComparator()
      throws RocksDBException {
    try (final Options options = new Options()
        .setCreateIfMissing(true)
        .setComparator(BuiltinComparator.BYTEWISE_COMPARATOR);
         final RocksDB rocksDb = RocksDB.open(options,
             dbFolder.getRoot().getAbsolutePath())
    ) {
      rocksDb.put("abc1".getBytes(), "abc1".getBytes());
      rocksDb.put("abc2".getBytes(), "abc2".getBytes());
      rocksDb.put("abc3".getBytes(), "abc3".getBytes());

      try(final RocksIterator rocksIterator = rocksDb.newIterator()) {
        // Iterate over keys using a iterator
        rocksIterator.seekToFirst();
        assertThat(rocksIterator.isValid()).isTrue();
        assertThat(rocksIterator.key()).isEqualTo(
            "abc1".getBytes());
        assertThat(rocksIterator.value()).isEqualTo(
            "abc1".getBytes());
        rocksIterator.next();
        assertThat(rocksIterator.isValid()).isTrue();
        assertThat(rocksIterator.key()).isEqualTo(
            "abc2".getBytes());
        assertThat(rocksIterator.value()).isEqualTo(
            "abc2".getBytes());
        rocksIterator.next();
        assertThat(rocksIterator.isValid()).isTrue();
        assertThat(rocksIterator.key()).isEqualTo(
            "abc3".getBytes());
        assertThat(rocksIterator.value()).isEqualTo(
            "abc3".getBytes());
        rocksIterator.next();
        assertThat(rocksIterator.isValid()).isFalse();
        // Get last one
        rocksIterator.seekToLast();
        assertThat(rocksIterator.isValid()).isTrue();
        assertThat(rocksIterator.key()).isEqualTo(
            "abc3".getBytes());
        assertThat(rocksIterator.value()).isEqualTo(
            "abc3".getBytes());
        // Seek for abc
        rocksIterator.seek("abc".getBytes());
        assertThat(rocksIterator.isValid()).isTrue();
        assertThat(rocksIterator.key()).isEqualTo(
            "abc1".getBytes());
        assertThat(rocksIterator.value()).isEqualTo(
            "abc1".getBytes());
      }
    }
  }

  @Test
  public void builtinReverseComparator()
      throws RocksDBException {
    try (final Options options = new Options()
        .setCreateIfMissing(true)
        .setComparator(BuiltinComparator.REVERSE_BYTEWISE_COMPARATOR);
         final RocksDB rocksDb = RocksDB.open(options,
             dbFolder.getRoot().getAbsolutePath())
    ) {

      rocksDb.put("abc1".getBytes(), "abc1".getBytes());
      rocksDb.put("abc2".getBytes(), "abc2".getBytes());
      rocksDb.put("abc3".getBytes(), "abc3".getBytes());

      try (final RocksIterator rocksIterator = rocksDb.newIterator()) {
        // Iterate over keys using a iterator
        rocksIterator.seekToFirst();
        assertThat(rocksIterator.isValid()).isTrue();
        assertThat(rocksIterator.key()).isEqualTo(
            "abc3".getBytes());
        assertThat(rocksIterator.value()).isEqualTo(
            "abc3".getBytes());
        rocksIterator.next();
        assertThat(rocksIterator.isValid()).isTrue();
        assertThat(rocksIterator.key()).isEqualTo(
            "abc2".getBytes());
        assertThat(rocksIterator.value()).isEqualTo(
            "abc2".getBytes());
        rocksIterator.next();
        assertThat(rocksIterator.isValid()).isTrue();
        assertThat(rocksIterator.key()).isEqualTo(
            "abc1".getBytes());
        assertThat(rocksIterator.value()).isEqualTo(
            "abc1".getBytes());
        rocksIterator.next();
        assertThat(rocksIterator.isValid()).isFalse();
        // Get last one
        rocksIterator.seekToLast();
        assertThat(rocksIterator.isValid()).isTrue();
        assertThat(rocksIterator.key()).isEqualTo(
            "abc1".getBytes());
        assertThat(rocksIterator.value()).isEqualTo(
            "abc1".getBytes());
        // Will be invalid because abc is after abc1
        rocksIterator.seek("abc".getBytes());
        assertThat(rocksIterator.isValid()).isFalse();
        // Will be abc3 because the next one after abc999
        // is abc3
        rocksIterator.seek("abc999".getBytes());
        assertThat(rocksIterator.key()).isEqualTo(
            "abc3".getBytes());
        assertThat(rocksIterator.value()).isEqualTo(
            "abc3".getBytes());
      }
    }
  }

  @Test
  public void builtinComparatorEnum(){
    assertThat(BuiltinComparator.BYTEWISE_COMPARATOR.ordinal())
        .isEqualTo(0);
    assertThat(
        BuiltinComparator.REVERSE_BYTEWISE_COMPARATOR.ordinal())
        .isEqualTo(1);
    assertThat(BuiltinComparator.values().length).isEqualTo(4);
    assertThat(BuiltinComparator.valueOf("BYTEWISE_COMPARATOR")).
        isEqualTo(BuiltinComparator.BYTEWISE_COMPARATOR);
  }
}
