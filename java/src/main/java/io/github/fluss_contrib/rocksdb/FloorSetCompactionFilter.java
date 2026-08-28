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

package io.github.fluss_contrib.rocksdb;

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
   * @param tagOffset byte offset of the tag within each value; no default is applied, so callers
   *     must specify it according to the serialized value layout
   * @param floor inclusive floor for tags to remove
   * @param explicitSet additional tags to remove; {@code null} is treated as an empty set
   */
  public FloorSetCompactionFilter(int tagOffset, long floor, long[] explicitSet) {
    super(createNative(tagOffset, floor, explicitSet));
  }

  private static native long createNative(int tagOffset, long floor, long[] explicitSet);
}
