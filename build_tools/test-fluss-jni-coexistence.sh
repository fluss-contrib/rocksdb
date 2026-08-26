#!/usr/bin/env bash

set -euo pipefail

if [[ $# -ne 2 ]]; then
  echo "Usage: $0 <Fluss RocksDB JNI JAR> <work-directory>" >&2
  exit 2
fi

fluss_jar=$1
work_directory=$2
frocksdbjni_version=6.20.3-ververica-2.0
frocksdbjni_sha256=5e6e5063b75196a17fbeaf656f088a807f83177525d9ecbec7125b9f3630b966
frocksdbjni_jar="$work_directory/frocksdbjni-$frocksdbjni_version.jar"
classes_directory="$work_directory/classes"

test -f "$fluss_jar"
mkdir -p "$classes_directory"
curl --fail --location --retry 3 \
  --output "$frocksdbjni_jar" \
  "https://repo1.maven.org/maven2/com/ververica/frocksdbjni/$frocksdbjni_version/frocksdbjni-$frocksdbjni_version.jar"
printf '%s  %s\n' "$frocksdbjni_sha256" "$frocksdbjni_jar" | shasum -a 256 --check

classpath="$fluss_jar:$frocksdbjni_jar"
javac -source 8 -target 8 -cp "$classpath" -d "$classes_directory" \
  java/samples/src/main/java/RocksDBCoexistenceSample.java

for load_order in fluss-first flink-first; do
  java -ea -Xcheck:jni -cp "$classpath:$classes_directory" \
    RocksDBCoexistenceSample "$load_order" "$work_directory/$load_order"
done
