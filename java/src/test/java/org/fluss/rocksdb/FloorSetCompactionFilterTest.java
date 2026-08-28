//  Copyright (c) Meta Platforms, Inc. and affiliates.
// Copyright (c) 2024-present, platinumhamburg. All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

package org.fluss.rocksdb;

import static org.assertj.core.api.Assertions.assertThat;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;

public class FloorSetCompactionFilterTest {
  private static final int TAG_OFFSET = 2;

  static {
    RocksDB.loadLibrary();
  }

  @Rule public TemporaryFolder dbFolder = new TemporaryFolder();

  @Test
  public void filtersByFloorOrExplicitSet() throws RocksDBException {
    try (FloorSetCompactionFilter filter =
             new FloorSetCompactionFilter(TAG_OFFSET, 50, new long[] {100, 200});
        Options options = new Options().setCreateIfMissing(true).setCompactionFilter(filter);
        RocksDB db = RocksDB.open(options, dbFolder.getRoot().getAbsolutePath())) {
      put(db, "below-floor", 30);
      put(db, "at-floor", 50);
      put(db, "above-floor", 51);
      put(db, "in-set", 100);
      put(db, "kept", 300);

      db.compactRange();

      assertThat(db.get(bytes("below-floor"))).isNull();
      assertThat(db.get(bytes("at-floor"))).isNull();
      assertThat(db.get(bytes("above-floor"))).isNotNull();
      assertThat(db.get(bytes("in-set"))).isNull();
      assertThat(db.get(bytes("kept"))).isNotNull();
    }
  }

  @Test
  public void decodesSignedBigEndianTags() throws RocksDBException {
    try (FloorSetCompactionFilter filter =
             new FloorSetCompactionFilter(TAG_OFFSET, -1, new long[] {7});
        Options options = new Options().setCreateIfMissing(true).setCompactionFilter(filter);
        RocksDB db = RocksDB.open(options, dbFolder.getRoot().getAbsolutePath())) {
      put(db, "minimum", Long.MIN_VALUE);
      put(db, "negative", -1);
      put(db, "zero", 0);
      put(db, "explicit", 7);
      put(db, "positive", 8);

      db.compactRange();

      assertThat(db.get(bytes("minimum"))).isNull();
      assertThat(db.get(bytes("negative"))).isNull();
      assertThat(db.get(bytes("zero"))).isNotNull();
      assertThat(db.get(bytes("explicit"))).isNull();
      assertThat(db.get(bytes("positive"))).isNotNull();
    }
  }

  @Test
  public void keepsValuesWithoutACompleteTag() throws RocksDBException {
    try (FloorSetCompactionFilter filter =
             new FloorSetCompactionFilter(TAG_OFFSET, Long.MAX_VALUE, new long[0]);
        Options options = new Options().setCreateIfMissing(true).setCompactionFilter(filter);
        RocksDB db = RocksDB.open(options, dbFolder.getRoot().getAbsolutePath())) {
      for (int length = 0; length < TAG_OFFSET + Long.BYTES; length++) {
        db.put(bytes("short-" + length), new byte[length]);
      }
      put(db, "tag-only", 42, new byte[0]);

      db.compactRange();

      for (int length = 0; length < TAG_OFFSET + Long.BYTES; length++) {
        assertThat(db.get(bytes("short-" + length))).isNotNull();
      }
      assertThat(db.get(bytes("tag-only"))).isNull();
    }
  }

  private static void put(RocksDB db, String key, long tag) throws RocksDBException {
    put(db, key, tag, bytes("payload"));
  }

  private static void put(RocksDB db, String key, long tag, byte[] payload)
      throws RocksDBException {
    db.put(bytes(key), encodeValue(tag, payload));
  }

  private static byte[] encodeValue(long tag, byte[] payload) {
    return ByteBuffer.allocate(TAG_OFFSET + Long.BYTES + payload.length)
        .order(ByteOrder.LITTLE_ENDIAN)
        .putShort((short) 1)
        .order(ByteOrder.BIG_ENDIAN)
        .putLong(tag)
        .put(payload)
        .array();
  }

  private static byte[] bytes(String value) {
    return value.getBytes(StandardCharsets.UTF_8);
  }
}
