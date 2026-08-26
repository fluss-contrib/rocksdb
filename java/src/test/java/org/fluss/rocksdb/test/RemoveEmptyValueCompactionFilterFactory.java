// Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.
package org.fluss.rocksdb.test;

import org.fluss.rocksdb.AbstractCompactionFilter;
import org.fluss.rocksdb.AbstractCompactionFilterFactory;
import org.fluss.rocksdb.RemoveEmptyValueCompactionFilter;

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
