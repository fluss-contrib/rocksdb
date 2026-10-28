// Copyright (c) Meta Platforms, Inc. and affiliates.
// This source code is licensed under both the GPLv2 (found in the
// COPYING file in the root directory) and Apache 2.0 License
// (found in the LICENSE.Apache file in the root directory).

package io.github.fluss_contrib.rocksdb;

import java.util.Objects;

/**
 * An immutable description of a prefix made of consecutive encoded components.
 * Components describe byte boundaries, not logical fields or their values.
 *
 * <p>For example, a varint followed by length-prefixed bytes can be described as
 * {@code PrefixLayout.of(PrefixLayout.varint32(), PrefixLayout.bytesVarint32())}.
 * An extractor stops after these components, regardless of any trailing bytes.
 * Inputs that do not contain all components are outside the extractor domain.
 *
 * <p>Layouts are installed once through JNI. Prefix extraction runs entirely in
 * native code, without Java callbacks or copies of the extracted prefix. This
 * object owns no native resources and can be shared between threads.
 *
 * <p>The caller must ensure that the layout describes every stored key and that
 * equal prefixes form contiguous groups under the column family's comparator.
 * Bytewise ordering satisfies this requirement for these component encodings.
 */
public final class PrefixLayout {
  private final String nativeId;

  private PrefixLayout(final String nativeId) {
    this.nativeId = nativeId;
  }

  /** An immutable byte-boundary operation, constructed by the factories below. */
  public static final class Step {
    private final String encoding;

    private Step(final String encoding) {
      this.encoding = encoding;
    }
  }

  /**
   * Describes a prefix using at least one component. The supplied array is not retained.
   *
   * @param steps the consecutive components making up the prefix
   * @return an immutable layout
   * @throws IllegalArgumentException if there are no components
   * @throws NullPointerException if the array or any component is null
   */
  public static PrefixLayout of(final Step... steps) {
    Objects.requireNonNull(steps, "steps");
    if (steps.length == 0) {
      throw new IllegalArgumentException("A prefix layout needs at least one component");
    }
    final StringBuilder id = new StringBuilder("rocksdb.LayoutPrefix.v1:");
    for (int i = 0; i < steps.length; ++i) {
      if (i != 0) {
        id.append(',');
      }
      id.append(Objects.requireNonNull(steps[i], "step").encoding);
    }
    return new PrefixLayout(id.toString());
  }

  /**
   * Skips a fixed number of bytes.
   * @param bytes positive byte count
   * @return the component
   * @throws IllegalArgumentException if the byte count is not positive
   */
  public static Step fixed(final int bytes) {
    if (bytes <= 0) {
      throw new IllegalArgumentException("Fixed byte count must be positive");
    }
    return new Step("f" + bytes);
  }

  /**
   * Skips an unsigned base128 varint32 (at most five bytes, with overflow checked).
   * @return the component
   */
  public static Step varint32() {
    return new Step("v32");
  }

  /**
   * Skips an unsigned base128 varint64 (at most ten bytes, with overflow checked).
   * @return the component
   */
  public static Step varint64() {
    return new Step("v64");
  }

  /**
   * Reads an unsigned varint32 length, then skips exactly that many payload bytes.
   * An empty payload is allowed. The payload contents are not inspected.
   * @return the component
   */
  public static Step bytesVarint32() {
    return new Step("b32");
  }

  String nativeId() {
    return nativeId;
  }
}
