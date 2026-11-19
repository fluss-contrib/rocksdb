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

The `fluss-main` Nightly version is:

```text
<rocksdb-version>-fluss-nightly-SNAPSHOT
```

A pull-request Snapshot is:

```text
<rocksdb-version>-fluss-pr<pull-request>-<head-sha-12>-SNAPSHOT
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
Nightly: 11.8.1-fluss-nightly-SNAPSHOT
PR Snapshot: 11.8.1-fluss-pr42-a1b2c3d4e5f6-SNAPSHOT
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

Pull requests automatically run the Linux x86_64 tests and publication script
checks. To validate packaging across all supported platforms, a maintainer with
repository write access runs **Fluss PR Snapshot** from `fluss-main`, enters
the PR number and selects `preflight`. This mode accepts open PRs targeting
`fluss-main` or the matching `fluss-release-X.Y` branch and reads the RocksDB
baseline from the PR's base commit. It runs these checks:

- build and smoke-test every supported platform;
- assemble the four-platform JNI JAR;
- generate the POM, sources JAR, and javadoc JAR;
- generate checksums;
- sign with an ephemeral test key and verify the signatures;
- validate the Maven repository layout and bundle contents; and
- retain the bundle as a GitHub Actions artifact.

The preflight receives neither Central credentials nor the production signing
key. It does not publish a Snapshot, create a tag, or create a GitHub Release.

### Pull-request Snapshots

When a change needs a Maven dependency for cross-repository testing, a
maintainer with repository write access can publish a PR Snapshot:

1. Review the current head of the open PR targeting `fluss-main`.
2. In **Actions → Fluss PR Snapshot → Run workflow**, select `fluss-main`,
   enter the PR number, and use the default `pr-snapshot` mode.
3. Confirm the resolved head SHA in the run summary. After the publication
   job succeeds, use the Maven coordinates from that summary and verify
   dependency resolution via the [Snapshot repository](#consuming-maven-snapshots).

The workflow resolves the pull request through the GitHub API and locks its
source repository and complete head commit SHA. It rejects a pull request that
is closed, targets a repository or branch other than
`fluss-contrib/rocksdb:fluss-main`, has no accessible source repository, or no
longer has the locked head SHA when publication begins. It does not create a
temporary branch, Git tag, or GitHub Release.

The workflow definition comes from `fluss-main`. Build jobs check out the
locked PR head SHA with persisted checkout credentials disabled. The
verification job compares the retained manifest with the current PR before
the separate publication job signs and uploads the assembled component. The
publication job uses the `maven-snapshots` Environment and consumes the build
artifacts directly.

The version includes the target RocksDB version, pull request number, and the
first 12 lowercase hexadecimal characters of the locked head SHA:

```text
<rocksdb-version>-fluss-pr<pull-request>-<head-sha-12>-SNAPSHOT
```

The retained manifest records the target repository and branch, pull request
number, source repository, complete head SHA, RocksDB version, and Maven
version. A new pull-request head therefore produces a new Maven version.
Repeated publication of the same head remains a normal timestamped Maven
Snapshot deployment.

### Nightly builds

**Fluss JNI Publish** builds the Nightly directly from `fluss-main`. The
scheduled workflow checks the branch daily and skips building a commit that a
previous scheduled or manual Nightly run already published. The **Run workflow**
button on `fluss-main` republishes the current head on demand.

Nightlies use the mutable `<rocksdb-version>-fluss-nightly-SNAPSHOT` Maven
version. The Maven `-SNAPSHOT` suffix enables snapshot repository semantics.
Nightly and PR Snapshot coordinates are separate. Neither mode creates
a Git tag or GitHub Release.

### Consuming Maven Snapshots

Consumers of Nightly or PR Snapshot versions add the Central Portal Snapshot
repository explicitly:

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

The main Maven JAR contains Java 8-compatible `io.github.fluss_contrib.rocksdb` classes and
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

Nightly and PR Snapshot publication use the `maven-snapshots`
GitHub Environment, restricted to workflows running from `fluss-main` without
a separate manual approval gate. Starting the pull-request Snapshot workflow
is its single maintainer approval.

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
Nightly and PR Snapshot Maven versions are mutable.
