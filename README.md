# RocksDB Fork for Apache Fluss

> [!IMPORTANT]
> This repository is a community-maintained RocksDB fork for use with
> [Apache Fluss](https://fluss.apache.org/). It is not an official repository,
> release, or distribution of the Apache Software Foundation or the Apache
> Fluss project, nor is it an official distribution of the upstream RocksDB
> project. No endorsement by these projects or their governing organizations
> is implied.
>
> Apache, Apache Fluss, and Fluss are trademarks or registered trademarks of
> [The Apache Software Foundation](https://www.apache.org/) in the United States
> and/or other countries. All other marks mentioned may be trademarks or
> registered trademarks of their respective owners.

[![Fluss PR](https://github.com/fluss-contrib/rocksdb/actions/workflows/fluss-pr.yml/badge.svg?event=pull_request)](https://github.com/fluss-contrib/rocksdb/actions/workflows/fluss-pr.yml?query=event%3Apull_request)

## About this fork

This repository is maintained by the `fluss-contrib` community as a
release-based fork of [RocksDB](https://github.com/facebook/rocksdb). It
provides a reviewable home for the RocksDB Java and native changes needed by
the Apache Fluss ecosystem while preserving traceability to the authoritative
upstream source.

Each maintained line starts from an immutable upstream RocksDB release tag.
Fluss-specific changes are reviewed and maintained as a clean, linear series
on top of that baseline.

## Use from Maven Central

Add the Fluss RocksDB JNI artifact to your Maven project:

```xml
<dependency>
  <groupId>io.github.fluss-contrib</groupId>
  <artifactId>fluss-rocksdbjni</artifactId>
  <version>11.8.1-fluss-3</version>
</dependency>
```

## Releases

- [Changelog](CHANGELOG.md) summarizes engine, Java/JNI API, and runtime
  compatibility changes in each Fluss release.
- [GitHub Releases](https://github.com/fluss-contrib/rocksdb/releases) provides
  release artifacts and tags.
- [Maven Central](https://central.sonatype.com/artifact/io.github.fluss-contrib/fluss-rocksdbjni)
  provides the published JNI components.

## Repository workflow

| Branch | Purpose |
| --- | --- |
| `main` | Mirrors `facebook/rocksdb/main` without Fluss-specific commits. |
| `fluss-main` | Default Fluss development branch and the only source of Snapshot publications. |
| `fluss-release-X.Y` | Release line based on an upstream RocksDB tag; the only source of RC and final releases. |

Changes to this fork are driven by a GitHub issue and submitted through a pull
request. Ordinary pull requests are squash-merged, and `fluss-main` maintains a
linear history.

Read the repository policies before contributing:

- [Branch management](docs/fluss/branch-management.md) describes branch roles,
  pull-request integration, release branches, and upstream upgrades.
- [Versioning and releases](docs/fluss/versioning-and-releases.md) describes
  artifact versions, supported platforms, and Snapshot, RC, and final
  publication.

## Upstream RocksDB

RocksDB is a persistent key-value store based on a Log-Structured Merge (LSM)
design. It supports flexible tradeoffs among write, read, and space
amplification and uses multithreaded compaction for large data sets.

For general RocksDB documentation, examples, and upstream development, see:

- [upstream RocksDB repository](https://github.com/facebook/rocksdb);
- [RocksDB examples](https://github.com/facebook/rocksdb/tree/main/examples);
  and
- [RocksDB wiki](https://github.com/facebook/rocksdb/wiki).

Report issues caused by Fluss-specific changes in this repository. General
RocksDB issues should be reported to the upstream RocksDB project.

## License

RocksDB is dual-licensed under both the GPLv2 (found in the `COPYING` file in
the root directory) and Apache License 2.0 (found in the `LICENSE.Apache` file
in the root directory). You may select, at your option, one of these licenses.
