// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include <jni.h>

#include "include/io_github_fluss_contrib_rocksdb_RocksDBExceptionTest.h"
#include "rocksdb/slice.h"
#include "rocksdb/status.h"
#include "rocksjni/portal.h"

/*
 * Class:     io_github_fluss_contrib_rocksdb_RocksDBExceptionTest
 * Method:    raiseException
 * Signature: ()V
 */
void Java_io_github_fluss_1contrib_rocksdb_RocksDBExceptionTest_raiseException(
    JNIEnv* env, jobject /*jobj*/) {
  ROCKSDB_NAMESPACE::RocksDBExceptionJni::ThrowNew(env,
                                                   std::string("test message"));
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_RocksDBExceptionTest
 * Method:    raiseExceptionWithStatusCode
 * Signature: ()V
 */
void Java_io_github_fluss_1contrib_rocksdb_RocksDBExceptionTest_raiseExceptionWithStatusCode(
    JNIEnv* env, jobject /*jobj*/) {
  ROCKSDB_NAMESPACE::RocksDBExceptionJni::ThrowNew(
      env, "test message", ROCKSDB_NAMESPACE::Status::NotSupported());
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_RocksDBExceptionTest
 * Method:    raiseExceptionNoMsgWithStatusCode
 * Signature: ()V
 */
void Java_io_github_fluss_1contrib_rocksdb_RocksDBExceptionTest_raiseExceptionNoMsgWithStatusCode(
    JNIEnv* env, jobject /*jobj*/) {
  ROCKSDB_NAMESPACE::RocksDBExceptionJni::ThrowNew(
      env, ROCKSDB_NAMESPACE::Status::NotSupported());
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_RocksDBExceptionTest
 * Method:    raiseExceptionWithStatusCodeSubCode
 * Signature: ()V
 */
void Java_io_github_fluss_1contrib_rocksdb_RocksDBExceptionTest_raiseExceptionWithStatusCodeSubCode(
    JNIEnv* env, jobject /*jobj*/) {
  ROCKSDB_NAMESPACE::RocksDBExceptionJni::ThrowNew(
      env, "test message",
      ROCKSDB_NAMESPACE::Status::TimedOut(
          ROCKSDB_NAMESPACE::Status::SubCode::kLockTimeout));
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_RocksDBExceptionTest
 * Method:    raiseExceptionNoMsgWithStatusCodeSubCode
 * Signature: ()V
 */
void Java_io_github_fluss_1contrib_rocksdb_RocksDBExceptionTest_raiseExceptionNoMsgWithStatusCodeSubCode(
    JNIEnv* env, jobject /*jobj*/) {
  ROCKSDB_NAMESPACE::RocksDBExceptionJni::ThrowNew(
      env, ROCKSDB_NAMESPACE::Status::TimedOut(
               ROCKSDB_NAMESPACE::Status::SubCode::kLockTimeout));
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_RocksDBExceptionTest
 * Method:    raiseExceptionWithStatusCodeState
 * Signature: ()V
 */
void Java_io_github_fluss_1contrib_rocksdb_RocksDBExceptionTest_raiseExceptionWithStatusCodeState(
    JNIEnv* env, jobject /*jobj*/) {
  ROCKSDB_NAMESPACE::Slice state("test state");
  ROCKSDB_NAMESPACE::RocksDBExceptionJni::ThrowNew(
      env, "test message", ROCKSDB_NAMESPACE::Status::NotSupported(state));
}
