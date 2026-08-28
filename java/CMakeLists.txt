cmake_minimum_required(VERSION 3.11)

set(JAVA_JUNIT_VERSION "4.13.1")
set(JAVA_HAMCR_VERSION "2.2")
set(JAVA_MOCKITO_VERSION "1.10.19")
set(JAVA_CGLIB_VERSION "3.3.0")
set(JAVA_ASSERTJ_VERSION "2.9.0")


if(${CMAKE_VERSION} VERSION_LESS "3.11.4")
    message("Please consider switching to CMake 3.11.4 or newer")
endif()

set(CMAKE_JAVA_COMPILE_FLAGS -source 8)

set(JNI_NATIVE_SOURCES
        rocksjni/backup_engine_options.cc
        rocksjni/backupenginejni.cc
        rocksjni/cache.cc
        rocksjni/cassandra_compactionfilterjni.cc
        rocksjni/cassandra_value_operator.cc
        rocksjni/checkpoint.cc
        rocksjni/clock_cache.cc
        rocksjni/columnfamilyhandle.cc
        rocksjni/compact_range_options.cc
        rocksjni/compaction_filter.cc
        rocksjni/compaction_filter_factory.cc
        rocksjni/compaction_filter_factory_jnicallback.cc
        rocksjni/compaction_job_info.cc
        rocksjni/compaction_job_stats.cc
        rocksjni/compaction_options.cc
        rocksjni/compaction_options_fifo.cc
        rocksjni/compaction_options_universal.cc
        rocksjni/comparator.cc
        rocksjni/comparatorjnicallback.cc
        rocksjni/compression_options.cc
        rocksjni/concurrent_task_limiter.cc
        rocksjni/config_options.cc
        rocksjni/env.cc
        rocksjni/env_options.cc
        rocksjni/event_listener.cc
        rocksjni/event_listener_jnicallback.cc
        rocksjni/export_import_files_metadatajni.cc
        rocksjni/filter.cc
        rocksjni/floor_set_compaction_filter_jni.cc
        rocksjni/fluss_ttl_compactionfilterjni.cc
        rocksjni/hyper_clock_cache.cc
        rocksjni/import_column_family_options.cc
        rocksjni/ingest_external_file_options.cc
        rocksjni/iterator.cc
        rocksjni/jni_multiget_helpers.cc
        rocksjni/jni_perf_context.cc
        rocksjni/jnicallback.cc
        rocksjni/loggerjnicallback.cc
        rocksjni/lru_cache.cc
        rocksjni/memory_util.cc
        rocksjni/memtablejni.cc
        rocksjni/merge_operator.cc
        rocksjni/native_comparator_wrapper_test.cc
        rocksjni/optimistic_transaction_db.cc
        rocksjni/optimistic_transaction_options.cc
        rocksjni/options.cc
        rocksjni/options_util.cc
        rocksjni/persistent_cache.cc
        rocksjni/ratelimiterjni.cc
        rocksjni/remove_emptyvalue_compactionfilterjni.cc
        rocksjni/restorejni.cc
        rocksjni/rocks_callback_object.cc
        rocksjni/rocksdb_exception_test.cc
        rocksjni/rocksjni.cc
        rocksjni/slice.cc
        rocksjni/snapshot.cc
        rocksjni/sst_file_manager.cc
        rocksjni/sst_file_reader_iterator.cc
        rocksjni/sst_file_readerjni.cc
        rocksjni/sst_file_writerjni.cc
        rocksjni/sst_partitioner.cc
        rocksjni/statistics.cc
        rocksjni/statisticsjni.cc
        rocksjni/stderr_logger.cc
        rocksjni/table.cc
        rocksjni/table_filter.cc
        rocksjni/table_filter_jnicallback.cc
        rocksjni/table_properties_collector_factory.cc
        rocksjni/testable_event_listener.cc
        rocksjni/thread_status.cc
        rocksjni/trace_writer.cc
        rocksjni/trace_writer_jnicallback.cc
        rocksjni/transaction.cc
        rocksjni/transaction_db.cc
        rocksjni/transaction_db_options.cc
        rocksjni/transaction_log.cc
        rocksjni/transaction_notifier.cc
        rocksjni/transaction_notifier_jnicallback.cc
        rocksjni/transaction_options.cc
        rocksjni/ttl.cc
        rocksjni/wal_filter.cc
        rocksjni/wal_filter_jnicallback.cc
        rocksjni/write_batch.cc
        rocksjni/write_batch_test.cc
        rocksjni/write_batch_with_index.cc
        rocksjni/write_buffer_manager.cc
        rocksjni/writebatchhandlerjnicallback.cc
)

set(JAVA_MAIN_CLASSES
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractCompactionFilter.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractCompactionFilterFactory.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractComparator.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractComparatorJniBridge.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractEventListener.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractImmutableNativeReference.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractMutableOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractNativeReference.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractRocksIterator.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractSlice.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractTableFilter.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractTraceWriter.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractTransactionNotifier.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractWalFilter.java
  src/main/java/io/github/fluss_contrib/rocksdb/AbstractWriteBatch.java
  src/main/java/io/github/fluss_contrib/rocksdb/AdvancedColumnFamilyOptionsInterface.java
  src/main/java/io/github/fluss_contrib/rocksdb/AdvancedMutableColumnFamilyOptionsInterface.java
  src/main/java/io/github/fluss_contrib/rocksdb/BackgroundErrorReason.java
  src/main/java/io/github/fluss_contrib/rocksdb/BackupEngine.java
  src/main/java/io/github/fluss_contrib/rocksdb/BackupEngineOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/BackupInfo.java
  src/main/java/io/github/fluss_contrib/rocksdb/BlockBasedTableConfig.java
  src/main/java/io/github/fluss_contrib/rocksdb/BloomFilter.java
  src/main/java/io/github/fluss_contrib/rocksdb/BuiltinComparator.java
  src/main/java/io/github/fluss_contrib/rocksdb/ByteBufferGetStatus.java
  src/main/java/io/github/fluss_contrib/rocksdb/Cache.java
  src/main/java/io/github/fluss_contrib/rocksdb/CassandraCompactionFilter.java
  src/main/java/io/github/fluss_contrib/rocksdb/CassandraValueMergeOperator.java
  src/main/java/io/github/fluss_contrib/rocksdb/Checkpoint.java
  src/main/java/io/github/fluss_contrib/rocksdb/ChecksumType.java
  src/main/java/io/github/fluss_contrib/rocksdb/ClockCache.java
  src/main/java/io/github/fluss_contrib/rocksdb/ColumnFamilyDescriptor.java
  src/main/java/io/github/fluss_contrib/rocksdb/ColumnFamilyHandle.java
  src/main/java/io/github/fluss_contrib/rocksdb/ColumnFamilyMetaData.java
  src/main/java/io/github/fluss_contrib/rocksdb/ColumnFamilyOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/ColumnFamilyOptionsInterface.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompactRangeOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompactionJobInfo.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompactionJobStats.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompactionOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompactionOptionsFIFO.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompactionOptionsUniversal.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompactionPriority.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompactionReason.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompactionStopStyle.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompactionStyle.java
  src/main/java/io/github/fluss_contrib/rocksdb/ComparatorOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/ComparatorType.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompressionOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/CompressionType.java
  src/main/java/io/github/fluss_contrib/rocksdb/ConcurrentTaskLimiter.java
  src/main/java/io/github/fluss_contrib/rocksdb/ConcurrentTaskLimiterImpl.java
  src/main/java/io/github/fluss_contrib/rocksdb/ConfigOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/DBOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/DBOptionsInterface.java
  src/main/java/io/github/fluss_contrib/rocksdb/DataBlockIndexType.java
  src/main/java/io/github/fluss_contrib/rocksdb/DbPath.java
  src/main/java/io/github/fluss_contrib/rocksdb/DirectSlice.java
  src/main/java/io/github/fluss_contrib/rocksdb/EncodingType.java
  src/main/java/io/github/fluss_contrib/rocksdb/Env.java
  src/main/java/io/github/fluss_contrib/rocksdb/EnvOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/EventListener.java
  src/main/java/io/github/fluss_contrib/rocksdb/Experimental.java
  src/main/java/io/github/fluss_contrib/rocksdb/ExportImportFilesMetaData.java
  src/main/java/io/github/fluss_contrib/rocksdb/ExternalFileIngestionInfo.java
  src/main/java/io/github/fluss_contrib/rocksdb/FileOperationInfo.java
  src/main/java/io/github/fluss_contrib/rocksdb/Filter.java
  src/main/java/io/github/fluss_contrib/rocksdb/FilterPolicyType.java
  src/main/java/io/github/fluss_contrib/rocksdb/FloorSetCompactionFilter.java
  src/main/java/io/github/fluss_contrib/rocksdb/FlussTtlCompactionFilter.java
  src/main/java/io/github/fluss_contrib/rocksdb/FlushJobInfo.java
  src/main/java/io/github/fluss_contrib/rocksdb/FlushOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/FlushReason.java
  src/main/java/io/github/fluss_contrib/rocksdb/GetStatus.java
  src/main/java/io/github/fluss_contrib/rocksdb/HashLinkedListMemTableConfig.java
  src/main/java/io/github/fluss_contrib/rocksdb/HashSkipListMemTableConfig.java
  src/main/java/io/github/fluss_contrib/rocksdb/HistogramData.java
  src/main/java/io/github/fluss_contrib/rocksdb/HistogramType.java
  src/main/java/io/github/fluss_contrib/rocksdb/Holder.java
  src/main/java/io/github/fluss_contrib/rocksdb/HyperClockCache.java
  src/main/java/io/github/fluss_contrib/rocksdb/ImportColumnFamilyOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/IndexShorteningMode.java
  src/main/java/io/github/fluss_contrib/rocksdb/IndexSearchType.java
  src/main/java/io/github/fluss_contrib/rocksdb/IndexType.java
  src/main/java/io/github/fluss_contrib/rocksdb/InfoLogLevel.java
  src/main/java/io/github/fluss_contrib/rocksdb/IngestExternalFileOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/KeyMayExist.java
  src/main/java/io/github/fluss_contrib/rocksdb/LRUCache.java
  src/main/java/io/github/fluss_contrib/rocksdb/LevelMetaData.java
  src/main/java/io/github/fluss_contrib/rocksdb/LiveFileMetaData.java
  src/main/java/io/github/fluss_contrib/rocksdb/LogFile.java
  src/main/java/io/github/fluss_contrib/rocksdb/Logger.java
  src/main/java/io/github/fluss_contrib/rocksdb/LoggerInterface.java
  src/main/java/io/github/fluss_contrib/rocksdb/LoggerType.java
  src/main/java/io/github/fluss_contrib/rocksdb/MemTableConfig.java
  src/main/java/io/github/fluss_contrib/rocksdb/MemTableInfo.java
  src/main/java/io/github/fluss_contrib/rocksdb/MemoryUsageType.java
  src/main/java/io/github/fluss_contrib/rocksdb/MemoryUtil.java
  src/main/java/io/github/fluss_contrib/rocksdb/MergeOperator.java
  src/main/java/io/github/fluss_contrib/rocksdb/MutableColumnFamilyOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/MutableColumnFamilyOptionsInterface.java
  src/main/java/io/github/fluss_contrib/rocksdb/MutableDBOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/MutableDBOptionsInterface.java
  src/main/java/io/github/fluss_contrib/rocksdb/MutableOptionKey.java
  src/main/java/io/github/fluss_contrib/rocksdb/MutableOptionValue.java
  src/main/java/io/github/fluss_contrib/rocksdb/NativeComparatorWrapper.java
  src/main/java/io/github/fluss_contrib/rocksdb/NativeLibraryLoader.java
  src/main/java/io/github/fluss_contrib/rocksdb/OperationStage.java
  src/main/java/io/github/fluss_contrib/rocksdb/OperationType.java
  src/main/java/io/github/fluss_contrib/rocksdb/OptimisticTransactionDB.java
  src/main/java/io/github/fluss_contrib/rocksdb/OptimisticTransactionOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/OptionString.java
  src/main/java/io/github/fluss_contrib/rocksdb/Options.java
  src/main/java/io/github/fluss_contrib/rocksdb/OptionsUtil.java
  src/main/java/io/github/fluss_contrib/rocksdb/PerfContext.java
  src/main/java/io/github/fluss_contrib/rocksdb/PerfLevel.java
  src/main/java/io/github/fluss_contrib/rocksdb/PersistentCache.java
  src/main/java/io/github/fluss_contrib/rocksdb/PlainTableConfig.java
  src/main/java/io/github/fluss_contrib/rocksdb/PrepopulateBlobCache.java
  src/main/java/io/github/fluss_contrib/rocksdb/Priority.java
  src/main/java/io/github/fluss_contrib/rocksdb/Range.java
  src/main/java/io/github/fluss_contrib/rocksdb/RateLimiter.java
  src/main/java/io/github/fluss_contrib/rocksdb/RateLimiterMode.java
  src/main/java/io/github/fluss_contrib/rocksdb/ReadOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/ReadTier.java
  src/main/java/io/github/fluss_contrib/rocksdb/RemoveEmptyValueCompactionFilter.java
  src/main/java/io/github/fluss_contrib/rocksdb/RestoreOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/ReusedSynchronisationType.java
  src/main/java/io/github/fluss_contrib/rocksdb/RocksCallbackObject.java
  src/main/java/io/github/fluss_contrib/rocksdb/RocksDB.java
  src/main/java/io/github/fluss_contrib/rocksdb/RocksDBException.java
  src/main/java/io/github/fluss_contrib/rocksdb/RocksEnv.java
  src/main/java/io/github/fluss_contrib/rocksdb/RocksIterator.java
  src/main/java/io/github/fluss_contrib/rocksdb/RocksIteratorInterface.java
  src/main/java/io/github/fluss_contrib/rocksdb/RocksMemEnv.java
  src/main/java/io/github/fluss_contrib/rocksdb/RocksMutableObject.java
  src/main/java/io/github/fluss_contrib/rocksdb/RocksObject.java
  src/main/java/io/github/fluss_contrib/rocksdb/SanityLevel.java
  src/main/java/io/github/fluss_contrib/rocksdb/SizeApproximationFlag.java
  src/main/java/io/github/fluss_contrib/rocksdb/SkipListMemTableConfig.java
  src/main/java/io/github/fluss_contrib/rocksdb/Slice.java
  src/main/java/io/github/fluss_contrib/rocksdb/Snapshot.java
  src/main/java/io/github/fluss_contrib/rocksdb/SstFileManager.java
  src/main/java/io/github/fluss_contrib/rocksdb/SstFileMetaData.java
  src/main/java/io/github/fluss_contrib/rocksdb/SstFileReader.java
  src/main/java/io/github/fluss_contrib/rocksdb/SstFileReaderIterator.java
  src/main/java/io/github/fluss_contrib/rocksdb/SstFileWriter.java
  src/main/java/io/github/fluss_contrib/rocksdb/SstPartitionerFactory.java
  src/main/java/io/github/fluss_contrib/rocksdb/SstPartitionerFixedPrefixFactory.java
  src/main/java/io/github/fluss_contrib/rocksdb/StateType.java
  src/main/java/io/github/fluss_contrib/rocksdb/Statistics.java
  src/main/java/io/github/fluss_contrib/rocksdb/StatisticsCollector.java
  src/main/java/io/github/fluss_contrib/rocksdb/StatisticsCollectorCallback.java
  src/main/java/io/github/fluss_contrib/rocksdb/StatsCollectorInput.java
  src/main/java/io/github/fluss_contrib/rocksdb/StatsLevel.java
  src/main/java/io/github/fluss_contrib/rocksdb/Status.java
  src/main/java/io/github/fluss_contrib/rocksdb/StringAppendOperator.java
  src/main/java/io/github/fluss_contrib/rocksdb/TableFileCreationBriefInfo.java
  src/main/java/io/github/fluss_contrib/rocksdb/TableFileCreationInfo.java
  src/main/java/io/github/fluss_contrib/rocksdb/TableFileCreationReason.java
  src/main/java/io/github/fluss_contrib/rocksdb/TableFileDeletionInfo.java
  src/main/java/io/github/fluss_contrib/rocksdb/TableFilter.java
  src/main/java/io/github/fluss_contrib/rocksdb/TableFormatConfig.java
  src/main/java/io/github/fluss_contrib/rocksdb/TableProperties.java
  src/main/java/io/github/fluss_contrib/rocksdb/TablePropertiesCollectorFactory.java
  src/main/java/io/github/fluss_contrib/rocksdb/ThreadStatus.java
  src/main/java/io/github/fluss_contrib/rocksdb/ThreadType.java
  src/main/java/io/github/fluss_contrib/rocksdb/TickerType.java
  src/main/java/io/github/fluss_contrib/rocksdb/TimedEnv.java
  src/main/java/io/github/fluss_contrib/rocksdb/TraceOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/TraceWriter.java
  src/main/java/io/github/fluss_contrib/rocksdb/Transaction.java
  src/main/java/io/github/fluss_contrib/rocksdb/TransactionDB.java
  src/main/java/io/github/fluss_contrib/rocksdb/TransactionDBOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/TransactionLogIterator.java
  src/main/java/io/github/fluss_contrib/rocksdb/TransactionOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/TransactionalDB.java
  src/main/java/io/github/fluss_contrib/rocksdb/TransactionalOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/TtlDB.java
  src/main/java/io/github/fluss_contrib/rocksdb/TxnDBWritePolicy.java
  src/main/java/io/github/fluss_contrib/rocksdb/UInt64AddOperator.java
  src/main/java/io/github/fluss_contrib/rocksdb/VectorMemTableConfig.java
  src/main/java/io/github/fluss_contrib/rocksdb/WALRecoveryMode.java
  src/main/java/io/github/fluss_contrib/rocksdb/WBWIRocksIterator.java
  src/main/java/io/github/fluss_contrib/rocksdb/WalFileType.java
  src/main/java/io/github/fluss_contrib/rocksdb/WalFilter.java
  src/main/java/io/github/fluss_contrib/rocksdb/WalProcessingOption.java
  src/main/java/io/github/fluss_contrib/rocksdb/WriteBatch.java
  src/main/java/io/github/fluss_contrib/rocksdb/WriteBatchInterface.java
  src/main/java/io/github/fluss_contrib/rocksdb/WriteBatchWithIndex.java
  src/main/java/io/github/fluss_contrib/rocksdb/WriteBufferManager.java
  src/main/java/io/github/fluss_contrib/rocksdb/WriteOptions.java
  src/main/java/io/github/fluss_contrib/rocksdb/WriteStallCondition.java
  src/main/java/io/github/fluss_contrib/rocksdb/WriteStallInfo.java
  src/main/java/io/github/fluss_contrib/rocksdb/util/BufferUtil.java
  src/main/java/io/github/fluss_contrib/rocksdb/util/ByteUtil.java
  src/main/java/io/github/fluss_contrib/rocksdb/util/BytewiseComparator.java
  src/main/java/io/github/fluss_contrib/rocksdb/util/Environment.java
  src/main/java/io/github/fluss_contrib/rocksdb/util/IntComparator.java
  src/main/java/io/github/fluss_contrib/rocksdb/util/ReverseBytewiseComparator.java
  src/main/java/io/github/fluss_contrib/rocksdb/util/SizeUnit.java
  src/main/java/io/github/fluss_contrib/rocksdb/util/StdErrLogger.java
  src/test/java/io/github/fluss_contrib/rocksdb/NativeComparatorWrapperTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/RocksDBExceptionTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/RocksNativeLibraryResource.java
  src/test/java/io/github/fluss_contrib/rocksdb/WriteBatchTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/test/TestableEventListener.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/CapturingWriteBatchHandler.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/WriteBatchGetter.java
)

set(JAVA_TEST_CLASSES
  src/test/java/io/github/fluss_contrib/rocksdb/AbstractTransactionTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/BackupEngineOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/BackupEngineTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/BlobOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/BlockBasedTableConfigTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/BuiltinComparatorTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/ByteBufferUnsupportedOperationTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/BytewiseComparatorRegressionTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CheckPointTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/ClockCacheTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/ColumnFamilyOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/ColumnFamilyTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CompactRangeOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CompactionFilterFactoryTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CompactionJobInfoTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CompactionJobStatsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CompactionOptionsFIFOTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CompactionOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CompactionOptionsUniversalTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CompactionPriorityTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CompactionStopStyleTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/ComparatorOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CompressionOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/CompressionTypesTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/ConcurrentTaskLimiterTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/DBOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/DefaultEnvTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/DirectSliceTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/EnvOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/EventListenerTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/FilterTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/FloorSetCompactionFilterTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/FlussTtlCompactionFilterTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/FlushOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/FlushTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/HyperClockCacheTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/ImportColumnFamilyTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/InfoLogLevelTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/IngestExternalFileOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/KeyExistsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/KeyMayExistTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/LRUCacheTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/LoggerTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MemTableTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MemoryUtilTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MergeCFVariantsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MergeTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MergeVariantsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MixedOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MultiColumnRegressionTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MultiGetManyKeysTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MultiGetTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MutableColumnFamilyOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MutableDBOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/MutableOptionsGetSetTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/NativeLibraryLoaderTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/OptimisticTransactionDBTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/OptimisticTransactionOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/OptimisticTransactionTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/OptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/OptionsUtilTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/PerfContextTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/PerfLevelTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/PlainTableConfigTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/PlatformRandomHelper.java
  src/test/java/io/github/fluss_contrib/rocksdb/PutCFVariantsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/PutMultiplePartsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/PutVariantsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/RateLimiterTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/ReadOnlyTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/ReadOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/RocksDBTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/RocksIteratorTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/RocksMemEnvTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/SecondaryDBTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/SliceTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/SnapshotTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/SstFileManagerTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/SstFileReaderTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/SstFileWriterTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/SstPartitionerTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/StatisticsCollectorTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/StatisticsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/StatsCallbackMock.java
  src/test/java/io/github/fluss_contrib/rocksdb/TableFilterTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/TimedEnvTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/TransactionDBOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/TransactionDBTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/TransactionLogIteratorTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/TransactionOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/TransactionTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/TtlDBTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/Types.java
  src/test/java/io/github/fluss_contrib/rocksdb/VerifyChecksumsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/WALRecoveryModeTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/WalFilterTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/WriteBatchHandlerTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/WriteBatchThreadedTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/WriteBatchWithIndexTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/WriteOptionsTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/test/RemoveEmptyValueCompactionFilterFactory.java
  src/test/java/io/github/fluss_contrib/rocksdb/test/RemoveEmptyValueCompactionFilterFactory.java
  src/test/java/io/github/fluss_contrib/rocksdb/test/RemoveEmptyValueCompactionFilterFactory.java
  src/test/java/io/github/fluss_contrib/rocksdb/test/RocksJunitRunner.java
  src/test/java/io/github/fluss_contrib/rocksdb/test/TestableEventListener.java
  src/test/java/io/github/fluss_contrib/rocksdb/test/TestableEventListener.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/ByteBufferAllocator.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/BytewiseComparatorIntTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/BytewiseComparatorTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/DirectByteBufferAllocator.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/EnvironmentTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/HeapByteBufferAllocator.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/IntComparatorTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/JNIComparatorTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/ReverseBytewiseComparatorIntTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/SizeUnitTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/StdErrLoggerTest.java
  src/test/java/io/github/fluss_contrib/rocksdb/util/TestUtil.java
)

set(JAVA_TEST_RUNNING_CLASSES
  io.github.fluss_contrib.rocksdb.BackupEngineOptionsTest
  io.github.fluss_contrib.rocksdb.BackupEngineTest
  io.github.fluss_contrib.rocksdb.BlobOptionsTest
  io.github.fluss_contrib.rocksdb.BlockBasedTableConfigTest
  io.github.fluss_contrib.rocksdb.BuiltinComparatorTest
  io.github.fluss_contrib.rocksdb.ByteBufferUnsupportedOperationTest
  io.github.fluss_contrib.rocksdb.BytewiseComparatorRegressionTest
  io.github.fluss_contrib.rocksdb.CheckPointTest
  io.github.fluss_contrib.rocksdb.ClockCacheTest
  io.github.fluss_contrib.rocksdb.ColumnFamilyOptionsTest
  io.github.fluss_contrib.rocksdb.ColumnFamilyTest
  io.github.fluss_contrib.rocksdb.CompactRangeOptionsTest
  io.github.fluss_contrib.rocksdb.CompactionFilterFactoryTest
  io.github.fluss_contrib.rocksdb.CompactionJobInfoTest
  io.github.fluss_contrib.rocksdb.CompactionJobStatsTest
  io.github.fluss_contrib.rocksdb.CompactionOptionsFIFOTest
  io.github.fluss_contrib.rocksdb.CompactionOptionsTest
  io.github.fluss_contrib.rocksdb.CompactionOptionsUniversalTest
  io.github.fluss_contrib.rocksdb.CompactionPriorityTest
  io.github.fluss_contrib.rocksdb.CompactionStopStyleTest
  io.github.fluss_contrib.rocksdb.ComparatorOptionsTest
  io.github.fluss_contrib.rocksdb.CompressionOptionsTest
  io.github.fluss_contrib.rocksdb.CompressionTypesTest
  io.github.fluss_contrib.rocksdb.ConcurrentTaskLimiterTest
  io.github.fluss_contrib.rocksdb.DBOptionsTest
  io.github.fluss_contrib.rocksdb.DefaultEnvTest
  io.github.fluss_contrib.rocksdb.DirectSliceTest
  io.github.fluss_contrib.rocksdb.EnvOptionsTest
  io.github.fluss_contrib.rocksdb.EventListenerTest
  io.github.fluss_contrib.rocksdb.FilterTest
  io.github.fluss_contrib.rocksdb.FloorSetCompactionFilterTest
  io.github.fluss_contrib.rocksdb.FlussTtlCompactionFilterTest
  io.github.fluss_contrib.rocksdb.FlushOptionsTest
  io.github.fluss_contrib.rocksdb.FlushTest
  io.github.fluss_contrib.rocksdb.HyperClockCacheTest
  io.github.fluss_contrib.rocksdb.ImportColumnFamilyTest
  io.github.fluss_contrib.rocksdb.InfoLogLevelTest
  io.github.fluss_contrib.rocksdb.IngestExternalFileOptionsTest
  io.github.fluss_contrib.rocksdb.KeyExistsTest
  io.github.fluss_contrib.rocksdb.KeyMayExistTest
  io.github.fluss_contrib.rocksdb.LRUCacheTest
  io.github.fluss_contrib.rocksdb.LoggerTest
  io.github.fluss_contrib.rocksdb.MemTableTest
  io.github.fluss_contrib.rocksdb.MemoryUtilTest
  io.github.fluss_contrib.rocksdb.MergeCFVariantsTest
  io.github.fluss_contrib.rocksdb.MergeTest
  io.github.fluss_contrib.rocksdb.MergeVariantsTest
  io.github.fluss_contrib.rocksdb.MixedOptionsTest
  io.github.fluss_contrib.rocksdb.MultiColumnRegressionTest
  io.github.fluss_contrib.rocksdb.MultiGetManyKeysTest
  io.github.fluss_contrib.rocksdb.MultiGetTest
  io.github.fluss_contrib.rocksdb.MutableColumnFamilyOptionsTest
  io.github.fluss_contrib.rocksdb.MutableDBOptionsTest
  io.github.fluss_contrib.rocksdb.MutableOptionsGetSetTest
  io.github.fluss_contrib.rocksdb.NativeComparatorWrapperTest
  io.github.fluss_contrib.rocksdb.NativeLibraryLoaderTest
  io.github.fluss_contrib.rocksdb.OptimisticTransactionDBTest
  io.github.fluss_contrib.rocksdb.OptimisticTransactionOptionsTest
  io.github.fluss_contrib.rocksdb.OptimisticTransactionTest
  io.github.fluss_contrib.rocksdb.OptionsTest
  io.github.fluss_contrib.rocksdb.OptionsUtilTest
  io.github.fluss_contrib.rocksdb.PerfContextTest
  io.github.fluss_contrib.rocksdb.PerfLevelTest
  io.github.fluss_contrib.rocksdb.PlainTableConfigTest
  io.github.fluss_contrib.rocksdb.PutCFVariantsTest
  io.github.fluss_contrib.rocksdb.PutMultiplePartsTest
  io.github.fluss_contrib.rocksdb.PutVariantsTest
  io.github.fluss_contrib.rocksdb.RateLimiterTest
  io.github.fluss_contrib.rocksdb.ReadOnlyTest
  io.github.fluss_contrib.rocksdb.ReadOptionsTest
  io.github.fluss_contrib.rocksdb.RocksDBExceptionTest
  io.github.fluss_contrib.rocksdb.RocksDBTest
  io.github.fluss_contrib.rocksdb.RocksIteratorTest
  io.github.fluss_contrib.rocksdb.RocksMemEnvTest
  io.github.fluss_contrib.rocksdb.SecondaryDBTest
  io.github.fluss_contrib.rocksdb.SliceTest
  io.github.fluss_contrib.rocksdb.SnapshotTest
  io.github.fluss_contrib.rocksdb.SstFileManagerTest
  io.github.fluss_contrib.rocksdb.SstFileReaderTest
  io.github.fluss_contrib.rocksdb.SstFileWriterTest
  io.github.fluss_contrib.rocksdb.SstPartitionerTest
  io.github.fluss_contrib.rocksdb.StatisticsCollectorTest
  io.github.fluss_contrib.rocksdb.StatisticsTest
  io.github.fluss_contrib.rocksdb.TableFilterTest
  io.github.fluss_contrib.rocksdb.TimedEnvTest
  io.github.fluss_contrib.rocksdb.TransactionDBOptionsTest
  io.github.fluss_contrib.rocksdb.TransactionDBTest
  io.github.fluss_contrib.rocksdb.TransactionLogIteratorTest
  io.github.fluss_contrib.rocksdb.TransactionOptionsTest
  io.github.fluss_contrib.rocksdb.TransactionTest
  io.github.fluss_contrib.rocksdb.TtlDBTest
  io.github.fluss_contrib.rocksdb.VerifyChecksumsTest
  io.github.fluss_contrib.rocksdb.WALRecoveryModeTest
  io.github.fluss_contrib.rocksdb.WalFilterTest
  io.github.fluss_contrib.rocksdb.WriteBatchHandlerTest
  io.github.fluss_contrib.rocksdb.WriteBatchTest
  io.github.fluss_contrib.rocksdb.WriteBatchThreadedTest
  io.github.fluss_contrib.rocksdb.WriteBatchWithIndexTest
  io.github.fluss_contrib.rocksdb.WriteOptionsTest
  io.github.fluss_contrib.rocksdb.util.BytewiseComparatorIntTest
  io.github.fluss_contrib.rocksdb.util.BytewiseComparatorTest
  io.github.fluss_contrib.rocksdb.util.EnvironmentTest
  io.github.fluss_contrib.rocksdb.util.IntComparatorTest
  io.github.fluss_contrib.rocksdb.util.JNIComparatorTest
  io.github.fluss_contrib.rocksdb.util.ReverseBytewiseComparatorIntTest
  io.github.fluss_contrib.rocksdb.util.SizeUnitTest
  io.github.fluss_contrib.rocksdb.util.StdErrLoggerTest
)

include(FindJava)
include(UseJava)
find_package(JNI)

include_directories(${JNI_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/java)

set(JAVA_TEST_LIBDIR ${PROJECT_SOURCE_DIR}/java/test-libs)
set(JAVA_TMP_JAR ${JAVA_TEST_LIBDIR}/tmp.jar)
set(JAVA_JUNIT_JAR ${JAVA_TEST_LIBDIR}/junit-${JAVA_JUNIT_VERSION}.jar)
set(JAVA_HAMCR_JAR ${JAVA_TEST_LIBDIR}/hamcrest-${JAVA_HAMCR_VERSION}.jar)
set(JAVA_MOCKITO_JAR ${JAVA_TEST_LIBDIR}/mockito-all-${JAVA_MOCKITO_VERSION}.jar)
set(JAVA_CGLIB_JAR ${JAVA_TEST_LIBDIR}/cglib-${JAVA_CGLIB_VERSION}.jar)
set(JAVA_ASSERTJ_JAR ${JAVA_TEST_LIBDIR}/assertj-core-${JAVA_ASSERTJ_VERSION}.jar)
set(JAVA_TESTCLASSPATH ${JAVA_JUNIT_JAR} ${JAVA_HAMCR_JAR} ${JAVA_MOCKITO_JAR} ${JAVA_CGLIB_JAR} ${JAVA_ASSERTJ_JAR})
message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message("MINGW: ${MINGW}")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(JAVA_RUN_TESTCLASSPATH ${JAVA_JUNIT_JAR}$<SEMICOLON>${JAVA_HAMCR_JAR}$<SEMICOLON>${JAVA_MOCKITO_JAR}$<SEMICOLON>${JAVA_CGLIB_JAR}$<SEMICOLON>${JAVA_ASSERTJ_JAR})
else()
  set(JAVA_RUN_TESTCLASSPATH ${JAVA_JUNIT_JAR}:${JAVA_HAMCR_JAR}:${JAVA_MOCKITO_JAR}:${JAVA_CGLIB_JAR}:${JAVA_ASSERTJ_JAR})
endif()

set(JNI_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/java/include)
file(MAKE_DIRECTORY ${JNI_OUTPUT_DIR})

if(${Java_VERSION_MINOR} VERSION_LESS_EQUAL "7" AND ${Java_VERSION_MAJOR} STREQUAL "1")
  message(FATAL_ERROR "Detected Java 7 or older (${Java_VERSION_STRING}), minimum required version in now Java 8")
endif()

if(${Java_VERSION_MAJOR} VERSION_GREATER_EQUAL "10" AND ${CMAKE_VERSION} VERSION_LESS "3.11.4")
  # Java 10 and newer don't have javah, but the alternative GENERATE_NATIVE_HEADERS requires CMake 3.11.4 or newer
  message(FATAL_ERROR "Detected Java 10 or newer (${Java_VERSION_STRING}), to build with CMake please upgrade CMake to 3.11.4 or newer")

elseif(${CMAKE_VERSION} VERSION_LESS "3.11.4")
  # Old CMake
  message("Using an old CMAKE (${CMAKE_VERSION}) - JNI headers generated in separate step")
  add_jar(
      rocksdbjni_classes
      SOURCES ${JAVA_MAIN_CLASSES}
  )

else ()
  # Java 1.8 or newer prepare the JAR...
  message("Preparing Jar for JDK ${Java_VERSION_STRING}")
  message("JAVA_TESTCLASSPATH=${JAVA_TESTCLASSPATH}")
  add_jar(
      rocksdbjni_classes
      SOURCES ${JAVA_MAIN_CLASSES}
      INCLUDE_JARS ${ROCKSDBJNI_CLASSES_JAR_FILE} ${JAVA_TESTCLASSPATH}
      GENERATE_NATIVE_HEADERS rocksdbjni_headers DESTINATION ${JNI_OUTPUT_DIR}
  )

endif()

add_jar(
    rocksdbjni_test_classes
    SOURCES
    ${JAVA_MAIN_CLASSES}
    ${JAVA_TEST_CLASSES}
    INCLUDE_JARS ${JAVA_TESTCLASSPATH}
    GENERATE_NATIVE_HEADERS rocksdbjni_test_headers DESTINATION ${JNI_OUTPUT_DIR}
)

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/java/classes)
  file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/java/classes)
endif()

if(NOT EXISTS ${JAVA_TEST_LIBDIR})
  file(MAKE_DIRECTORY mkdir ${JAVA_TEST_LIBDIR})
endif()

if (DEFINED CUSTOM_DEPS_URL)
  set(DEPS_URL ${CUSTOM_DEPS_URL}/)
else ()
  # Using a Facebook AWS account for S3 storage. (maven.org has a history
  # of failing in Travis builds.)
  set(DEPS_URL "https://rocksdb-deps.s3-us-west-2.amazonaws.com/jars")
endif()

if(NOT EXISTS ${JAVA_JUNIT_JAR})
  message("Downloading ${JAVA_JUNIT_JAR}")
  file(DOWNLOAD ${DEPS_URL}/junit-${JAVA_JUNIT_VERSION}.jar ${JAVA_TMP_JAR} STATUS downloadStatus)
  list(GET downloadStatus 0 error_code)
  list(GET downloadStatus 1 error_message)
  if(NOT error_code EQUAL 0)
    message(FATAL_ERROR "Failed downloading ${JAVA_JUNIT_JAR}: ${error_message}")
  endif()
  file(RENAME ${JAVA_TMP_JAR} ${JAVA_JUNIT_JAR})
endif()
if(NOT EXISTS ${JAVA_HAMCR_JAR})
  message("Downloading ${JAVA_HAMCR_JAR}")
  file(DOWNLOAD ${DEPS_URL}/hamcrest-${JAVA_HAMCR_VERSION}.jar ${JAVA_TMP_JAR} STATUS downloadStatus)
  list(GET downloadStatus 0 error_code)
  list(GET downloadStatus 1 error_message)
  if(NOT error_code EQUAL 0)
    message(FATAL_ERROR "Failed downloading ${JAVA_HAMCR_JAR}: ${error_message}")
  endif()
  file(RENAME ${JAVA_TMP_JAR} ${JAVA_HAMCR_JAR})
endif()
if(NOT EXISTS ${JAVA_MOCKITO_JAR})
  message("Downloading ${JAVA_MOCKITO_JAR}")
  file(DOWNLOAD ${DEPS_URL}/mockito-all-${JAVA_MOCKITO_VERSION}.jar ${JAVA_TMP_JAR} STATUS downloadStatus)
  list(GET downloadStatus 0 error_code)
  list(GET downloadStatus 1 error_message)
  if(NOT error_code EQUAL 0)
    message(FATAL_ERROR "Failed downloading ${JAVA_MOCKITO_JAR}: ${error_message}")
  endif()
  file(RENAME ${JAVA_TMP_JAR} ${JAVA_MOCKITO_JAR})
endif()
if(NOT EXISTS ${JAVA_CGLIB_JAR})
  message("Downloading ${JAVA_CGLIB_JAR}")
  file(DOWNLOAD ${DEPS_URL}/cglib-${JAVA_CGLIB_VERSION}.jar ${JAVA_TMP_JAR} STATUS downloadStatus)
  list(GET downloadStatus 0 error_code)
  list(GET downloadStatus 1 error_message)
  if(NOT error_code EQUAL 0)
    message(FATAL_ERROR "Failed downloading ${JAVA_CGLIB_JAR}: ${error_message}")
  endif()
  file(RENAME ${JAVA_TMP_JAR} ${JAVA_CGLIB_JAR})
endif()
if(NOT EXISTS ${JAVA_ASSERTJ_JAR})
  message("Downloading ${JAVA_ASSERTJ_JAR}")
  file(DOWNLOAD ${DEPS_URL}/assertj-core-${JAVA_ASSERTJ_VERSION}.jar ${JAVA_TMP_JAR} STATUS downloadStatus)
  list(GET downloadStatus 0 error_code)
  list(GET downloadStatus 1 error_message)
  if(NOT error_code EQUAL 0)
    message(FATAL_ERROR "Failed downloading ${JAVA_ASSERTJ_JAR}: ${error_message}")
  endif()
  file(RENAME ${JAVA_TMP_JAR} ${JAVA_ASSERTJ_JAR})
endif()

if(${CMAKE_VERSION} VERSION_LESS "3.11.4")
  # Old CMake ONLY generate JNI headers, otherwise JNI is handled in add_jar step above
  message("Preparing JNI headers for old CMake (${CMAKE_VERSION})")
  set(NATIVE_JAVA_CLASSES
          io.github.fluss_contrib.rocksdb.AbstractCompactionFilter
          io.github.fluss_contrib.rocksdb.AbstractCompactionFilterFactory
          io.github.fluss_contrib.rocksdb.AbstractComparator
          io.github.fluss_contrib.rocksdb.AbstractEventListener
          io.github.fluss_contrib.rocksdb.AbstractImmutableNativeReference
          io.github.fluss_contrib.rocksdb.AbstractNativeReference
          io.github.fluss_contrib.rocksdb.AbstractRocksIterator
          io.github.fluss_contrib.rocksdb.AbstractSlice
          io.github.fluss_contrib.rocksdb.AbstractTableFilter
          io.github.fluss_contrib.rocksdb.AbstractTraceWriter
          io.github.fluss_contrib.rocksdb.AbstractTransactionNotifier
          io.github.fluss_contrib.rocksdb.AbstractWalFilter
          io.github.fluss_contrib.rocksdb.BackupEngineOptions
          io.github.fluss_contrib.rocksdb.BackupEngine
          io.github.fluss_contrib.rocksdb.BlockBasedTableConfig
          io.github.fluss_contrib.rocksdb.BloomFilter
          io.github.fluss_contrib.rocksdb.CassandraCompactionFilter
          io.github.fluss_contrib.rocksdb.CassandraValueMergeOperator
          io.github.fluss_contrib.rocksdb.Checkpoint
          io.github.fluss_contrib.rocksdb.ClockCache
          io.github.fluss_contrib.rocksdb.Cache
          io.github.fluss_contrib.rocksdb.ColumnFamilyHandle
          io.github.fluss_contrib.rocksdb.ColumnFamilyOptions
          io.github.fluss_contrib.rocksdb.CompactionJobInfo
          io.github.fluss_contrib.rocksdb.CompactionJobStats
          io.github.fluss_contrib.rocksdb.CompactionOptions
          io.github.fluss_contrib.rocksdb.CompactionOptionsFIFO
          io.github.fluss_contrib.rocksdb.CompactionOptionsUniversal
          io.github.fluss_contrib.rocksdb.CompactRangeOptions
          io.github.fluss_contrib.rocksdb.ComparatorOptions
          io.github.fluss_contrib.rocksdb.CompressionOptions
          io.github.fluss_contrib.rocksdb.ConcurrentTaskLimiterImpl
          io.github.fluss_contrib.rocksdb.ConfigOptions
          io.github.fluss_contrib.rocksdb.DBOptions
          io.github.fluss_contrib.rocksdb.DirectSlice
          io.github.fluss_contrib.rocksdb.Env
          io.github.fluss_contrib.rocksdb.EnvOptions
          io.github.fluss_contrib.rocksdb.Filter
          io.github.fluss_contrib.rocksdb.FloorSetCompactionFilter
          io.github.fluss_contrib.rocksdb.FlushOptions
          io.github.fluss_contrib.rocksdb.HashLinkedListMemTableConfig
          io.github.fluss_contrib.rocksdb.HashSkipListMemTableConfig
          io.github.fluss_contrib.rocksdb.HyperClockCache
          io.github.fluss_contrib.rocksdb.IngestExternalFileOptions
          io.github.fluss_contrib.rocksdb.Logger
          io.github.fluss_contrib.rocksdb.LRUCache
          io.github.fluss_contrib.rocksdb.MemoryUtil
          io.github.fluss_contrib.rocksdb.MemTableConfig
          io.github.fluss_contrib.rocksdb.NativeComparatorWrapper
          io.github.fluss_contrib.rocksdb.NativeLibraryLoader
          io.github.fluss_contrib.rocksdb.OptimisticTransactionDB
          io.github.fluss_contrib.rocksdb.OptimisticTransactionOptions
          io.github.fluss_contrib.rocksdb.Options
          io.github.fluss_contrib.rocksdb.OptionsUtil
          io.github.fluss_contrib.rocksdb.PersistentCache
          io.github.fluss_contrib.rocksdb.PlainTableConfig
          io.github.fluss_contrib.rocksdb.RateLimiter
          io.github.fluss_contrib.rocksdb.ReadOptions
          io.github.fluss_contrib.rocksdb.RemoveEmptyValueCompactionFilter
          io.github.fluss_contrib.rocksdb.RestoreOptions
          io.github.fluss_contrib.rocksdb.RocksCallbackObject
          io.github.fluss_contrib.rocksdb.RocksDB
          io.github.fluss_contrib.rocksdb.RocksEnv
          io.github.fluss_contrib.rocksdb.RocksIterator
          io.github.fluss_contrib.rocksdb.RocksIteratorInterface
          io.github.fluss_contrib.rocksdb.RocksMemEnv
          io.github.fluss_contrib.rocksdb.RocksMutableObject
          io.github.fluss_contrib.rocksdb.RocksObject
          io.github.fluss_contrib.rocksdb.SkipListMemTableConfig
          io.github.fluss_contrib.rocksdb.Slice
          io.github.fluss_contrib.rocksdb.Snapshot
          io.github.fluss_contrib.rocksdb.SstFileManager
          io.github.fluss_contrib.rocksdb.SstFileWriter
          io.github.fluss_contrib.rocksdb.SstFileReader
          io.github.fluss_contrib.rocksdb.SstFileReaderIterator
          io.github.fluss_contrib.rocksdb.SstPartitionerFactory
          io.github.fluss_contrib.rocksdb.SstPartitionerFixedPrefixFactory
          io.github.fluss_contrib.rocksdb.Statistics
          io.github.fluss_contrib.rocksdb.StringAppendOperator
          io.github.fluss_contrib.rocksdb.TableFormatConfig
          io.github.fluss_contrib.rocksdb.ThreadStatus
          io.github.fluss_contrib.rocksdb.TimedEnv
          io.github.fluss_contrib.rocksdb.Transaction
          io.github.fluss_contrib.rocksdb.TransactionDB
          io.github.fluss_contrib.rocksdb.TransactionDBOptions
          io.github.fluss_contrib.rocksdb.TransactionLogIterator
          io.github.fluss_contrib.rocksdb.TransactionOptions
          io.github.fluss_contrib.rocksdb.TtlDB
          io.github.fluss_contrib.rocksdb.UInt64AddOperator
          io.github.fluss_contrib.rocksdb.VectorMemTableConfig
          io.github.fluss_contrib.rocksdb.WBWIRocksIterator
          io.github.fluss_contrib.rocksdb.WriteBatch
          io.github.fluss_contrib.rocksdb.WriteBatch.Handler
          io.github.fluss_contrib.rocksdb.WriteBatchInterface
          io.github.fluss_contrib.rocksdb.WriteBatchWithIndex
          io.github.fluss_contrib.rocksdb.WriteOptions
          io.github.fluss_contrib.rocksdb.NativeComparatorWrapperTest
          io.github.fluss_contrib.rocksdb.RocksDBExceptionTest
          io.github.fluss_contrib.rocksdb.SnapshotTest
          io.github.fluss_contrib.rocksdb.WriteBatchTest
          io.github.fluss_contrib.rocksdb.WriteBatchTestInternalHelper
          io.github.fluss_contrib.rocksdb.WriteBufferManager
          io.github.fluss_contrib.rocksdb.test.TestableEventListener
  )

  create_javah(
    TARGET rocksdbjni_headers
    CLASSES ${NATIVE_JAVA_CLASSES}
    CLASSPATH rocksdbjni_classes ${JAVA_TESTCLASSPATH}
    OUTPUT_DIR ${JNI_OUTPUT_DIR}
  )
endif()

if(NOT MSVC)
  set_property(TARGET ${ROCKSDB_STATIC_LIB} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

set(ROCKSDBJNI_STATIC_LIB rocksdbjni${ARTIFACT_SUFFIX})
if(MINGW)
  # Build mingw only as staic library.
  add_library(${ROCKSDBJNI_STATIC_LIB} ${JNI_NATIVE_SOURCES})
else()
  add_library(${ROCKSDBJNI_STATIC_LIB} SHARED ${JNI_NATIVE_SOURCES})
endif()
add_dependencies(${ROCKSDBJNI_STATIC_LIB} rocksdbjni_headers)
target_link_libraries(${ROCKSDBJNI_STATIC_LIB} ${ROCKSDB_STATIC_LIB} ${ROCKSDB_LIB})

if(NOT MINGW)
  set(ROCKSDBJNI_SHARED_LIB rocksdbjni-shared${ARTIFACT_SUFFIX})
  add_library(${ROCKSDBJNI_SHARED_LIB} SHARED ${JNI_NATIVE_SOURCES})
  add_dependencies(${ROCKSDBJNI_SHARED_LIB} rocksdbjni_headers)
  target_link_libraries(${ROCKSDBJNI_SHARED_LIB} ${ROCKSDB_STATIC_LIB} ${ROCKSDB_LIB})

  set_target_properties(
    ${ROCKSDBJNI_SHARED_LIB}
    PROPERTIES
    COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CFG_INTDIR}
    COMPILE_PDB_NAME ${ROCKSDBJNI_STATIC_LIB}.pdb
  )
endif()

# Javadoc Jar
set(ROCKSDB_JAVADOC_JAR rocksdbjni-${CMAKE_PROJECT_VERSION}-javadoc.jar)
create_javadoc(rocksdb
        PACKAGES io.github.fluss_contrib.rocksdb io.github.fluss_contrib.rocksdb.util
        SOURCEPATH "${PROJECT_SOURCE_DIR}/java/src/main/java"
        WINDOWTITLE "RocksDB Java API JavaDoc"
        AUTHOR FALSE
        USE FALSE
        VERSION TRUE
)
add_custom_target(rocksdb_javadocs_jar ALL
  COMMAND ${Java_JAR_EXECUTABLE} cvf ${CMAKE_CURRENT_BINARY_DIR}/${ROCKSDB_JAVADOC_JAR} -C ${CMAKE_CURRENT_BINARY_DIR}/javadoc/rocksdb .
  BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/${ROCKSDB_JAVADOC_JAR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS rocksdb_javadoc
)

# Sources Jar
set(ROCKSDB_SOURCES_JAR rocksdbjni-${CMAKE_PROJECT_VERSION}-sources.jar)
add_custom_target(rocksdb_sources_jar ALL
        ${Java_JAR_EXECUTABLE} cvf ${CMAKE_CURRENT_BINARY_DIR}/${ROCKSDB_SOURCES_JAR} -C ${PROJECT_SOURCE_DIR}/java/src/main/java/ .
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/${ROCKSDB_SOURCES_JAR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(bitness 64)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_target_properties(
            ${ROCKSDBJNI_STATIC_LIB}
            PROPERTIES
            OUTPUT_NAME libflussrocksdbjni-win${bitness}
    )
    set(ROCKSDB_JAR rocksdbjni-${CMAKE_PROJECT_VERSION}-win${bitness}.jar)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set_target_properties(
            ${ROCKSDBJNI_STATIC_LIB}
            PROPERTIES
            OUTPUT_NAME "rocksdbjni-linux${bitness}"
    )
    set(ROCKSDB_JAR rocksdbjni-${CMAKE_PROJECT_VERSION}-linux${bitness}.jar)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(
            ${ROCKSDBJNI_STATIC_LIB}
            PROPERTIES
            OUTPUT_NAME rocksdbjni-osx-${CMAKE_SYSTEM_PROCESSOR}
    )
    set_target_properties(
            ${ROCKSDBJNI_STATIC_LIB}
            PROPERTIES
            SUFFIX ".jnilib"
    )
    set(ROCKSDB_JAR rocksdbjni-${CMAKE_PROJECT_VERSION}-osx.jar)
else()
    set(ROCKSDB_JAR rocksdb-${CMAKE_PROJECT_VERSION}.jar)
endif()

get_target_property(ROCKS_JAR_FILE rocksdbjni_classes JAR_FILE)

add_custom_target(rocksdbjava ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${ROCKS_JAR_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${ROCKSDB_JAR}
        COMMAND ${Java_JAR_EXECUTABLE} -uf ${CMAKE_CURRENT_BINARY_DIR}/${ROCKSDB_JAR} -C $<TARGET_FILE_DIR:${ROCKSDBJNI_STATIC_LIB}> $<TARGET_FILE_NAME:${ROCKSDBJNI_STATIC_LIB}>
        COMMAND ${Java_JAR_EXECUTABLE} -uf ${CMAKE_CURRENT_BINARY_DIR}/${ROCKSDB_JAR} -C ${CMAKE_CURRENT_SOURCE_DIR} HISTORY-JAVA.md
        DEPENDS ${ROCKSDBJNI_STATIC_LIB} rocksdbjni_classes
        BYPRODUCTS ${ROCKSDB_JAR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )

enable_testing()
get_target_property(ROCKSDBJNI_CLASSES_TEST_JAR_FILE rocksdbjni_test_classes JAR_FILE)
foreach (CLAZZ ${JAVA_TEST_RUNNING_CLASSES})
  if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_test(
      NAME jtest_${CLAZZ}
            COMMAND ${Java_JAVA_EXECUTABLE} ${JVMARGS} -ea -Xcheck:jni -Djava.library.path=${PROJECT_BINARY_DIR}/java/${CMAKE_BUILD_TYPE} -classpath ${CMAKE_CURRENT_BINARY_DIR}/${ROCKSDB_JAR}$<SEMICOLON>${JAVA_RUN_TESTCLASSPATH}$<SEMICOLON>${ROCKSDBJNI_CLASSES_TEST_JAR_FILE} io.github.fluss_contrib.rocksdb.test.RocksJunitRunner ${CLAZZ}
    )
  else()
    add_test(
      NAME jtest_${CLAZZ}
            COMMAND ${Java_JAVA_EXECUTABLE} ${JVMARGS} -ea -Xcheck:jni -Djava.library.path=${PROJECT_BINARY_DIR}/java -classpath ${JAVA_RUN_TESTCLASSPATH}:${ROCKSDBJNI_CLASSES_TEST_JAR_FILE}:${CMAKE_CURRENT_BINARY_DIR}/${ROCKSDB_JAR} io.github.fluss_contrib.rocksdb.test.RocksJunitRunner ${CLAZZ}
    )
  endif()
endforeach(CLAZZ)
