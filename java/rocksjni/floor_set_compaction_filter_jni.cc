/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>

#include <cstdint>
#include <unordered_set>
#include <utility>

#include "include/io_github_fluss_contrib_rocksdb_FloorSetCompactionFilter.h"
#include "rocksjni/cplusplus_to_java_convert.h"
#include "utilities/compaction_filters/floor_set_compaction_filter.h"

jlong Java_io_github_fluss_1contrib_rocksdb_FloorSetCompactionFilter_createNative(
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
