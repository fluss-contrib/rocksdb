// Copyright (c) Meta Platforms, Inc. and affiliates.
// This source code is licensed under both the GPLv2 (found in the
// COPYING file in the root directory) and Apache 2.0 License
// (found in the LICENSE.Apache file in the root directory).
package io.github.fluss_contrib.rocksdb;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatThrownBy;

import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Properties;
import org.junit.ClassRule;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;

public class PrefixLayoutTest {
  @ClassRule
  public static final RocksNativeLibraryResource ROCKS_NATIVE_LIBRARY_RESOURCE =
      new RocksNativeLibraryResource();

  @Rule public TemporaryFolder folder = new TemporaryFolder();

  @Test
  public void rejectsInvalidLayouts() {
    assertThatThrownBy(() -> PrefixLayout.of()).isInstanceOf(IllegalArgumentException.class);
    assertThatThrownBy(() -> PrefixLayout.fixed(0)).isInstanceOf(IllegalArgumentException.class);
    assertThatThrownBy(() -> PrefixLayout.fixed(-1)).isInstanceOf(IllegalArgumentException.class);
    assertThatThrownBy(() -> PrefixLayout.of((PrefixLayout.Step[]) null))
        .isInstanceOf(NullPointerException.class);
    assertThatThrownBy(() -> PrefixLayout.of((PrefixLayout.Step) null))
        .isInstanceOf(NullPointerException.class);
    try (final Options options = new Options();
        final ColumnFamilyOptions cf = new ColumnFamilyOptions()) {
      assertThatThrownBy(() -> options.useLayoutPrefixExtractor(null))
          .isInstanceOf(NullPointerException.class);
      assertThatThrownBy(() -> cf.useLayoutPrefixExtractor(null))
          .isInstanceOf(NullPointerException.class);
    }
  }

  @Test
  public void optionsOwnsNativeLayoutAfterInstallation() throws Exception {
    final PrefixLayout.Step[] steps = {PrefixLayout.varint32(), PrefixLayout.bytesVarint32()};
    final PrefixLayout layout = PrefixLayout.of(steps);
    steps[0] = PrefixLayout.fixed(20);
    final Options options = new Options()
                                .setCreateIfMissing(true)
                                .setDisableAutoCompactions(true)
                                .useLayoutPrefixExtractor(layout);
    try (final RocksDB db = RocksDB.open(options, folder.getRoot().getAbsolutePath())) {
      // DB owns a copy of the configured extractor; the Java layout owns no handle.
      options.close();
      verifyReads(db);
    } finally {
      options.close();
    }
  }

  @Test
  public void columnFamilyOptionsEntryPoint() throws Exception {
    try (final DBOptions options = new DBOptions().setCreateIfMissing(true);
        final ColumnFamilyOptions cf = new ColumnFamilyOptions().useLayoutPrefixExtractor(
            PrefixLayout.of(PrefixLayout.varint32(), PrefixLayout.bytesVarint32()))) {
      final List<ColumnFamilyHandle> handles = new ArrayList<>();
      final List<ColumnFamilyDescriptor> descriptors =
          Collections.singletonList(new ColumnFamilyDescriptor(RocksDB.DEFAULT_COLUMN_FAMILY, cf));
      try (final RocksDB db =
               RocksDB.open(options, folder.getRoot().getAbsolutePath(), descriptors, handles)) {
        try {
          cf.close();
          verifyReads(db);
        } finally {
          for (final ColumnFamilyHandle handle : handles) {
            handle.close();
          }
        }
      }
    }
  }

  @Test
  public void optionsRegistryReconstructsLayout() throws Exception {
    final Properties properties = new Properties();
    properties.setProperty("prefix_extractor", "rocksdb.LayoutPrefix.v1:v32,b32");
    try (final ColumnFamilyOptions cf =
             ColumnFamilyOptions.getColumnFamilyOptionsFromProps(properties);
        final DBOptions options = new DBOptions().setCreateIfMissing(true)) {
      assertThat(cf).isNotNull();
      final List<ColumnFamilyHandle> handles = new ArrayList<>();
      try (final RocksDB db = RocksDB.open(options, folder.getRoot().getAbsolutePath(),
               Collections.singletonList(
                   new ColumnFamilyDescriptor(RocksDB.DEFAULT_COLUMN_FAMILY, cf)),
               handles)) {
        try {
          verifyReads(db);
        } finally {
          for (final ColumnFamilyHandle handle : handles) {
            handle.close();
          }
        }
      }
    }
  }

  @Test
  public void bloomFiltersAreUsedWithExplicitPrefixMode() throws Exception {
    try (final BloomFilter bloom = new BloomFilter(10);
        final Statistics statistics = new Statistics();
        final Options options = new Options()
            .setCreateIfMissing(true)
            .setDisableAutoCompactions(true)
            .setStatistics(statistics)
            .setTableFormatConfig(
                new BlockBasedTableConfig().setFilterPolicy(bloom).setWholeKeyFiltering(true))
            .useLayoutPrefixExtractor(
                PrefixLayout.of(PrefixLayout.varint32(), PrefixLayout.bytesVarint32()));
        final RocksDB db = RocksDB.open(options, folder.getRoot().getAbsolutePath());
        final FlushOptions flush = new FlushOptions().setWaitForFlush(true)) {
      for (int file = 1; file <= 8; ++file) {
        db.put(prefix(0, "low", "0"), new byte[] {0});
        db.put(prefix(file, "group", "0"), new byte[] {1});
        db.put(prefix(127, "high", "0"), new byte[] {2});
        db.flush(flush);
      }
      final long before = statistics.getTickerCount(TickerType.NON_LAST_LEVEL_SEEK_FILTERED);
      assertThat(read(db, prefix(3, "group", ""), true)).hasSize(1);
      assertThat(read(db, prefix(99, "absent", ""), true)).isEmpty();
      assertThat(statistics.getTickerCount(TickerType.NON_LAST_LEVEL_SEEK_FILTERED))
          .isGreaterThan(before);
      assertThat(read(db, prefix(3, "group", ""), true))
          .isEqualTo(read(db, prefix(3, "group", ""), false));
    }
  }

  @Test
  public void allFixedAndVarint64Layouts() throws Exception {
    final PrefixLayout[] layouts = {PrefixLayout.of(PrefixLayout.fixed(1), PrefixLayout.fixed(1)),
        PrefixLayout.of(PrefixLayout.varint64())};
    final byte[] key = {(byte) 0x80, 1, 42};
    for (final PrefixLayout layout : layouts) {
      try (final Options options =
               new Options().setCreateIfMissing(true).useLayoutPrefixExtractor(layout);
          final RocksDB db = RocksDB.open(options, folder.newFolder().getAbsolutePath())) {
        db.put(key, new byte[] {7});
        assertThat(read(db, Arrays.copyOf(key, 2), true)).hasSize(1);
        assertThat(read(db, new byte[] {(byte) 0x80}, true))
            .isEqualTo(read(db, new byte[] {(byte) 0x80}, false));
      }
    }
  }

  private static void verifyReads(final RocksDB db) throws RocksDBException {
    final byte[] a = prefix(128, "account", "");
    final byte[] a1 = prefix(128, "account", "1");
    final byte[] a2 = prefix(128, "account", "2");
    final byte[] b = prefix(128, "another", "1");
    db.put(a1, new byte[] {1});
    db.put(a2, new byte[] {2});
    db.put(b, new byte[] {3});
    final byte[][] queries = {
        {}, {(byte) 0x80}, a, a1, prefix(128, "", ""), prefix(129, "missing", "")};
    for (int state = 0; state < 3; ++state) {
      for (final byte[] query : queries) {
        assertThat(read(db, query, true)).isEqualTo(read(db, query, false));
      }
      assertThat(read(db, a, true)).hasSize(state < 2 ? 2 : 1);
      if (state == 0) {
        try (final FlushOptions flush = new FlushOptions().setWaitForFlush(true)) {
          db.flush(flush);
        }
      } else if (state == 1) {
        db.delete(a1);
        db.put(a2, new byte[] {4});
        db.compactRange();
      }
    }
    assertThat(db.get(a1)).isNull();
    assertThat(db.get(a2)).containsExactly((byte) 4);
  }

  private static List<String> read(final RocksDB db, final byte[] query, final boolean prefixMode) {
    int end = query.length;
    while (end > 0 && query[end - 1] == (byte) 0xff) {
      --end;
    }
    final byte[] upper = Arrays.copyOf(query, end);
    if (end > 0) {
      ++upper[end - 1];
    }
    try (final Slice upperSlice = new Slice(upper);
        final ReadOptions options =
            new ReadOptions().setTotalOrderSeek(true).setPrefixSameAsStart(prefixMode)) {
      if (end > 0) {
        options.setIterateUpperBound(upperSlice);
      }
      try (final RocksIterator iterator = db.newIterator(options)) {
        final List<String> result = new ArrayList<>();
        for (iterator.seek(query); iterator.isValid(); iterator.next()) {
          final byte[] key = iterator.key();
          if (key.length < query.length
              || !Arrays.equals(query, Arrays.copyOf(key, query.length))) {
            break;
          }
          result.add(Arrays.toString(key) + ":" + Arrays.toString(iterator.value()));
        }
        try {
          iterator.status();
        } catch (final RocksDBException exception) {
          throw new AssertionError(exception);
        }
        return result;
      }
    }
  }

  private static byte[] prefix(final int number, final String group, final String suffix) {
    final ByteArrayOutputStream bytes = new ByteArrayOutputStream();
    varint(bytes, number);
    final byte[] payload = group.getBytes(StandardCharsets.UTF_8);
    varint(bytes, payload.length);
    bytes.write(payload, 0, payload.length);
    final byte[] tail = suffix.getBytes(StandardCharsets.UTF_8);
    bytes.write(tail, 0, tail.length);
    return bytes.toByteArray();
  }

  private static void varint(final ByteArrayOutputStream out, int value) {
    while ((value & ~0x7f) != 0) {
      out.write((value & 0x7f) | 0x80);
      value >>>= 7;
    }
    out.write(value);
  }
}
