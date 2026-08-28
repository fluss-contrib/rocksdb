//  Copyright (c) Meta Platforms, Inc. and affiliates.
// Copyright (c) 2024-present, platinumhamburg. All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include <jni.h>

#include <cstdint>
#include <unordered_set>
#include <utility>

#include "include/org_fluss_rocksdb_FloorSetCompactionFilter.h"
#include "rocksjni/cplusplus_to_java_convert.h"
#include "utilities/compaction_filters/floor_set_compaction_filter.h"

jlong Java_org_fluss_rocksdb_FloorSetCompactionFilter_createNative(
    JNIEnv* env, jclass, jint tag_offset, jlong floor,
    jlongArray explicit_set_array) {
  std::unordered_set<int64_t> explicit_set;
  if (explicit_set_array != nullptr) {
    const jsize length = env->GetArrayLength(explicit_set_array);
    jlong* elements = env->GetLongArrayElements(explicit_set_array, nullptr);
    if (elements == nullptr) {
      return 0;
    }
    explicit_set.reserve(static_cast<std::size_t>(length));
    for (jsize i = 0; i < length; ++i) {
      explicit_set.insert(static_cast<int64_t>(elements[i]));
    }
    env->ReleaseLongArrayElements(explicit_set_array, elements, JNI_ABORT);
  }

  auto* filter = new ROCKSDB_NAMESPACE::FloorSetCompactionFilter(
      static_cast<int>(tag_offset), static_cast<int64_t>(floor),
      std::move(explicit_set));
  return GET_CPLUSPLUS_POINTER(filter);
}
