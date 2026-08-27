#!/usr/bin/env bash

set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <Fluss RocksDB JNI library>" >&2
  exit 2
fi

library=$1
if [[ ! -f "$library" ]]; then
  echo "Native library not found: $library" >&2
  exit 2
fi

symbols_file=$(mktemp)
unexpected_file=$(mktemp)
trap 'rm -f "$symbols_file" "$unexpected_file"' EXIT

case "$(uname -s)" in
  Linux)
    readelf --dyn-syms --wide "$library" \
      | awk '$7 != "UND" && ($5 == "GLOBAL" || $5 == "WEAK" || $5 == "UNIQUE") && $6 == "DEFAULT" { sub(/@.*/, "", $8); print $8 }' \
      | sort -u > "$symbols_file"
    ;;
  Darwin)
    nm -gjU "$library" | sed 's/^_//' | sort -u > "$symbols_file"
    ;;
  *)
    echo "Unsupported host platform: $(uname -s)" >&2
    exit 2
    ;;
esac

awk '!/^Java_org_fluss_rocksdb_/ && $0 != "JNI_OnLoad" && $0 != "JNI_OnUnload"' \
  "$symbols_file" > "$unexpected_file"

if ! grep -q '^Java_org_fluss_rocksdb_' "$symbols_file"; then
  echo "No relocated Fluss JNI entry points are exported by $library" >&2
  exit 1
fi

if [[ -s "$unexpected_file" ]]; then
  echo "Unexpected native exports in $library:" >&2
  sed -n '1,50p' "$unexpected_file" >&2
  unexpected_count=$(wc -l < "$unexpected_file" | tr -d ' ')
  if [[ "$unexpected_count" -gt 50 ]]; then
    echo "... and $((unexpected_count - 50)) more" >&2
  fi
  exit 1
fi

echo "Verified hidden native export surface for $library"
