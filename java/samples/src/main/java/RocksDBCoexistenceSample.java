// Copyright (c) Meta Platforms, Inc. and affiliates.
// All rights reserved.
// This source code is licensed under both the GPLv2 (found in the
// COPYING file in the root directory) and Apache 2.0 License
// (found in the LICENSE.Apache file in the root directory).

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;

public final class RocksDBCoexistenceSample {
  private RocksDBCoexistenceSample() {}

  public static void main(final String[] args) throws Exception {
    if (args.length != 2) {
      throw new IllegalArgumentException(
          "Usage: RocksDBCoexistenceSample <fluss-first|flink-first> <work-directory>");
    }

    final Path workDirectory = Paths.get(args[1]);
    Files.createDirectories(workDirectory);

    switch (args[0]) {
      case "fluss-first":
        org.fluss.rocksdb.RocksDB.loadLibrary();
        org.rocksdb.RocksDB.loadLibrary();
        exerciseFluss(workDirectory.resolve("fluss-1"));
        exerciseFlink(workDirectory.resolve("flink-1"));
        exerciseFluss(workDirectory.resolve("fluss-2"));
        break;
      case "flink-first":
        org.rocksdb.RocksDB.loadLibrary();
        org.fluss.rocksdb.RocksDB.loadLibrary();
        exerciseFlink(workDirectory.resolve("flink-1"));
        exerciseFluss(workDirectory.resolve("fluss-1"));
        exerciseFlink(workDirectory.resolve("flink-2"));
        break;
      default:
        throw new IllegalArgumentException("Unknown load order: " + args[0]);
    }
  }

  private static void exerciseFluss(final Path databasePath) throws Exception {
    final byte[] batchKey = bytes("fluss-batch-key");
    final byte[] batchValue = bytes("fluss-batch-value");
    try (org.fluss.rocksdb.Options options =
             new org.fluss.rocksdb.Options().setCreateIfMissing(true);
        org.fluss.rocksdb.RocksDB database =
            org.fluss.rocksdb.RocksDB.open(options, databasePath.toString());
        org.fluss.rocksdb.WriteOptions writeOptions = new org.fluss.rocksdb.WriteOptions();
        org.fluss.rocksdb.ReadOptions readOptions = new org.fluss.rocksdb.ReadOptions();
        org.fluss.rocksdb.WriteBatch batch = new org.fluss.rocksdb.WriteBatch()) {
      batch.put(batchKey, batchValue);
      database.write(writeOptions, batch);
      require(Arrays.equals(batchValue, database.get(batchKey)), "Fluss WriteBatch read failed");

      final byte[] directKey = bytes("fluss-direct-key");
      final byte[] directValue = bytes("fluss-direct-value");
      database.put(writeOptions, directBuffer(directKey), directBuffer(directValue));
      final ByteBuffer output = ByteBuffer.allocateDirect(64);
      final int length = database.get(readOptions, directBuffer(directKey), output);
      requireDirectValue(length, directValue, output, "Fluss");
    }
    requireFlussException(databasePath.resolve("missing"));
  }

  private static void exerciseFlink(final Path databasePath) throws Exception {
    final byte[] batchKey = bytes("flink-batch-key");
    final byte[] batchValue = bytes("flink-batch-value");
    try (org.rocksdb.Options options = new org.rocksdb.Options().setCreateIfMissing(true);
        org.rocksdb.RocksDB database = org.rocksdb.RocksDB.open(options, databasePath.toString());
        org.rocksdb.WriteOptions writeOptions = new org.rocksdb.WriteOptions();
        org.rocksdb.ReadOptions readOptions = new org.rocksdb.ReadOptions();
        org.rocksdb.WriteBatch batch = new org.rocksdb.WriteBatch()) {
      batch.put(batchKey, batchValue);
      database.write(writeOptions, batch);
      require(Arrays.equals(batchValue, database.get(batchKey)), "Flink WriteBatch read failed");

      final byte[] directKey = bytes("flink-direct-key");
      final byte[] directValue = bytes("flink-direct-value");
      database.put(writeOptions, directBuffer(directKey), directBuffer(directValue));
      final ByteBuffer output = ByteBuffer.allocateDirect(64);
      final int length = database.get(readOptions, directBuffer(directKey), output);
      requireDirectValue(length, directValue, output, "Flink");
    }
    requireFlinkException(databasePath.resolve("missing"));
  }

  private static void requireDirectValue(
      final int length, final byte[] expected, final ByteBuffer output, final String binding) {
    require(length == expected.length, binding + " direct ByteBuffer length mismatch");
    final byte[] actual = new byte[length];
    output.get(actual);
    require(Arrays.equals(expected, actual), binding + " direct ByteBuffer read failed");
  }

  private static void requireFlussException(final Path path) throws Exception {
    try (org.fluss.rocksdb.Options options = new org.fluss.rocksdb.Options()) {
      org.fluss.rocksdb.RocksDB.open(options, path.toString());
      throw new AssertionError("Fluss unexpectedly opened a missing database");
    } catch (final org.fluss.rocksdb.RocksDBException expected) {
      require(expected.getStatus() != null, "Fluss exception status was not initialized");
    }
  }

  private static void requireFlinkException(final Path path) throws Exception {
    try (org.rocksdb.Options options = new org.rocksdb.Options()) {
      org.rocksdb.RocksDB.open(options, path.toString());
      throw new AssertionError("Flink unexpectedly opened a missing database");
    } catch (final org.rocksdb.RocksDBException expected) {
      require(expected.getStatus() != null, "Flink exception status was not initialized");
    }
  }

  private static ByteBuffer directBuffer(final byte[] value) {
    final ByteBuffer buffer = ByteBuffer.allocateDirect(value.length);
    buffer.put(value);
    buffer.flip();
    return buffer;
  }

  private static byte[] bytes(final String value) {
    return value.getBytes(StandardCharsets.UTF_8);
  }

  private static void require(final boolean condition, final String message) {
    if (!condition) {
      throw new AssertionError(message);
    }
  }
}
