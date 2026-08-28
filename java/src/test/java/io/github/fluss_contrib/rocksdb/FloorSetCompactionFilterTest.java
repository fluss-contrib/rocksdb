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

package io.github.fluss_contrib.rocksdb;

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

  @Test
  public void treatsNullExplicitSetAsEmpty() throws RocksDBException {
    try (FloorSetCompactionFilter filter = new FloorSetCompactionFilter(TAG_OFFSET, 50, null);
         Options options = new Options().setCreateIfMissing(true).setCompactionFilter(filter);
         RocksDB db = RocksDB.open(options, dbFolder.getRoot().getAbsolutePath())) {
      put(db, "below-floor", 30);
      put(db, "above-floor", 100);

      db.compactRange();

      assertThat(db.get(bytes("below-floor"))).isNull();
      assertThat(db.get(bytes("above-floor"))).isNotNull();
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
