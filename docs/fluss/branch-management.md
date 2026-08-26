# Fluss RocksDB Branch Management

This repository maintains the Fluss RocksDB fork as a clean, linear series of
Fluss changes on top of an explicit upstream RocksDB release.

## Repository branches

`main` mirrors `facebook/rocksdb/main`. It contains no Fluss-specific commits.
An immutable upstream RocksDB release tag is the baseline for each Fluss
release line.

`fluss-main` is the default branch and the current Fluss development and
Snapshot line. It is protected, requires pull requests for ordinary changes,
and requires linear history. RC and final release tags are never created
directly from `fluss-main`.

Feature branches are short-lived. Their names use lowercase letters, numbers,
hyphens, and dots. A feature branch may be rebased before integration.

Fluss release branches are named `fluss-release-X.Y`, where `X.Y` is the major
and minor series of the selected RocksDB release. This name is intentionally
distinct from the Facebook/RocksDB release-branch naming scheme. RC and final
releases for the series are built only from this Fluss release branch.

## Ordinary changes

Each change is driven by a GitHub issue with a coherent scope and explicit
acceptance criteria. Its pull request links the issue and includes verification
evidence.

Pull requests are squash-merged. The squash commit describes the resulting
change rather than the development process. Merge commits and GitHub rebase
merges are disabled. Ordinary feature and fix integration never force-pushes or
rewrites `fluss-main`.

## Preparing a release branch

1. Select the immutable Facebook/RocksDB release tag `vX.Y.Z` that will be the
   release baseline. Do not use an arbitrary `facebook/rocksdb/main` commit.
2. Create `fluss-release-X.Y` from that upstream tag.
3. Replay the applicable Fluss changes from `fluss-main` onto the release
   branch in their logical order. Adapt a change within its own commit when the
   selected RocksDB baseline requires compatibility work.
4. Review and validate the complete release branch, including the full
   supported JNI platform matrix.
5. Create RC or final tags only from commits contained in
   `origin/fluss-release-X.Y`. The selected upstream tag `vX.Y.Z` must be an
   ancestor of every such tag commit.

## Upstream release upgrades

An upstream upgrade is a controlled replacement of the `fluss-main` history. It
is the only operation allowed to rewrite that branch.

1. Select an immutable upstream RocksDB release tag. Do not use an arbitrary
   `facebook/rocksdb/main` commit.
2. Record the current `fluss-main` head and stop ordinary integration until the
   upgrade finishes.
3. Create an upgrade branch directly from the selected upstream release tag.
4. Replay the effective Fluss commits in their logical order. Resolve an
   upstream compatibility change inside the Fluss commit that requires it.
   Development fixups and obsolete intermediate states are not preserved.
5. Review the complete replay under a dedicated issue and pull request. Run the
   normal source checks and the full supported JNI platform matrix.
6. Confirm that every previously supported Fluss behavior is either present or
   intentionally removed by an independently approved change.
7. Record the expected old head and the reviewed new head. Temporarily grant the
   designated maintainer the minimum bypass required to update `fluss-main`.
8. Replace `fluss-main` with an exact `--force-with-lease` guard against the
   recorded old head. Never use an unguarded force push.
9. Restore branch protection immediately. Fetch the remote branch and verify
   its head, linear ancestry, upstream source version, and complete tree against
   the reviewed upgrade branch.
10. Resume ordinary integration only after the verification evidence is added
    to the upgrade issue.

Published release tags are immutable and remain attached to their original
histories. Rewriting `fluss-main` therefore does not rewrite a published
release. Fixes for an existing released series are developed on its
`fluss-release-X.Y` branch and published under a new immutable RC sequence or
Fluss revision.

## Fork synchronization

GitHub's generic **Sync fork** operation may be used only for the upstream
mirror branch `main`. It must never be used for `fluss-main`.

Preserve upstream release tags so every Fluss baseline remains traceable to its
authoritative RocksDB source.
