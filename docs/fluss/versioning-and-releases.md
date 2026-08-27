# Fluss RocksDB Versioning and Releases

The Fluss RocksDB JNI artifact is published with these Maven coordinates:

```text
io.github.fluss-contrib:fluss-rocksdbjni
```

The artifact version identifies its upstream RocksDB source and its Fluss
release state.

## Version formats

The upstream version is read from `include/rocksdb/version.h`. A separate copy
of that version is not maintained.

The current development Snapshot is:

```text
<rocksdb-version>-fluss-SNAPSHOT
```

A release candidate is:

```text
<rocksdb-version>-fluss-<revision>-rc<sequence>
```

A final release is:

```text
<rocksdb-version>-fluss-<revision>
```

Git tags add a leading `v`; Maven versions do not. For the RocksDB `11.8.1`
baseline, the corresponding examples are:

```text
Snapshot: 11.8.1-fluss-SNAPSHOT
RC tag:   v11.8.1-fluss-1-rc1
RC Maven: 11.8.1-fluss-1-rc1
Tag:      v11.8.1-fluss-1
Maven:    11.8.1-fluss-1
```

The Fluss revision starts at `1` for an upstream baseline and increases for an
immutable replacement release on the same baseline. RC sequences start at `1`
within a revision.

## Publication modes

### Pull-request preflight

An ordinary pull request runs the Linux x86_64 test workflow. A pull request
that changes publication-related files additionally exercises the complete
publication build without external side effects:

- build and smoke-test every supported platform;
- assemble the four-platform JNI JAR;
- generate the POM, sources JAR, and javadoc JAR;
- generate checksums;
- sign with an ephemeral test key and verify the signatures;
- validate the Maven repository layout and bundle contents; and
- retain the bundle as a GitHub Actions artifact.

The preflight receives neither Central credentials nor the production signing
key. It does not publish a Snapshot, create a tag, or create a GitHub Release.

### Development Snapshots

Snapshot publication is the only publication mode that runs directly from
`fluss-main`.

A scheduled job checks the current `fluss-main` head daily. It publishes only
when that head differs from the commit associated with the latest successful
deployment to the `maven-snapshots` GitHub Environment. A manual
`workflow_dispatch` run may republish the current head on demand.

Snapshots use the mutable `<rocksdb-version>-fluss-SNAPSHOT` version. They have
no Fluss revision, Git tag, or GitHub Release. Consumers add the Central Portal
Snapshot repository explicitly:

```xml
<repository>
  <id>central-portal-snapshots</id>
  <url>https://central.sonatype.com/repository/maven-snapshots/</url>
  <releases>
    <enabled>false</enabled>
  </releases>
  <snapshots>
    <enabled>true</enabled>
  </snapshots>
</repository>
```

Snapshots are temporary and expire according to the Central Portal retention
policy.

### Release candidates and final releases

An RC or final release starts from a Git tag matching the formats above on
`fluss-release-X.Y`, where `X.Y` comes from the tag's RocksDB version. The
release branch starts from the selected upstream RocksDB tag `vX.Y.Z`, then
replays or adapts the applicable Fluss changes from `fluss-main`. RC and final
release tags are never published directly from `fluss-main`.

The workflow rejects a tag when:

- the RocksDB portion does not match `include/rocksdb/version.h`;
- the Fluss revision or RC sequence is not a positive integer;
- the tag target is contained in `origin/fluss-main`;
- the tag target is not contained in `origin/fluss-release-X.Y`;
- the upstream RocksDB tag `vX.Y.Z` is not an ancestor of the release tag
  commit; or
- the corresponding immutable Maven version already exists with different
  content.

RC and final releases follow the same build, verification, signing, and
publication pipeline. An RC's GitHub Release is marked as a prerelease. An RC
remains optional; a final release may be published directly.

## Supported artifacts

The main Maven JAR contains Java 8-compatible `org.fluss.rocksdb` classes and
these native libraries:

```text
libflussrocksdbjni-linux64.so
libflussrocksdbjni-linux-aarch64.so
libflussrocksdbjni-osx-x86_64.jnilib
libflussrocksdbjni-osx-arm64.jnilib
```

The build matrix verifies Linux x86_64, Linux arm64, macOS x86_64, and macOS
arm64 independently before assembling the main JAR. Each platform job checks
the native architecture, supported operating-system floor, unexpected dynamic
dependencies, Java namespace, native-library name, JNI loading, and a basic
database smoke test.

The Maven component also contains:

- `fluss-rocksdbjni-<version>.pom`;
- `fluss-rocksdbjni-<version>-sources.jar`;
- `fluss-rocksdbjni-<version>-javadoc.jar`;
- PGP signatures; and
- required checksums.

The POM identifies `fluss-contrib/rocksdb` as the project and SCM source while
retaining the applicable RocksDB licenses and upstream attribution.

## Credentials and environments

Snapshot publication uses the `maven-snapshots` GitHub Environment, restricted
to `fluss-main` without a manual approval gate.

RC and final publication use the `maven-central` GitHub Environment, restricted
to Fluss release tags and protected by a required reviewer.

Each environment supplies these secrets only to its publication job:

```text
CENTRAL_USERNAME
CENTRAL_TOKEN
MAVEN_GPG_KEY
MAVEN_GPG_PASSPHRASE
```

Build, test, assembly, and pull-request jobs do not reference a publishing
environment and cannot access its secrets.

## RC and final publication order

1. Validate the tag and version, reject a commit contained in
   `origin/fluss-main`, require it on the matching `origin/fluss-release-X.Y`,
   and verify ancestry from upstream tag `vX.Y.Z`.
2. Build and smoke-test every supported platform.
3. Assemble and verify the four-platform JNI JAR.
4. Generate and verify the complete signed Maven component.
5. Create a draft GitHub Release and attach the release files.
6. Upload the Maven bundle as a user-managed Central deployment.
7. Wait for Central validation, request publication, and wait for `PUBLISHED`.
8. Publish the draft GitHub Release.

The public GitHub Release must not precede Maven Central availability.

If Central validation fails, the workflow drops the deployment and leaves no
public GitHub Release. If Central has published successfully but publishing the
GitHub Release fails, a rerun compares the locally rebuilt POM and JARs with the
published component. Identical content skips the immutable Central upload and
resumes GitHub Release publication; different content fails without modifying
either publication.

## Immutability

RC and final Maven versions, Git tags, and published GitHub Release assets are
immutable. A defect is corrected with the next RC sequence or Fluss revision.
Snapshots are the only mutable publication channel.
