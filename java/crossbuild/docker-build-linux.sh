#!/usr/bin/env bash
# Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.

set -e
#set -x

# Set job parallelism to 1 (none) if it is not defined in the environment
if [ -z "${J}" ]; then
  J=1
fi

# just in-case this is run outside Docker
mkdir -p /rocksdb-local-build

rm -rf /rocksdb-local-build/*
cp -r /rocksdb-host/* /rocksdb-local-build
cd /rocksdb-local-build

# Use the same dependency mirror as the upstream GitHub Actions setup. The
# Makefile still verifies every downloaded archive against its pinned SHA-256.
export ZLIB_DOWNLOAD_BASE="${ZLIB_DOWNLOAD_BASE:-https://rocksdb-deps.s3.us-west-2.amazonaws.com/pkgs/zlib}"
export BZIP2_DOWNLOAD_BASE="${BZIP2_DOWNLOAD_BASE:-https://rocksdb-deps.s3.us-west-2.amazonaws.com/pkgs/bzip2}"
export SNAPPY_DOWNLOAD_BASE="${SNAPPY_DOWNLOAD_BASE:-https://rocksdb-deps.s3.us-west-2.amazonaws.com/pkgs/snappy}"
export LZ4_DOWNLOAD_BASE="${LZ4_DOWNLOAD_BASE:-https://rocksdb-deps.s3.us-west-2.amazonaws.com/pkgs/lz4}"
export ZSTD_DOWNLOAD_BASE="${ZSTD_DOWNLOAD_BASE:-https://rocksdb-deps.s3.us-west-2.amazonaws.com/pkgs/zstd}"

# Use scl devtoolset if available
if hash scl 2>/dev/null; then
  DEVTOOLSET=$(scl --list 2>/dev/null | grep '^devtoolset-' | sort -V | tail -1)
  if [ -z "$DEVTOOLSET" ]; then
    echo "Could not find devtoolset"
    exit 1;
  fi
  if [ "$DEVTOOLSET" = "devtoolset-12" ]; then
    export EXTRA_CXXFLAGS="${EXTRA_CXXFLAGS:-} -Wno-error=restrict"
  fi
  scl enable "$DEVTOOLSET" 'make clean-not-downloaded'
  scl enable "$DEVTOOLSET" "PORTABLE=1 J=$J make -j$J rocksdbjavastatic"
else
  make clean-not-downloaded
  PORTABLE=1 make -j$J rocksdbjavastatic
fi

cp java/target/libflussrocksdbjni-linux*.so java/target/rocksdbjni-*-linux*.jar java/target/rocksdbjni-*-linux*.jar.sha1 /rocksdb-java-target
