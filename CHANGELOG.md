# Changelog

This changelog summarizes engine, Java/JNI API, and runtime compatibility
changes in published Fluss releases. For changes inherited from RocksDB, see
the upstream [HISTORY.md](https://github.com/facebook/rocksdb/blob/main/HISTORY.md).

## [v11.8.1-fluss-3] - 2026-08-29

Based on upstream RocksDB
[v11.8.1](https://github.com/facebook/rocksdb/releases/tag/v11.8.1).

### Added

- Added the FloorSet compaction filter and its Java/JNI API
  ([#24](https://github.com/fluss-contrib/rocksdb/pull/24)).

### Changed

- Relocated the Java API and JNI entry points from `org.fluss.rocksdb` to
  `io.github.fluss_contrib.rocksdb`
  ([#27](https://github.com/fluss-contrib/rocksdb/pull/27)).

## [v11.8.1-fluss-2] - 2026-08-28

Based on upstream RocksDB
[v11.8.1](https://github.com/facebook/rocksdb/releases/tag/v11.8.1).

### Added

- Relocated the Java API and JNI entry points to `org.fluss.rocksdb` and gave
  the native library the Fluss-specific `flussrocksdbjni` identity so it can
  coexist with the upstream RocksDB JNI library
  ([#2](https://github.com/fluss-contrib/rocksdb/pull/2)).
- Hid non-JNI native symbols on all supported platforms to prevent symbol
  conflicts when both native libraries are loaded in one process
  ([#6](https://github.com/fluss-contrib/rocksdb/pull/6)).
- Added the Fluss TTL compaction filter for value and list state, including its
  Java/JNI API and failure-safe filtering behavior
  ([#14](https://github.com/fluss-contrib/rocksdb/pull/14)).

[v11.8.1-fluss-3]: https://github.com/fluss-contrib/rocksdb/releases/tag/v11.8.1-fluss-3
[v11.8.1-fluss-2]: https://github.com/fluss-contrib/rocksdb/releases/tag/v11.8.1-fluss-2
