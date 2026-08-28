# Changelog

This changelog summarizes user-visible changes in published Fluss releases.
For changes inherited from RocksDB, see the upstream
[HISTORY.md](https://github.com/facebook/rocksdb/blob/main/HISTORY.md).

## [v11.8.1-fluss-2] - 2026-08-28

Based on upstream RocksDB
[v11.8.1](https://github.com/facebook/rocksdb/releases/tag/v11.8.1).

### Added

- Published the first Fluss RocksDB JNI release under the Maven coordinates
  `io.github.fluss-contrib:fluss-rocksdbjni:11.8.1-fluss-2`, with native
  libraries for Linux x86_64, Linux arm64, macOS x86_64, and macOS arm64
  ([#4](https://github.com/fluss-contrib/rocksdb/pull/4)).
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

### Fixed

- Corrected the repository context used to create and update GitHub Releases
  ([#16](https://github.com/fluss-contrib/rocksdb/pull/16)).

[v11.8.1-fluss-2]: https://github.com/fluss-contrib/rocksdb/releases/tag/v11.8.1-fluss-2
