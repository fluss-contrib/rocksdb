//  Copyright (c) Meta Platforms, Inc. and affiliates.
//
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include "java/rocksjni/table_properties_collector_factory.h"

#include "java/include/io_github_fluss_contrib_rocksdb_TablePropertiesCollectorFactory.h"
#include "java/rocksjni/cplusplus_to_java_convert.h"
#include "rocksdb/db.h"
#include "rocksdb/utilities/table_properties_collectors.h"

/*
 * Class:     io_github_fluss_contrib_rocksdb_TablePropertiesCollectorFactory
 * Method:    newCompactOnDeletionCollectorFactory
 * Signature: (JJD)J
 */
jlong Java_io_github_fluss_1contrib_rocksdb_TablePropertiesCollectorFactory_newCompactOnDeletionCollectorFactory(
    JNIEnv*, jclass, jlong sliding_window_size, jlong deletion_trigger,
    jdouble deletion_ratio) {
  auto* wrapper = new TablePropertiesCollectorFactoriesJniWrapper();
  wrapper->table_properties_collector_factories =
      ROCKSDB_NAMESPACE::NewCompactOnDeletionCollectorFactory(
          sliding_window_size, deletion_trigger, deletion_ratio);
  return GET_CPLUSPLUS_POINTER(wrapper);
}

/*
 * Class:     io_github_fluss_contrib_rocksdb_TablePropertiesCollectorFactory
 * Method:    deleteCompactOnDeletionCollectorFactory
 * Signature: (J)J
 */
void Java_io_github_fluss_1contrib_rocksdb_TablePropertiesCollectorFactory_deleteCompactOnDeletionCollectorFactory(
    JNIEnv*, jclass, jlong jhandle) {
  auto instance =
      reinterpret_cast<TablePropertiesCollectorFactoriesJniWrapper*>(jhandle);
  delete instance;
}
