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
prepare_script="$script_dir/prepare-release-component.sh"
verify_script="$script_dir/verify-release-component.sh"
temporary_directory=$(mktemp -d)
trap 'rm -rf "$temporary_directory"' EXIT

version=11.8.1-fluss-4
artifact="fluss-rocksdbjni-$version"
suffixes=(.jar -sources.jar -javadoc.jar .pom)

write_component() {
  local directory=$1
  local marker=$2

  mkdir -p "$directory"
  for suffix in "${suffixes[@]}"; do
    printf '%s:%s\n' "$marker" "$suffix" > "$directory/$artifact$suffix"
  done
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

built_component="$temporary_directory/built"
draft_component="$temporary_directory/draft"
write_component "$built_component" rebuilt
write_component "$draft_component" draft

first_output="$temporary_directory/first-output"
"$prepare_script" "$version" "$built_component" - "$first_output"
for suffix in "${suffixes[@]}"; do
  cmp "$built_component/$artifact$suffix" "$first_output/$artifact$suffix"
done

recovery_output="$temporary_directory/recovery-output"
"$prepare_script" "$version" "$built_component" "$draft_component" "$recovery_output"
for suffix in "${suffixes[@]}"; do
  cmp "$draft_component/$artifact$suffix" "$recovery_output/$artifact$suffix"
done

incomplete_draft="$temporary_directory/incomplete-draft"
write_component "$incomplete_draft" incomplete
rm "$incomplete_draft/$artifact-javadoc.jar"
assert_failure \
  "Release component must contain exactly the four expected primary files" \
  "$prepare_script" "$version" "$built_component" "$incomplete_draft" \
    "$temporary_directory/incomplete-output"

unexpected_draft="$temporary_directory/unexpected-draft"
write_component "$unexpected_draft" unexpected
printf 'unexpected\n' > "$unexpected_draft/checksums.txt"
assert_failure \
  "Release component must contain exactly the four expected primary files" \
  "$prepare_script" "$version" "$built_component" "$unexpected_draft" \
    "$temporary_directory/unexpected-output"

published_component="$temporary_directory/published"
write_component "$published_component" draft
"$verify_script" "$version" "$draft_component" "$published_component"
printf 'different\n' > "$published_component/$artifact.jar"
assert_failure \
  "Published component differs: $artifact.jar" \
  "$verify_script" "$version" "$draft_component" "$published_component"

echo "Release component recovery tests passed"
