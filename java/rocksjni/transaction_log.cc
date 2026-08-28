// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// This file implements the "bridge" between Java and C++ and enables
// calling c++ ROCKSDB_NAMESPACE::Iterator methods from Java side.

#include "rocksdb/transaction_log.h"

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/io_github_fluss_contrib_rocksdb_TransactionLogIterator.h"
#include "rocksjni/portal.h"

/*
 * Class:     io_github_fluss_contrib_rocksdb_TransactionLogIterator
 * Method:    disposeInternal
 * Signature: (J)V
 */
void Java_io_github_fluss_1contrib_rocksdb_TransactionLogIterator_disposeInternalJni(
    JNIEnv* /*env*/, jclass /*jcls*/, jlong handle) {
  delete reinterpret_cast<ROCKSDB_NAMESPACE::TransactionLogIterator*>(handle);
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_TransactionLogIterator
 * Method:    isValid
 * Signature: (J)Z
 */
jboolean Java_io_github_fluss_1contrib_rocksdb_TransactionLogIterator_isValid(
    JNIEnv* /*env*/, jclass /*jcls*/, jlong handle) {
  return reinterpret_cast<ROCKSDB_NAMESPACE::TransactionLogIterator*>(handle)
      ->Valid();
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_TransactionLogIterator
 * Method:    next
 * Signature: (J)V
 */
void Java_io_github_fluss_1contrib_rocksdb_TransactionLogIterator_next(
    JNIEnv* /*env*/, jclass /*jcls*/, jlong handle) {
  reinterpret_cast<ROCKSDB_NAMESPACE::TransactionLogIterator*>(handle)->Next();
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_TransactionLogIterator
 * Method:    status
 * Signature: (J)V
 */
void Java_io_github_fluss_1contrib_rocksdb_TransactionLogIterator_status(
    JNIEnv* env, jclass /*jcls*/, jlong handle) {
  ROCKSDB_NAMESPACE::Status s =
      reinterpret_cast<ROCKSDB_NAMESPACE::TransactionLogIterator*>(handle)
          ->status();
  if (!s.ok()) {
    ROCKSDB_NAMESPACE::RocksDBExceptionJni::ThrowNew(env, s);
  }
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_TransactionLogIterator
 * Method:    getBatch
 * Signature:
 * (J)Lio/github/fluss_contrib/rocksdb/TransactionLogIterator$BatchResult
 */
jobject Java_io_github_fluss_1contrib_rocksdb_TransactionLogIterator_getBatch(
    JNIEnv* env, jclass /*jcls*/, jlong handle) {
  ROCKSDB_NAMESPACE::BatchResult batch_result =
      reinterpret_cast<ROCKSDB_NAMESPACE::TransactionLogIterator*>(handle)
          ->GetBatch();
  return ROCKSDB_NAMESPACE::BatchResultJni::construct(env, batch_result);
}
