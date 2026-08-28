//  Copyright (c) Meta Platforms, Inc. and affiliates.
//
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include <jni.h>

#include "include/io_github_fluss_contrib_rocksdb_ImportColumnFamilyOptions.h"
#include "rocksdb/options.h"
#include "rocksjni/cplusplus_to_java_convert.h"

/*
 * Class:     io_github_fluss_contrib_rocksdb_ImportColumnFamilyOptions
 * Method:    newImportColumnFamilyOptions
 * Signature: ()J
 */
jlong Java_io_github_fluss_1contrib_rocksdb_ImportColumnFamilyOptions_newImportColumnFamilyOptions(
    JNIEnv*, jclass) {
  ROCKSDB_NAMESPACE::ImportColumnFamilyOptions* opts =
      new ROCKSDB_NAMESPACE::ImportColumnFamilyOptions();
  return GET_CPLUSPLUS_POINTER(opts);
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_ImportColumnFamilyOptions
 * Method:    setMoveFiles
 * Signature: (JZ)V
 */
void Java_io_github_fluss_1contrib_rocksdb_ImportColumnFamilyOptions_setMoveFiles(
    JNIEnv*, jobject, jlong jhandle, jboolean jmove_files) {
  auto* options =
      reinterpret_cast<ROCKSDB_NAMESPACE::ImportColumnFamilyOptions*>(jhandle);
  options->move_files = static_cast<bool>(jmove_files);
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_ImportColumnFamilyOptions
 * Method:    moveFiles
 * Signature: (J)Z
 */
jboolean
Java_io_github_fluss_1contrib_rocksdb_ImportColumnFamilyOptions_moveFiles(
    JNIEnv*, jobject, jlong jhandle) {
  auto* options =
      reinterpret_cast<ROCKSDB_NAMESPACE::ImportColumnFamilyOptions*>(jhandle);
  return static_cast<jboolean>(options->move_files);
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_ImportColumnFamilyOptions
 * Method:    disposeInternal
 * Signature: (J)V
 */
void Java_io_github_fluss_1contrib_rocksdb_ImportColumnFamilyOptions_disposeInternal(
    JNIEnv*, jobject, jlong jhandle) {
  delete reinterpret_cast<ROCKSDB_NAMESPACE::ImportColumnFamilyOptions*>(
      jhandle);
}