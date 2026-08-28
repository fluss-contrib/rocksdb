//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// This file implements the "bridge" between Java and C++ for
// io.github.fluss_contrib.rocksdb.AbstractTableFilter.

#include <jni.h>

#include <memory>

#include "include/io_github_fluss_contrib_rocksdb_AbstractTableFilter.h"
#include "rocksjni/cplusplus_to_java_convert.h"
#include "rocksjni/table_filter_jnicallback.h"

/*
 * Class:     io_github_fluss_contrib_rocksdb_AbstractTableFilter
 * Method:    createNewTableFilter
 * Signature: ()J
 */
jlong Java_io_github_fluss_1contrib_rocksdb_AbstractTableFilter_createNewTableFilter(
    JNIEnv* env, jobject jtable_filter) {
  auto* table_filter_jnicallback =
      new ROCKSDB_NAMESPACE::TableFilterJniCallback(env, jtable_filter);
  return GET_CPLUSPLUS_POINTER(table_filter_jnicallback);
}
