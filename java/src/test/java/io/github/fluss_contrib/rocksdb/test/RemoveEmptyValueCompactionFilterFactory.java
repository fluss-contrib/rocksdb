// Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.
package io.github.fluss_contrib.rocksdb.test;

import io.github.fluss_contrib.rocksdb.AbstractCompactionFilter;
import io.github.fluss_contrib.rocksdb.AbstractCompactionFilterFactory;
import io.github.fluss_contrib.rocksdb.RemoveEmptyValueCompactionFilter;

/**
 * Simple CompactionFilterFactory class used in tests. Generates RemoveEmptyValueCompactionFilters.
 */
public class RemoveEmptyValueCompactionFilterFactory extends AbstractCompactionFilterFactory<RemoveEmptyValueCompactionFilter> {
    @Override
    public RemoveEmptyValueCompactionFilter createCompactionFilter(final AbstractCompactionFilter.Context context) {
        return new RemoveEmptyValueCompactionFilter();
    }

    @Override
    public String name() {
        return "RemoveEmptyValueCompactionFilterFactory";
    }
}
