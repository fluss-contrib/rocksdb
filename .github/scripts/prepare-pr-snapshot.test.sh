#!/usr/bin/env bash

# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

set -euo pipefail

script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
script="$script_dir/prepare-pr-snapshot.sh"
temporary_directory=$(mktemp -d)
trap 'rm -rf "$temporary_directory"' EXIT

head_sha=0123456789abcdef0123456789abcdef01234567

write_pull_request() {
  local output_file=$1
  local state=${2:-open}
  local base_ref=${3:-fluss-main}
  local source_repository=${4-contributor/rocksdb}

  jq -n \
    --argjson number 42 \
    --arg state "$state" \
    --arg base_ref "$base_ref" \
    --arg source_repository "$source_repository" \
    --arg head_sha "$head_sha" \
    '{
      number: $number,
      state: $state,
      base: {
        ref: $base_ref,
        repo: {full_name: "fluss-contrib/rocksdb"}
      },
      head: {
        sha: $head_sha,
        repo: (
          if $source_repository == "" then null
          else {full_name: $source_repository}
          end
        )
      }
    }' > "$output_file"
}

run_prepare() {
  local pr_json=$1
  local output_file=$2
  local manifest_file=$3
  local event_name=${4:-workflow_dispatch}
  local git_ref=${5:-refs/heads/fluss-main}
  local requested_pr_number=${6:-42}

  GITHUB_EVENT_NAME=$event_name \
    GITHUB_REF=$git_ref \
    GITHUB_REPOSITORY=fluss-contrib/rocksdb \
    REQUESTED_PR_NUMBER=$requested_pr_number \
    "$script" "$pr_json" 11.8.1 "$output_file" "$manifest_file"
}

assert_failure() {
  local expected_message=$1
  shift
  local error_file="$temporary_directory/error"

  if "$@" > /dev/null 2> "$error_file"; then
    echo "Expected command to fail: $*" >&2
    exit 1
  fi
  grep -Fqx "$expected_message" "$error_file"
}

valid_pr="$temporary_directory/valid-pr.json"
outputs="$temporary_directory/outputs"
manifest="$temporary_directory/manifest.json"
write_pull_request "$valid_pr"
run_prepare "$valid_pr" "$outputs" "$manifest"

cat > "$temporary_directory/expected-outputs" <<EOF
pr_number=42
source_repository=contributor/rocksdb
source_sha=$head_sha
rocksdb_version=11.8.1
version=11.8.1-fluss-pr42-0123456789ab-SNAPSHOT
EOF
diff -u "$temporary_directory/expected-outputs" "$outputs"

jq -e \
  --arg head_sha "$head_sha" \
  '. == {
    target_repository: "fluss-contrib/rocksdb",
    target_branch: "fluss-main",
    pull_request: 42,
    source_repository: "contributor/rocksdb",
    source_sha: $head_sha,
    rocksdb_version: "11.8.1",
    maven_version: "11.8.1-fluss-pr42-0123456789ab-SNAPSHOT"
  }' "$manifest" > /dev/null

closed_pr="$temporary_directory/closed-pr.json"
write_pull_request "$closed_pr" closed
assert_failure \
  "Pull request 42 is not open" \
  run_prepare "$closed_pr" "$outputs" "$manifest"

wrong_base_pr="$temporary_directory/wrong-base-pr.json"
write_pull_request "$wrong_base_pr" open main
assert_failure \
  "Pull request 42 must target fluss-contrib/rocksdb:fluss-main" \
  run_prepare "$wrong_base_pr" "$outputs" "$manifest"

missing_source_pr="$temporary_directory/missing-source-pr.json"
write_pull_request "$missing_source_pr" open fluss-main ""
assert_failure \
  "Pull request 42 source repository is unavailable" \
  run_prepare "$missing_source_pr" "$outputs" "$manifest"

assert_failure \
  "Manual PR Snapshot publication must run from fluss-main" \
  run_prepare "$valid_pr" "$outputs" "$manifest" \
    workflow_dispatch refs/heads/feature-branch

assert_failure \
  "Requested pull request 41 does not match API response 42" \
  run_prepare "$valid_pr" "$outputs" "$manifest" \
    pull_request refs/pull/42/merge 41

echo "PR Snapshot preparation tests passed"
