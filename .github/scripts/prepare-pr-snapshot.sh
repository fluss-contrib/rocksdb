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

if [[ $# -ne 4 ]]; then
  echo "Usage: $0 <pull-request-json> <rocksdb-version> <output-file> <manifest-file>" >&2
  exit 2
fi

pull_request_json=$1
rocksdb_version=$2
output_file=$3
manifest_file=$4

: "${GITHUB_EVENT_NAME:?GITHUB_EVENT_NAME is required}"
: "${GITHUB_REF:?GITHUB_REF is required}"
: "${GITHUB_REPOSITORY:?GITHUB_REPOSITORY is required}"
: "${REQUESTED_PR_NUMBER:?REQUESTED_PR_NUMBER is required}"

case "$GITHUB_EVENT_NAME" in
  workflow_dispatch)
    if [[ "$GITHUB_REF" != refs/heads/fluss-main ]]; then
      echo "Manual PR Snapshot publication must run from fluss-main" >&2
      exit 1
    fi
    ;;
  pull_request)
    ;;
  *)
    echo "Unsupported PR Snapshot event: $GITHUB_EVENT_NAME" >&2
    exit 1
    ;;
esac

if [[ ! "$REQUESTED_PR_NUMBER" =~ ^[1-9][0-9]*$ ]]; then
  echo "Invalid pull request number: $REQUESTED_PR_NUMBER" >&2
  exit 1
fi
if [[ ! "$rocksdb_version" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
  echo "Invalid RocksDB version: $rocksdb_version" >&2
  exit 1
fi

api_pr_number=$(jq -er '.number' "$pull_request_json")
if [[ "$api_pr_number" != "$REQUESTED_PR_NUMBER" ]]; then
  echo "Requested pull request $REQUESTED_PR_NUMBER does not match API response $api_pr_number" >&2
  exit 1
fi

state=$(jq -er '.state' "$pull_request_json")
if [[ "$state" != open ]]; then
  echo "Pull request $REQUESTED_PR_NUMBER is not open" >&2
  exit 1
fi

base_repository=$(jq -er '.base.repo.full_name' "$pull_request_json")
base_ref=$(jq -er '.base.ref' "$pull_request_json")
if [[ "$base_repository" != "$GITHUB_REPOSITORY" || "$base_ref" != fluss-main ]]; then
  echo "Pull request $REQUESTED_PR_NUMBER must target $GITHUB_REPOSITORY:fluss-main" >&2
  exit 1
fi

source_repository=$(jq -r '.head.repo.full_name // empty' "$pull_request_json")
if [[ -z "$source_repository" ]]; then
  echo "Pull request $REQUESTED_PR_NUMBER source repository is unavailable" >&2
  exit 1
fi
if [[ ! "$source_repository" =~ ^[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+$ ]]; then
  echo "Pull request $REQUESTED_PR_NUMBER has invalid source repository $source_repository" >&2
  exit 1
fi

source_sha=$(jq -er '.head.sha' "$pull_request_json")
if [[ ! "$source_sha" =~ ^[0-9a-f]{40}$ ]]; then
  echo "Pull request $REQUESTED_PR_NUMBER has invalid source SHA $source_sha" >&2
  exit 1
fi

version="$rocksdb_version-fluss-pr$REQUESTED_PR_NUMBER-${source_sha:0:12}-SNAPSHOT"

{
  printf 'pr_number=%s\n' "$REQUESTED_PR_NUMBER"
  printf 'source_repository=%s\n' "$source_repository"
  printf 'source_sha=%s\n' "$source_sha"
  printf 'rocksdb_version=%s\n' "$rocksdb_version"
  printf 'version=%s\n' "$version"
} >> "$output_file"

jq -n \
  --arg target_repository "$GITHUB_REPOSITORY" \
  --arg source_repository "$source_repository" \
  --arg source_sha "$source_sha" \
  --arg rocksdb_version "$rocksdb_version" \
  --arg maven_version "$version" \
  --argjson pull_request "$REQUESTED_PR_NUMBER" \
  '{
    target_repository: $target_repository,
    target_branch: "fluss-main",
    pull_request: $pull_request,
    source_repository: $source_repository,
    source_sha: $source_sha,
    rocksdb_version: $rocksdb_version,
    maven_version: $maven_version
  }' > "$manifest_file"
