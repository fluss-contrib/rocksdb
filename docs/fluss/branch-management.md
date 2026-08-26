# Fluss RocksDB Branch Management

This repository maintains the Fluss RocksDB fork as a clean, linear series of
Fluss changes on top of an explicit upstream RocksDB release.

## Repository branches

`main` mirrors `facebook/rocksdb/main`. It contains no Fluss-specific commits
and is never used as the base of a Fluss release without selecting an upstream
release tag first.

`fluss-main` is the default branch and the current Fluss development and release
line. It is protected, requires pull requests for ordinary changes, and requires
linear history.

Feature branches are short-lived. Their names use lowercase letters, numbers,
hyphens, and dots. A feature branch may be rebased before integration.

A `release-<rocksdb-version>` branch is created only when a previously released
line requires maintenance after `fluss-main` has moved to a newer upstream
release.

## Ordinary changes

Each change is driven by a GitHub issue with a coherent scope and explicit
acceptance criteria. Its pull request links the issue and includes verification
evidence.

Pull requests are squash-merged. The squash commit describes the resulting
change rather than the development process. Merge commits and GitHub rebase
merges are disabled. Ordinary feature and fix integration never force-pushes or
rewrites `fluss-main`.

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
release. If an old line still needs fixes, create its release branch from the
published tag before making those fixes.

## Fork synchronization

GitHub's generic **Sync fork** operation may be used only for the upstream
mirror branch `main`. It must never be used for `fluss-main`.

Preserve upstream release tags so every Fluss baseline remains traceable to its
authoritative RocksDB source.
