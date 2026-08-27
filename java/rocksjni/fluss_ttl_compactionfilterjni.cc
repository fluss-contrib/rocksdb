//  Copyright (c) Meta Platforms, Inc. and affiliates.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include <jni.h>

#include <cstdint>
#include <limits>
#include <memory>

#include "include/org_fluss_rocksdb_FlussTtlCompactionFilter.h"
#include "rocksjni/jnicallback.h"
#include "rocksjni/loggerjnicallback.h"
#include "rocksjni/portal.h"
#include "utilities/fluss/fluss_ttl_compaction_filter.h"

namespace {

using ROCKSDB_NAMESPACE::JavaClass;
using ROCKSDB_NAMESPACE::JniCallback;
using ROCKSDB_NAMESPACE::Logger;
using ROCKSDB_NAMESPACE::LoggerJniCallback;
using ROCKSDB_NAMESPACE::Slice;
using ROCKSDB_NAMESPACE::fluss::FlussTtlCompactionFilter;

bool ClearPendingException(JNIEnv* env) {
  if (env == nullptr || !env->ExceptionCheck()) {
    return false;
  }
  env->ExceptionDescribe();
  env->ExceptionClear();
  return true;
}

class JavaListElementFilter
    : public FlussTtlCompactionFilter::ListElementFilter,
      private JniCallback {
 public:
  JavaListElementFilter(JNIEnv* env, jobject list_filter)
      : JniCallback(env, list_filter) {
    jclass clazz = JavaClass::getJClass(
        env, "org/fluss/rocksdb/FlussTtlCompactionFilter$ListElementFilter");
    if (clazz != nullptr) {
      next_unexpired_offset_method_ =
          env->GetMethodID(clazz, "nextUnexpiredOffset", "([BJJ)I");
    }
    if (ClearPendingException(env)) {
      next_unexpired_offset_method_ = nullptr;
    }
  }

  bool NextUnexpiredOffset(const Slice& list, int64_t ttl,
                           int64_t current_timestamp,
                           int64_t* offset) const override {
    if (offset == nullptr || next_unexpired_offset_method_ == nullptr ||
        list.size() >
            static_cast<std::size_t>(std::numeric_limits<jsize>::max())) {
      return false;
    }

    jboolean attached_thread = JNI_FALSE;
    JNIEnv* env = getJniEnv(&attached_thread);
    if (env == nullptr) {
      return false;
    }

    jbyteArray java_list = env->NewByteArray(static_cast<jsize>(list.size()));
    if (java_list == nullptr || ClearPendingException(env)) {
      if (java_list != nullptr) {
        env->DeleteLocalRef(java_list);
      }
      releaseJniEnv(attached_thread);
      return false;
    }
    env->SetByteArrayRegion(java_list, 0, static_cast<jsize>(list.size()),
                            reinterpret_cast<const jbyte*>(list.data()));
    if (ClearPendingException(env)) {
      env->DeleteLocalRef(java_list);
      releaseJniEnv(attached_thread);
      return false;
    }

    const jint next_offset = env->CallIntMethod(
        m_jcallback_obj, next_unexpired_offset_method_, java_list,
        static_cast<jlong>(ttl), static_cast<jlong>(current_timestamp));
    const bool failed = ClearPendingException(env);
    env->DeleteLocalRef(java_list);
    releaseJniEnv(attached_thread);
    if (failed) {
      return false;
    }
    *offset = static_cast<int64_t>(next_offset);
    return true;
  }

 private:
  jmethodID next_unexpired_offset_method_ = nullptr;
};

class JavaListElementFilterFactory
    : public FlussTtlCompactionFilter::ListElementFilterFactory,
      private JniCallback {
 public:
  JavaListElementFilterFactory(JNIEnv* env, jobject list_filter_factory)
      : JniCallback(env, list_filter_factory) {
    jclass clazz = JavaClass::getJClass(
        env,
        "org/fluss/rocksdb/FlussTtlCompactionFilter$ListElementFilterFactory");
    if (clazz != nullptr) {
      create_filter_method_ = env->GetMethodID(
          clazz, "createListElementFilter",
          "()Lorg/fluss/rocksdb/FlussTtlCompactionFilter$ListElementFilter;");
    }
    if (ClearPendingException(env)) {
      create_filter_method_ = nullptr;
    }
  }

  std::unique_ptr<FlussTtlCompactionFilter::ListElementFilter>
  CreateListElementFilter(std::shared_ptr<Logger>) const override {
    if (create_filter_method_ == nullptr) {
      return nullptr;
    }
    jboolean attached_thread = JNI_FALSE;
    JNIEnv* env = getJniEnv(&attached_thread);
    if (env == nullptr) {
      return nullptr;
    }
    jobject java_filter =
        env->CallObjectMethod(m_jcallback_obj, create_filter_method_);
    if (java_filter == nullptr || ClearPendingException(env)) {
      if (java_filter != nullptr) {
        env->DeleteLocalRef(java_filter);
      }
      releaseJniEnv(attached_thread);
      return nullptr;
    }
    auto filter = std::make_unique<JavaListElementFilter>(env, java_filter);
    env->DeleteLocalRef(java_filter);
    releaseJniEnv(attached_thread);
    return filter;
  }

 private:
  jmethodID create_filter_method_ = nullptr;
};

class JavaTimeProvider : public FlussTtlCompactionFilter::TimeProvider,
                         private JniCallback {
 public:
  JavaTimeProvider(JNIEnv* env, jobject time_provider)
      : JniCallback(env, time_provider) {
    jclass clazz = JavaClass::getJClass(
        env, "org/fluss/rocksdb/FlussTtlCompactionFilter$TimeProvider");
    if (clazz != nullptr) {
      current_timestamp_method_ =
          env->GetMethodID(clazz, "currentTimestamp", "()J");
    }
    if (ClearPendingException(env)) {
      current_timestamp_method_ = nullptr;
    }
  }

  bool CurrentTimestamp(int64_t* current_timestamp) const override {
    if (current_timestamp == nullptr || current_timestamp_method_ == nullptr) {
      return false;
    }
    jboolean attached_thread = JNI_FALSE;
    JNIEnv* env = getJniEnv(&attached_thread);
    if (env == nullptr) {
      return false;
    }
    const jlong timestamp =
        env->CallLongMethod(m_jcallback_obj, current_timestamp_method_);
    const bool failed = ClearPendingException(env);
    releaseJniEnv(attached_thread);
    if (failed) {
      return false;
    }
    *current_timestamp = static_cast<int64_t>(timestamp);
    return true;
  }

 private:
  jmethodID current_timestamp_method_ = nullptr;
};

std::unique_ptr<FlussTtlCompactionFilter::ListElementFilterFactory>
CreateListElementFilterFactory(JNIEnv* env, jint fixed_element_length,
                               jobject list_filter_factory) {
  if (fixed_element_length > 0) {
    return std::make_unique<
        FlussTtlCompactionFilter::FixedListElementFilterFactory>(
        static_cast<std::size_t>(fixed_element_length), 0);
  }
  if (list_filter_factory != nullptr) {
    return std::make_unique<JavaListElementFilterFactory>(env,
                                                          list_filter_factory);
  }
  return nullptr;
}

}  // namespace

jlong Java_org_fluss_rocksdb_FlussTtlCompactionFilter_createNewFlussTtlCompactionFilterConfigHolder(
    JNIEnv*, jclass) {
  auto holder = std::make_shared<FlussTtlCompactionFilter::ConfigHolder>();
  return reinterpret_cast<jlong>(
      new std::shared_ptr<FlussTtlCompactionFilter::ConfigHolder>(
          std::move(holder)));
}

void Java_org_fluss_rocksdb_FlussTtlCompactionFilter_disposeFlussTtlCompactionFilterConfigHolder(
    JNIEnv*, jclass, jlong handle) {
  delete reinterpret_cast<
      std::shared_ptr<FlussTtlCompactionFilter::ConfigHolder>*>(handle);
}

jlong Java_org_fluss_rocksdb_FlussTtlCompactionFilter_createNewFlussTtlCompactionFilter0(
    JNIEnv* env, jclass, jlong config_holder_handle, jobject time_provider,
    jlong logger_handle) {
  auto config_holder = *reinterpret_cast<
      std::shared_ptr<FlussTtlCompactionFilter::ConfigHolder>*>(
      config_holder_handle);
  std::shared_ptr<Logger> logger;
  if (logger_handle != 0) {
    logger =
        *reinterpret_cast<std::shared_ptr<LoggerJniCallback>*>(logger_handle);
  }
  return reinterpret_cast<jlong>(new FlussTtlCompactionFilter(
      std::move(config_holder),
      std::make_unique<JavaTimeProvider>(env, time_provider),
      std::move(logger)));
}

jboolean
Java_org_fluss_rocksdb_FlussTtlCompactionFilter_configureFlussTtlCompactionFilter(
    JNIEnv* env, jclass, jlong handle, jint state_type, jint timestamp_offset,
    jlong ttl, jlong query_time_after_num_entries, jint fixed_element_length,
    jobject list_filter_factory) {
  if (handle == 0 || state_type < FlussTtlCompactionFilter::Disabled ||
      state_type > FlussTtlCompactionFilter::List || timestamp_offset < 0 ||
      query_time_after_num_entries < 0) {
    return JNI_FALSE;
  }
  auto config = std::make_unique<FlussTtlCompactionFilter::Config>(
      static_cast<FlussTtlCompactionFilter::StateType>(state_type),
      static_cast<std::size_t>(timestamp_offset), static_cast<int64_t>(ttl),
      static_cast<int64_t>(query_time_after_num_entries),
      CreateListElementFilterFactory(env, fixed_element_length,
                                     list_filter_factory));
  auto config_holder = *reinterpret_cast<
      std::shared_ptr<FlussTtlCompactionFilter::ConfigHolder>*>(handle);
  return config_holder->Configure(std::move(config)) ? JNI_TRUE : JNI_FALSE;
}
