// Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.
package io.github.fluss_contrib.rocksdb;

public interface MutableOptionKey {
  enum ValueType {
    DOUBLE,
    LONG,
    INT,
    BOOLEAN,
    INT_ARRAY,
    ENUM,
    STRING,

  }

  String name();
  ValueType getValueType();
}
