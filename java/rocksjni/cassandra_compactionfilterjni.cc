// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include <jni.h>

#include "include/io_github_fluss_contrib_rocksdb_CassandraCompactionFilter.h"
#include "rocksjni/cplusplus_to_java_convert.h"
#include "utilities/cassandra/cassandra_compaction_filter.h"

/*
 * Class:     io_github_fluss_contrib_rocksdb_CassandraCompactionFilter
 * Method:    createNewCassandraCompactionFilter0
 * Signature: (ZI)J
 */
jlong Java_io_github_fluss_1contrib_rocksdb_CassandraCompactionFilter_createNewCassandraCompactionFilter0(
    JNIEnv* /*env*/, jclass /*jcls*/, jboolean purge_ttl_on_expiration,
    jint gc_grace_period_in_seconds) {
  auto* compaction_filter =
      new ROCKSDB_NAMESPACE::cassandra::CassandraCompactionFilter(
          purge_ttl_on_expiration, gc_grace_period_in_seconds);
  // set the native handle to our native compaction filter
  return GET_CPLUSPLUS_POINTER(compaction_filter);
}
