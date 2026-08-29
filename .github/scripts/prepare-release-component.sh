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
  echo "Usage: $0 <version> <built-component> <draft-component-or-dash> <output>" >&2
  exit 2
fi

version=$1
built_component=$2
draft_component=$3
output_directory=$4
artifact="fluss-rocksdbjni-$version"
suffixes=(.jar -sources.jar -javadoc.jar .pom)

if [[ "$draft_component" == - ]]; then
  source_directory=$built_component
else
  source_directory=$draft_component
fi

valid_files=0
for suffix in "${suffixes[@]}"; do
  if [[ -f "$source_directory/$artifact$suffix" ]]; then
    valid_files=$((valid_files + 1))
  fi
done
file_count=$(find "$source_directory" -maxdepth 1 -type f | wc -l | tr -d ' ')
if [[ "$valid_files" -ne 4 || "$file_count" -ne 4 ]]; then
  echo "Release component must contain exactly the four expected primary files" >&2
  exit 1
fi
if [[ -e "$output_directory" ]]; then
  echo "Release component output already exists: $output_directory" >&2
  exit 1
fi

mkdir -p "$output_directory"
for suffix in "${suffixes[@]}"; do
  cp "$source_directory/$artifact$suffix" "$output_directory/"
done
