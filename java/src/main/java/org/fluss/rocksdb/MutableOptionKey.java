// Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.
package org.fluss.rocksdb;

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
