//  Copyright (c) Meta Platforms, Inc. and affiliates.
// Copyright (c) 2024-present, platinumhamburg. All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

package org.fluss.rocksdb;

/**
 * A compaction filter that removes entries based on a tag embedded in each value.
 *
 * <p>The tag is a signed, big-endian {@code long} at the caller-provided byte offset. An entry is
 * removed when its tag is less than or equal to {@code floor}, or when the tag is present in {@code
 * explicitSet}. Values too short to contain the complete tag are kept.
 */
public class FloorSetCompactionFilter extends AbstractCompactionFilter<Slice> {
  /**
   * Creates a floor-and-set compaction filter.
   *
   * @param tagOffset byte offset of the tag within each value
   * @param floor inclusive floor for tags to remove
   * @param explicitSet additional tags to remove; {@code null} is treated as an empty set
   */
  public FloorSetCompactionFilter(int tagOffset, long floor, long[] explicitSet) {
    super(createNative(tagOffset, floor, explicitSet));
  }

  private static native long createNative(int tagOffset, long floor, long[] explicitSet);
}
