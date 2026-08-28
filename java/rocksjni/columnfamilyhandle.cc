// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// This file implements the "bridge" between Java and C++ for
// ROCKSDB_NAMESPACE::ColumnFamilyHandle.

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/io_github_fluss_contrib_rocksdb_ColumnFamilyHandle.h"
#include "rocksjni/portal.h"

/*
 * Class:     io_github_fluss_contrib_rocksdb_ColumnFamilyHandle
 * Method:    getName
 * Signature: (J)[B
 */
jbyteArray Java_io_github_fluss_1contrib_rocksdb_ColumnFamilyHandle_getName(
    JNIEnv* env, jclass /*jobj*/, jlong jhandle) {
  auto* cfh = reinterpret_cast<ROCKSDB_NAMESPACE::ColumnFamilyHandle*>(jhandle);
  std::string cf_name = cfh->GetName();
  return ROCKSDB_NAMESPACE::JniUtil::copyBytes(env, cf_name);
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_ColumnFamilyHandle
 * Method:    getID
 * Signature: (J)I
 */
jint Java_io_github_fluss_1contrib_rocksdb_ColumnFamilyHandle_getID(
    JNIEnv* /*env*/, jclass /*jcls*/, jlong jhandle) {
  auto* cfh = reinterpret_cast<ROCKSDB_NAMESPACE::ColumnFamilyHandle*>(jhandle);
  const int32_t id = cfh->GetID();
  return static_cast<jint>(id);
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_ColumnFamilyHandle
 * Method:    getDescriptor
 * Signature: (J)Lio/github/fluss_contrib/rocksdb/ColumnFamilyDescriptor;
 */
jobject Java_io_github_fluss_1contrib_rocksdb_ColumnFamilyHandle_getDescriptor(
    JNIEnv* env, jclass /*jcls*/, jlong jhandle) {
  auto* cfh = reinterpret_cast<ROCKSDB_NAMESPACE::ColumnFamilyHandle*>(jhandle);
  ROCKSDB_NAMESPACE::ColumnFamilyDescriptor desc;
  ROCKSDB_NAMESPACE::Status s = cfh->GetDescriptor(&desc);
  if (s.ok()) {
    return ROCKSDB_NAMESPACE::ColumnFamilyDescriptorJni::construct(env, &desc);
  } else {
    ROCKSDB_NAMESPACE::RocksDBExceptionJni::ThrowNew(env, s);
    return nullptr;
  }
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_ColumnFamilyHandle
 * Method:    disposeInternal
 * Signature: (J)V
 */
void Java_io_github_fluss_1contrib_rocksdb_ColumnFamilyHandle_disposeInternalJni(
    JNIEnv* /*env*/, jclass /*jobj*/, jlong jhandle) {
  auto* cfh = reinterpret_cast<ROCKSDB_NAMESPACE::ColumnFamilyHandle*>(jhandle);
  assert(cfh != nullptr);
  delete cfh;
}
