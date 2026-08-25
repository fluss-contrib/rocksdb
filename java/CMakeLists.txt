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
  src/main/java/org/fluss/rocksdb/AbstractCompactionFilter.java
  src/main/java/org/fluss/rocksdb/AbstractCompactionFilterFactory.java
  src/main/java/org/fluss/rocksdb/AbstractComparator.java
  src/main/java/org/fluss/rocksdb/AbstractComparatorJniBridge.java
  src/main/java/org/fluss/rocksdb/AbstractEventListener.java
  src/main/java/org/fluss/rocksdb/AbstractImmutableNativeReference.java
  src/main/java/org/fluss/rocksdb/AbstractMutableOptions.java
  src/main/java/org/fluss/rocksdb/AbstractNativeReference.java
  src/main/java/org/fluss/rocksdb/AbstractRocksIterator.java
  src/main/java/org/fluss/rocksdb/AbstractSlice.java
  src/main/java/org/fluss/rocksdb/AbstractTableFilter.java
  src/main/java/org/fluss/rocksdb/AbstractTraceWriter.java
  src/main/java/org/fluss/rocksdb/AbstractTransactionNotifier.java
  src/main/java/org/fluss/rocksdb/AbstractWalFilter.java
  src/main/java/org/fluss/rocksdb/AbstractWriteBatch.java
  src/main/java/org/fluss/rocksdb/AdvancedColumnFamilyOptionsInterface.java
  src/main/java/org/fluss/rocksdb/AdvancedMutableColumnFamilyOptionsInterface.java
  src/main/java/org/fluss/rocksdb/BackgroundErrorReason.java
  src/main/java/org/fluss/rocksdb/BackupEngine.java
  src/main/java/org/fluss/rocksdb/BackupEngineOptions.java
  src/main/java/org/fluss/rocksdb/BackupInfo.java
  src/main/java/org/fluss/rocksdb/BlockBasedTableConfig.java
  src/main/java/org/fluss/rocksdb/BloomFilter.java
  src/main/java/org/fluss/rocksdb/BuiltinComparator.java
  src/main/java/org/fluss/rocksdb/ByteBufferGetStatus.java
  src/main/java/org/fluss/rocksdb/Cache.java
  src/main/java/org/fluss/rocksdb/CassandraCompactionFilter.java
  src/main/java/org/fluss/rocksdb/CassandraValueMergeOperator.java
  src/main/java/org/fluss/rocksdb/Checkpoint.java
  src/main/java/org/fluss/rocksdb/ChecksumType.java
  src/main/java/org/fluss/rocksdb/ClockCache.java
  src/main/java/org/fluss/rocksdb/ColumnFamilyDescriptor.java
  src/main/java/org/fluss/rocksdb/ColumnFamilyHandle.java
  src/main/java/org/fluss/rocksdb/ColumnFamilyMetaData.java
  src/main/java/org/fluss/rocksdb/ColumnFamilyOptions.java
  src/main/java/org/fluss/rocksdb/ColumnFamilyOptionsInterface.java
  src/main/java/org/fluss/rocksdb/CompactRangeOptions.java
  src/main/java/org/fluss/rocksdb/CompactionJobInfo.java
  src/main/java/org/fluss/rocksdb/CompactionJobStats.java
  src/main/java/org/fluss/rocksdb/CompactionOptions.java
  src/main/java/org/fluss/rocksdb/CompactionOptionsFIFO.java
  src/main/java/org/fluss/rocksdb/CompactionOptionsUniversal.java
  src/main/java/org/fluss/rocksdb/CompactionPriority.java
  src/main/java/org/fluss/rocksdb/CompactionReason.java
  src/main/java/org/fluss/rocksdb/CompactionStopStyle.java
  src/main/java/org/fluss/rocksdb/CompactionStyle.java
  src/main/java/org/fluss/rocksdb/ComparatorOptions.java
  src/main/java/org/fluss/rocksdb/ComparatorType.java
  src/main/java/org/fluss/rocksdb/CompressionOptions.java
  src/main/java/org/fluss/rocksdb/CompressionType.java
  src/main/java/org/fluss/rocksdb/ConcurrentTaskLimiter.java
  src/main/java/org/fluss/rocksdb/ConcurrentTaskLimiterImpl.java
  src/main/java/org/fluss/rocksdb/ConfigOptions.java
  src/main/java/org/fluss/rocksdb/DBOptions.java
  src/main/java/org/fluss/rocksdb/DBOptionsInterface.java
  src/main/java/org/fluss/rocksdb/DataBlockIndexType.java
  src/main/java/org/fluss/rocksdb/DbPath.java
  src/main/java/org/fluss/rocksdb/DirectSlice.java
  src/main/java/org/fluss/rocksdb/EncodingType.java
  src/main/java/org/fluss/rocksdb/Env.java
  src/main/java/org/fluss/rocksdb/EnvOptions.java
  src/main/java/org/fluss/rocksdb/EventListener.java
  src/main/java/org/fluss/rocksdb/Experimental.java
  src/main/java/org/fluss/rocksdb/ExportImportFilesMetaData.java
  src/main/java/org/fluss/rocksdb/ExternalFileIngestionInfo.java
  src/main/java/org/fluss/rocksdb/FileOperationInfo.java
  src/main/java/org/fluss/rocksdb/Filter.java
  src/main/java/org/fluss/rocksdb/FilterPolicyType.java
  src/main/java/org/fluss/rocksdb/FlushJobInfo.java
  src/main/java/org/fluss/rocksdb/FlushOptions.java
  src/main/java/org/fluss/rocksdb/FlushReason.java
  src/main/java/org/fluss/rocksdb/GetStatus.java
  src/main/java/org/fluss/rocksdb/HashLinkedListMemTableConfig.java
  src/main/java/org/fluss/rocksdb/HashSkipListMemTableConfig.java
  src/main/java/org/fluss/rocksdb/HistogramData.java
  src/main/java/org/fluss/rocksdb/HistogramType.java
  src/main/java/org/fluss/rocksdb/Holder.java
  src/main/java/org/fluss/rocksdb/HyperClockCache.java
  src/main/java/org/fluss/rocksdb/ImportColumnFamilyOptions.java
  src/main/java/org/fluss/rocksdb/IndexShorteningMode.java
  src/main/java/org/fluss/rocksdb/IndexType.java
  src/main/java/org/fluss/rocksdb/InfoLogLevel.java
  src/main/java/org/fluss/rocksdb/IngestExternalFileOptions.java
  src/main/java/org/fluss/rocksdb/KeyMayExist.java
  src/main/java/org/fluss/rocksdb/LRUCache.java
  src/main/java/org/fluss/rocksdb/LevelMetaData.java
  src/main/java/org/fluss/rocksdb/LiveFileMetaData.java
  src/main/java/org/fluss/rocksdb/LogFile.java
  src/main/java/org/fluss/rocksdb/Logger.java
  src/main/java/org/fluss/rocksdb/LoggerInterface.java
  src/main/java/org/fluss/rocksdb/LoggerType.java
  src/main/java/org/fluss/rocksdb/MemTableConfig.java
  src/main/java/org/fluss/rocksdb/MemTableInfo.java
  src/main/java/org/fluss/rocksdb/MemoryUsageType.java
  src/main/java/org/fluss/rocksdb/MemoryUtil.java
  src/main/java/org/fluss/rocksdb/MergeOperator.java
  src/main/java/org/fluss/rocksdb/MutableColumnFamilyOptions.java
  src/main/java/org/fluss/rocksdb/MutableColumnFamilyOptionsInterface.java
  src/main/java/org/fluss/rocksdb/MutableDBOptions.java
  src/main/java/org/fluss/rocksdb/MutableDBOptionsInterface.java
  src/main/java/org/fluss/rocksdb/MutableOptionKey.java
  src/main/java/org/fluss/rocksdb/MutableOptionValue.java
  src/main/java/org/fluss/rocksdb/NativeComparatorWrapper.java
  src/main/java/org/fluss/rocksdb/NativeLibraryLoader.java
  src/main/java/org/fluss/rocksdb/OperationStage.java
  src/main/java/org/fluss/rocksdb/OperationType.java
  src/main/java/org/fluss/rocksdb/OptimisticTransactionDB.java
  src/main/java/org/fluss/rocksdb/OptimisticTransactionOptions.java
  src/main/java/org/fluss/rocksdb/OptionString.java
  src/main/java/org/fluss/rocksdb/Options.java
  src/main/java/org/fluss/rocksdb/OptionsUtil.java
  src/main/java/org/fluss/rocksdb/PerfContext.java
  src/main/java/org/fluss/rocksdb/PerfLevel.java
  src/main/java/org/fluss/rocksdb/PersistentCache.java
  src/main/java/org/fluss/rocksdb/PlainTableConfig.java
  src/main/java/org/fluss/rocksdb/PrepopulateBlobCache.java
  src/main/java/org/fluss/rocksdb/Priority.java
  src/main/java/org/fluss/rocksdb/Range.java
  src/main/java/org/fluss/rocksdb/RateLimiter.java
  src/main/java/org/fluss/rocksdb/RateLimiterMode.java
  src/main/java/org/fluss/rocksdb/ReadOptions.java
  src/main/java/org/fluss/rocksdb/ReadTier.java
  src/main/java/org/fluss/rocksdb/RemoveEmptyValueCompactionFilter.java
  src/main/java/org/fluss/rocksdb/RestoreOptions.java
  src/main/java/org/fluss/rocksdb/ReusedSynchronisationType.java
  src/main/java/org/fluss/rocksdb/RocksCallbackObject.java
  src/main/java/org/fluss/rocksdb/RocksDB.java
  src/main/java/org/fluss/rocksdb/RocksDBException.java
  src/main/java/org/fluss/rocksdb/RocksEnv.java
  src/main/java/org/fluss/rocksdb/RocksIterator.java
  src/main/java/org/fluss/rocksdb/RocksIteratorInterface.java
  src/main/java/org/fluss/rocksdb/RocksMemEnv.java
  src/main/java/org/fluss/rocksdb/RocksMutableObject.java
  src/main/java/org/fluss/rocksdb/RocksObject.java
  src/main/java/org/fluss/rocksdb/SanityLevel.java
  src/main/java/org/fluss/rocksdb/SizeApproximationFlag.java
  src/main/java/org/fluss/rocksdb/SkipListMemTableConfig.java
  src/main/java/org/fluss/rocksdb/Slice.java
  src/main/java/org/fluss/rocksdb/Snapshot.java
  src/main/java/org/fluss/rocksdb/SstFileManager.java
  src/main/java/org/fluss/rocksdb/SstFileMetaData.java
  src/main/java/org/fluss/rocksdb/SstFileReader.java
  src/main/java/org/fluss/rocksdb/SstFileReaderIterator.java
  src/main/java/org/fluss/rocksdb/SstFileWriter.java
  src/main/java/org/fluss/rocksdb/SstPartitionerFactory.java
  src/main/java/org/fluss/rocksdb/SstPartitionerFixedPrefixFactory.java
  src/main/java/org/fluss/rocksdb/StateType.java
  src/main/java/org/fluss/rocksdb/Statistics.java
  src/main/java/org/fluss/rocksdb/StatisticsCollector.java
  src/main/java/org/fluss/rocksdb/StatisticsCollectorCallback.java
  src/main/java/org/fluss/rocksdb/StatsCollectorInput.java
  src/main/java/org/fluss/rocksdb/StatsLevel.java
  src/main/java/org/fluss/rocksdb/Status.java
  src/main/java/org/fluss/rocksdb/StringAppendOperator.java
  src/main/java/org/fluss/rocksdb/TableFileCreationBriefInfo.java
  src/main/java/org/fluss/rocksdb/TableFileCreationInfo.java
  src/main/java/org/fluss/rocksdb/TableFileCreationReason.java
  src/main/java/org/fluss/rocksdb/TableFileDeletionInfo.java
  src/main/java/org/fluss/rocksdb/TableFilter.java
  src/main/java/org/fluss/rocksdb/TableFormatConfig.java
  src/main/java/org/fluss/rocksdb/TableProperties.java
  src/main/java/org/fluss/rocksdb/TablePropertiesCollectorFactory.java
  src/main/java/org/fluss/rocksdb/ThreadStatus.java
  src/main/java/org/fluss/rocksdb/ThreadType.java
  src/main/java/org/fluss/rocksdb/TickerType.java
  src/main/java/org/fluss/rocksdb/TimedEnv.java
  src/main/java/org/fluss/rocksdb/TraceOptions.java
  src/main/java/org/fluss/rocksdb/TraceWriter.java
  src/main/java/org/fluss/rocksdb/Transaction.java
  src/main/java/org/fluss/rocksdb/TransactionDB.java
  src/main/java/org/fluss/rocksdb/TransactionDBOptions.java
  src/main/java/org/fluss/rocksdb/TransactionLogIterator.java
  src/main/java/org/fluss/rocksdb/TransactionOptions.java
  src/main/java/org/fluss/rocksdb/TransactionalDB.java
  src/main/java/org/fluss/rocksdb/TransactionalOptions.java
  src/main/java/org/fluss/rocksdb/TtlDB.java
  src/main/java/org/fluss/rocksdb/TxnDBWritePolicy.java
  src/main/java/org/fluss/rocksdb/UInt64AddOperator.java
  src/main/java/org/fluss/rocksdb/VectorMemTableConfig.java
  src/main/java/org/fluss/rocksdb/WALRecoveryMode.java
  src/main/java/org/fluss/rocksdb/WBWIRocksIterator.java
  src/main/java/org/fluss/rocksdb/WalFileType.java
  src/main/java/org/fluss/rocksdb/WalFilter.java
  src/main/java/org/fluss/rocksdb/WalProcessingOption.java
  src/main/java/org/fluss/rocksdb/WriteBatch.java
  src/main/java/org/fluss/rocksdb/WriteBatchInterface.java
  src/main/java/org/fluss/rocksdb/WriteBatchWithIndex.java
  src/main/java/org/fluss/rocksdb/WriteBufferManager.java
  src/main/java/org/fluss/rocksdb/WriteOptions.java
  src/main/java/org/fluss/rocksdb/WriteStallCondition.java
  src/main/java/org/fluss/rocksdb/WriteStallInfo.java
  src/main/java/org/fluss/rocksdb/util/BufferUtil.java
  src/main/java/org/fluss/rocksdb/util/ByteUtil.java
  src/main/java/org/fluss/rocksdb/util/BytewiseComparator.java
  src/main/java/org/fluss/rocksdb/util/Environment.java
  src/main/java/org/fluss/rocksdb/util/IntComparator.java
  src/main/java/org/fluss/rocksdb/util/ReverseBytewiseComparator.java
  src/main/java/org/fluss/rocksdb/util/SizeUnit.java
  src/main/java/org/fluss/rocksdb/util/StdErrLogger.java
  src/test/java/org/fluss/rocksdb/NativeComparatorWrapperTest.java
  src/test/java/org/fluss/rocksdb/RocksDBExceptionTest.java
  src/test/java/org/fluss/rocksdb/RocksNativeLibraryResource.java
  src/test/java/org/fluss/rocksdb/WriteBatchTest.java
  src/test/java/org/fluss/rocksdb/test/TestableEventListener.java
  src/test/java/org/fluss/rocksdb/util/CapturingWriteBatchHandler.java
  src/test/java/org/fluss/rocksdb/util/WriteBatchGetter.java
)

set(JAVA_TEST_CLASSES
  src/test/java/org/fluss/rocksdb/AbstractTransactionTest.java
  src/test/java/org/fluss/rocksdb/BackupEngineOptionsTest.java
  src/test/java/org/fluss/rocksdb/BackupEngineTest.java
  src/test/java/org/fluss/rocksdb/BlobOptionsTest.java
  src/test/java/org/fluss/rocksdb/BlockBasedTableConfigTest.java
  src/test/java/org/fluss/rocksdb/BuiltinComparatorTest.java
  src/test/java/org/fluss/rocksdb/ByteBufferUnsupportedOperationTest.java
  src/test/java/org/fluss/rocksdb/BytewiseComparatorRegressionTest.java
  src/test/java/org/fluss/rocksdb/CheckPointTest.java
  src/test/java/org/fluss/rocksdb/ClockCacheTest.java
  src/test/java/org/fluss/rocksdb/ColumnFamilyOptionsTest.java
  src/test/java/org/fluss/rocksdb/ColumnFamilyTest.java
  src/test/java/org/fluss/rocksdb/CompactRangeOptionsTest.java
  src/test/java/org/fluss/rocksdb/CompactionFilterFactoryTest.java
  src/test/java/org/fluss/rocksdb/CompactionJobInfoTest.java
  src/test/java/org/fluss/rocksdb/CompactionJobStatsTest.java
  src/test/java/org/fluss/rocksdb/CompactionOptionsFIFOTest.java
  src/test/java/org/fluss/rocksdb/CompactionOptionsTest.java
  src/test/java/org/fluss/rocksdb/CompactionOptionsUniversalTest.java
  src/test/java/org/fluss/rocksdb/CompactionPriorityTest.java
  src/test/java/org/fluss/rocksdb/CompactionStopStyleTest.java
  src/test/java/org/fluss/rocksdb/ComparatorOptionsTest.java
  src/test/java/org/fluss/rocksdb/CompressionOptionsTest.java
  src/test/java/org/fluss/rocksdb/CompressionTypesTest.java
  src/test/java/org/fluss/rocksdb/ConcurrentTaskLimiterTest.java
  src/test/java/org/fluss/rocksdb/DBOptionsTest.java
  src/test/java/org/fluss/rocksdb/DefaultEnvTest.java
  src/test/java/org/fluss/rocksdb/DirectSliceTest.java
  src/test/java/org/fluss/rocksdb/EnvOptionsTest.java
  src/test/java/org/fluss/rocksdb/EventListenerTest.java
  src/test/java/org/fluss/rocksdb/FilterTest.java
  src/test/java/org/fluss/rocksdb/FlushOptionsTest.java
  src/test/java/org/fluss/rocksdb/FlushTest.java
  src/test/java/org/fluss/rocksdb/HyperClockCacheTest.java
  src/test/java/org/fluss/rocksdb/ImportColumnFamilyTest.java
  src/test/java/org/fluss/rocksdb/InfoLogLevelTest.java
  src/test/java/org/fluss/rocksdb/IngestExternalFileOptionsTest.java
  src/test/java/org/fluss/rocksdb/KeyExistsTest.java
  src/test/java/org/fluss/rocksdb/KeyMayExistTest.java
  src/test/java/org/fluss/rocksdb/LRUCacheTest.java
  src/test/java/org/fluss/rocksdb/LoggerTest.java
  src/test/java/org/fluss/rocksdb/MemTableTest.java
  src/test/java/org/fluss/rocksdb/MemoryUtilTest.java
  src/test/java/org/fluss/rocksdb/MergeCFVariantsTest.java
  src/test/java/org/fluss/rocksdb/MergeTest.java
  src/test/java/org/fluss/rocksdb/MergeVariantsTest.java
  src/test/java/org/fluss/rocksdb/MixedOptionsTest.java
  src/test/java/org/fluss/rocksdb/MultiColumnRegressionTest.java
  src/test/java/org/fluss/rocksdb/MultiGetManyKeysTest.java
  src/test/java/org/fluss/rocksdb/MultiGetTest.java
  src/test/java/org/fluss/rocksdb/MutableColumnFamilyOptionsTest.java
  src/test/java/org/fluss/rocksdb/MutableDBOptionsTest.java
  src/test/java/org/fluss/rocksdb/MutableOptionsGetSetTest.java
  src/test/java/org/fluss/rocksdb/NativeLibraryLoaderTest.java
  src/test/java/org/fluss/rocksdb/OptimisticTransactionDBTest.java
  src/test/java/org/fluss/rocksdb/OptimisticTransactionOptionsTest.java
  src/test/java/org/fluss/rocksdb/OptimisticTransactionTest.java
  src/test/java/org/fluss/rocksdb/OptionsTest.java
  src/test/java/org/fluss/rocksdb/OptionsUtilTest.java
  src/test/java/org/fluss/rocksdb/PerfContextTest.java
  src/test/java/org/fluss/rocksdb/PerfLevelTest.java
  src/test/java/org/fluss/rocksdb/PlainTableConfigTest.java
  src/test/java/org/fluss/rocksdb/PlatformRandomHelper.java
  src/test/java/org/fluss/rocksdb/PutCFVariantsTest.java
  src/test/java/org/fluss/rocksdb/PutMultiplePartsTest.java
  src/test/java/org/fluss/rocksdb/PutVariantsTest.java
  src/test/java/org/fluss/rocksdb/RateLimiterTest.java
  src/test/java/org/fluss/rocksdb/ReadOnlyTest.java
  src/test/java/org/fluss/rocksdb/ReadOptionsTest.java
  src/test/java/org/fluss/rocksdb/RocksDBTest.java
  src/test/java/org/fluss/rocksdb/RocksIteratorTest.java
  src/test/java/org/fluss/rocksdb/RocksMemEnvTest.java
  src/test/java/org/fluss/rocksdb/SecondaryDBTest.java
  src/test/java/org/fluss/rocksdb/SliceTest.java
  src/test/java/org/fluss/rocksdb/SnapshotTest.java
  src/test/java/org/fluss/rocksdb/SstFileManagerTest.java
  src/test/java/org/fluss/rocksdb/SstFileReaderTest.java
  src/test/java/org/fluss/rocksdb/SstFileWriterTest.java
  src/test/java/org/fluss/rocksdb/SstPartitionerTest.java
  src/test/java/org/fluss/rocksdb/StatisticsCollectorTest.java
  src/test/java/org/fluss/rocksdb/StatisticsTest.java
  src/test/java/org/fluss/rocksdb/StatsCallbackMock.java
  src/test/java/org/fluss/rocksdb/TableFilterTest.java
  src/test/java/org/fluss/rocksdb/TimedEnvTest.java
  src/test/java/org/fluss/rocksdb/TransactionDBOptionsTest.java
  src/test/java/org/fluss/rocksdb/TransactionDBTest.java
  src/test/java/org/fluss/rocksdb/TransactionLogIteratorTest.java
  src/test/java/org/fluss/rocksdb/TransactionOptionsTest.java
  src/test/java/org/fluss/rocksdb/TransactionTest.java
  src/test/java/org/fluss/rocksdb/TtlDBTest.java
  src/test/java/org/fluss/rocksdb/Types.java
  src/test/java/org/fluss/rocksdb/VerifyChecksumsTest.java
  src/test/java/org/fluss/rocksdb/WALRecoveryModeTest.java
  src/test/java/org/fluss/rocksdb/WalFilterTest.java
  src/test/java/org/fluss/rocksdb/WriteBatchHandlerTest.java
  src/test/java/org/fluss/rocksdb/WriteBatchThreadedTest.java
  src/test/java/org/fluss/rocksdb/WriteBatchWithIndexTest.java
  src/test/java/org/fluss/rocksdb/WriteOptionsTest.java
  src/test/java/org/fluss/rocksdb/test/RemoveEmptyValueCompactionFilterFactory.java
  src/test/java/org/fluss/rocksdb/test/RemoveEmptyValueCompactionFilterFactory.java
  src/test/java/org/fluss/rocksdb/test/RemoveEmptyValueCompactionFilterFactory.java
  src/test/java/org/fluss/rocksdb/test/RocksJunitRunner.java
  src/test/java/org/fluss/rocksdb/test/TestableEventListener.java
  src/test/java/org/fluss/rocksdb/test/TestableEventListener.java
  src/test/java/org/fluss/rocksdb/util/ByteBufferAllocator.java
  src/test/java/org/fluss/rocksdb/util/BytewiseComparatorIntTest.java
  src/test/java/org/fluss/rocksdb/util/BytewiseComparatorTest.java
  src/test/java/org/fluss/rocksdb/util/DirectByteBufferAllocator.java
  src/test/java/org/fluss/rocksdb/util/EnvironmentTest.java
  src/test/java/org/fluss/rocksdb/util/HeapByteBufferAllocator.java
  src/test/java/org/fluss/rocksdb/util/IntComparatorTest.java
  src/test/java/org/fluss/rocksdb/util/JNIComparatorTest.java
  src/test/java/org/fluss/rocksdb/util/ReverseBytewiseComparatorIntTest.java
  src/test/java/org/fluss/rocksdb/util/SizeUnitTest.java
  src/test/java/org/fluss/rocksdb/util/StdErrLoggerTest.java
  src/test/java/org/fluss/rocksdb/util/TestUtil.java
)

set(JAVA_TEST_RUNNING_CLASSES
  org.fluss.rocksdb.BackupEngineOptionsTest
  org.fluss.rocksdb.BackupEngineTest
  org.fluss.rocksdb.BlobOptionsTest
  org.fluss.rocksdb.BlockBasedTableConfigTest
  org.fluss.rocksdb.BuiltinComparatorTest
  org.fluss.rocksdb.ByteBufferUnsupportedOperationTest
  org.fluss.rocksdb.BytewiseComparatorRegressionTest
  org.fluss.rocksdb.CheckPointTest
  org.fluss.rocksdb.ClockCacheTest
  org.fluss.rocksdb.ColumnFamilyOptionsTest
  org.fluss.rocksdb.ColumnFamilyTest
  org.fluss.rocksdb.CompactRangeOptionsTest
  org.fluss.rocksdb.CompactionFilterFactoryTest
  org.fluss.rocksdb.CompactionJobInfoTest
  org.fluss.rocksdb.CompactionJobStatsTest
  org.fluss.rocksdb.CompactionOptionsFIFOTest
  org.fluss.rocksdb.CompactionOptionsTest
  org.fluss.rocksdb.CompactionOptionsUniversalTest
  org.fluss.rocksdb.CompactionPriorityTest
  org.fluss.rocksdb.CompactionStopStyleTest
  org.fluss.rocksdb.ComparatorOptionsTest
  org.fluss.rocksdb.CompressionOptionsTest
  org.fluss.rocksdb.CompressionTypesTest
  org.fluss.rocksdb.ConcurrentTaskLimiterTest
  org.fluss.rocksdb.DBOptionsTest
  org.fluss.rocksdb.DefaultEnvTest
  org.fluss.rocksdb.DirectSliceTest
  org.fluss.rocksdb.EnvOptionsTest
  org.fluss.rocksdb.EventListenerTest
  org.fluss.rocksdb.FilterTest
  org.fluss.rocksdb.FlushOptionsTest
  org.fluss.rocksdb.FlushTest
  org.fluss.rocksdb.HyperClockCacheTest
  org.fluss.rocksdb.ImportColumnFamilyTest
  org.fluss.rocksdb.InfoLogLevelTest
  org.fluss.rocksdb.IngestExternalFileOptionsTest
  org.fluss.rocksdb.KeyExistsTest
  org.fluss.rocksdb.KeyMayExistTest
  org.fluss.rocksdb.LRUCacheTest
  org.fluss.rocksdb.LoggerTest
  org.fluss.rocksdb.MemTableTest
  org.fluss.rocksdb.MemoryUtilTest
  org.fluss.rocksdb.MergeCFVariantsTest
  org.fluss.rocksdb.MergeTest
  org.fluss.rocksdb.MergeVariantsTest
  org.fluss.rocksdb.MixedOptionsTest
  org.fluss.rocksdb.MultiColumnRegressionTest
  org.fluss.rocksdb.MultiGetManyKeysTest
  org.fluss.rocksdb.MultiGetTest
  org.fluss.rocksdb.MutableColumnFamilyOptionsTest
  org.fluss.rocksdb.MutableDBOptionsTest
  org.fluss.rocksdb.MutableOptionsGetSetTest
  org.fluss.rocksdb.NativeComparatorWrapperTest
  org.fluss.rocksdb.NativeLibraryLoaderTest
  org.fluss.rocksdb.OptimisticTransactionDBTest
  org.fluss.rocksdb.OptimisticTransactionOptionsTest
  org.fluss.rocksdb.OptimisticTransactionTest
  org.fluss.rocksdb.OptionsTest
  org.fluss.rocksdb.OptionsUtilTest
  org.fluss.rocksdb.PerfContextTest
  org.fluss.rocksdb.PerfLevelTest
  org.fluss.rocksdb.PlainTableConfigTest
  org.fluss.rocksdb.PutCFVariantsTest
  org.fluss.rocksdb.PutMultiplePartsTest
  org.fluss.rocksdb.PutVariantsTest
  org.fluss.rocksdb.RateLimiterTest
  org.fluss.rocksdb.ReadOnlyTest
  org.fluss.rocksdb.ReadOptionsTest
  org.fluss.rocksdb.RocksDBExceptionTest
  org.fluss.rocksdb.RocksDBTest
  org.fluss.rocksdb.RocksIteratorTest
  org.fluss.rocksdb.RocksMemEnvTest
  org.fluss.rocksdb.SecondaryDBTest
  org.fluss.rocksdb.SliceTest
  org.fluss.rocksdb.SnapshotTest
  org.fluss.rocksdb.SstFileManagerTest
  org.fluss.rocksdb.SstFileReaderTest
  org.fluss.rocksdb.SstFileWriterTest
  org.fluss.rocksdb.SstPartitionerTest
  org.fluss.rocksdb.StatisticsCollectorTest
  org.fluss.rocksdb.StatisticsTest
  org.fluss.rocksdb.TableFilterTest
  org.fluss.rocksdb.TimedEnvTest
  org.fluss.rocksdb.TransactionDBOptionsTest
  org.fluss.rocksdb.TransactionDBTest
  org.fluss.rocksdb.TransactionLogIteratorTest
  org.fluss.rocksdb.TransactionOptionsTest
  org.fluss.rocksdb.TransactionTest
  org.fluss.rocksdb.TtlDBTest
  org.fluss.rocksdb.VerifyChecksumsTest
  org.fluss.rocksdb.WALRecoveryModeTest
  org.fluss.rocksdb.WalFilterTest
  org.fluss.rocksdb.WriteBatchHandlerTest
  org.fluss.rocksdb.WriteBatchTest
  org.fluss.rocksdb.WriteBatchThreadedTest
  org.fluss.rocksdb.WriteBatchWithIndexTest
  org.fluss.rocksdb.WriteOptionsTest
  org.fluss.rocksdb.util.BytewiseComparatorIntTest
  org.fluss.rocksdb.util.BytewiseComparatorTest
  org.fluss.rocksdb.util.EnvironmentTest
  org.fluss.rocksdb.util.IntComparatorTest
  org.fluss.rocksdb.util.JNIComparatorTest
  org.fluss.rocksdb.util.ReverseBytewiseComparatorIntTest
  org.fluss.rocksdb.util.SizeUnitTest
  org.fluss.rocksdb.util.StdErrLoggerTest
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
          org.fluss.rocksdb.AbstractCompactionFilter
          org.fluss.rocksdb.AbstractCompactionFilterFactory
          org.fluss.rocksdb.AbstractComparator
          org.fluss.rocksdb.AbstractEventListener
          org.fluss.rocksdb.AbstractImmutableNativeReference
          org.fluss.rocksdb.AbstractNativeReference
          org.fluss.rocksdb.AbstractRocksIterator
          org.fluss.rocksdb.AbstractSlice
          org.fluss.rocksdb.AbstractTableFilter
          org.fluss.rocksdb.AbstractTraceWriter
          org.fluss.rocksdb.AbstractTransactionNotifier
          org.fluss.rocksdb.AbstractWalFilter
          org.fluss.rocksdb.BackupEngineOptions
          org.fluss.rocksdb.BackupEngine
          org.fluss.rocksdb.BlockBasedTableConfig
          org.fluss.rocksdb.BloomFilter
          org.fluss.rocksdb.CassandraCompactionFilter
          org.fluss.rocksdb.CassandraValueMergeOperator
          org.fluss.rocksdb.Checkpoint
          org.fluss.rocksdb.ClockCache
          org.fluss.rocksdb.Cache
          org.fluss.rocksdb.ColumnFamilyHandle
          org.fluss.rocksdb.ColumnFamilyOptions
          org.fluss.rocksdb.CompactionJobInfo
          org.fluss.rocksdb.CompactionJobStats
          org.fluss.rocksdb.CompactionOptions
          org.fluss.rocksdb.CompactionOptionsFIFO
          org.fluss.rocksdb.CompactionOptionsUniversal
          org.fluss.rocksdb.CompactRangeOptions
          org.fluss.rocksdb.ComparatorOptions
          org.fluss.rocksdb.CompressionOptions
          org.fluss.rocksdb.ConcurrentTaskLimiterImpl
          org.fluss.rocksdb.ConfigOptions
          org.fluss.rocksdb.DBOptions
          org.fluss.rocksdb.DirectSlice
          org.fluss.rocksdb.Env
          org.fluss.rocksdb.EnvOptions
          org.fluss.rocksdb.Filter
          org.fluss.rocksdb.FlushOptions
          org.fluss.rocksdb.HashLinkedListMemTableConfig
          org.fluss.rocksdb.HashSkipListMemTableConfig
          org.fluss.rocksdb.HyperClockCache
          org.fluss.rocksdb.IngestExternalFileOptions
          org.fluss.rocksdb.Logger
          org.fluss.rocksdb.LRUCache
          org.fluss.rocksdb.MemoryUtil
          org.fluss.rocksdb.MemTableConfig
          org.fluss.rocksdb.NativeComparatorWrapper
          org.fluss.rocksdb.NativeLibraryLoader
          org.fluss.rocksdb.OptimisticTransactionDB
          org.fluss.rocksdb.OptimisticTransactionOptions
          org.fluss.rocksdb.Options
          org.fluss.rocksdb.OptionsUtil
          org.fluss.rocksdb.PersistentCache
          org.fluss.rocksdb.PlainTableConfig
          org.fluss.rocksdb.RateLimiter
          org.fluss.rocksdb.ReadOptions
          org.fluss.rocksdb.RemoveEmptyValueCompactionFilter
          org.fluss.rocksdb.RestoreOptions
          org.fluss.rocksdb.RocksCallbackObject
          org.fluss.rocksdb.RocksDB
          org.fluss.rocksdb.RocksEnv
          org.fluss.rocksdb.RocksIterator
          org.fluss.rocksdb.RocksIteratorInterface
          org.fluss.rocksdb.RocksMemEnv
          org.fluss.rocksdb.RocksMutableObject
          org.fluss.rocksdb.RocksObject
          org.fluss.rocksdb.SkipListMemTableConfig
          org.fluss.rocksdb.Slice
          org.fluss.rocksdb.Snapshot
          org.fluss.rocksdb.SstFileManager
          org.fluss.rocksdb.SstFileWriter
          org.fluss.rocksdb.SstFileReader
          org.fluss.rocksdb.SstFileReaderIterator
          org.fluss.rocksdb.SstPartitionerFactory
          org.fluss.rocksdb.SstPartitionerFixedPrefixFactory
          org.fluss.rocksdb.Statistics
          org.fluss.rocksdb.StringAppendOperator
          org.fluss.rocksdb.TableFormatConfig
          org.fluss.rocksdb.ThreadStatus
          org.fluss.rocksdb.TimedEnv
          org.fluss.rocksdb.Transaction
          org.fluss.rocksdb.TransactionDB
          org.fluss.rocksdb.TransactionDBOptions
          org.fluss.rocksdb.TransactionLogIterator
          org.fluss.rocksdb.TransactionOptions
          org.fluss.rocksdb.TtlDB
          org.fluss.rocksdb.UInt64AddOperator
          org.fluss.rocksdb.VectorMemTableConfig
          org.fluss.rocksdb.WBWIRocksIterator
          org.fluss.rocksdb.WriteBatch
          org.fluss.rocksdb.WriteBatch.Handler
          org.fluss.rocksdb.WriteBatchInterface
          org.fluss.rocksdb.WriteBatchWithIndex
          org.fluss.rocksdb.WriteOptions
          org.fluss.rocksdb.NativeComparatorWrapperTest
          org.fluss.rocksdb.RocksDBExceptionTest
          org.fluss.rocksdb.SnapshotTest
          org.fluss.rocksdb.WriteBatchTest
          org.fluss.rocksdb.WriteBatchTestInternalHelper
          org.fluss.rocksdb.WriteBufferManager
          org.fluss.rocksdb.test.TestableEventListener
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
        PACKAGES org.fluss.rocksdb org.fluss.rocksdb.util
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
            OUTPUT_NAME "flussrocksdbjni-linux${bitness}"
    )
    set(ROCKSDB_JAR rocksdbjni-${CMAKE_PROJECT_VERSION}-linux${bitness}.jar)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(
            ${ROCKSDBJNI_STATIC_LIB}
            PROPERTIES
            OUTPUT_NAME flussrocksdbjni-osx-${CMAKE_SYSTEM_PROCESSOR}
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
            COMMAND ${Java_JAVA_EXECUTABLE} ${JVMARGS} -ea -Xcheck:jni -Djava.library.path=${PROJECT_BINARY_DIR}/java/${CMAKE_BUILD_TYPE} -classpath ${CMAKE_CURRENT_BINARY_DIR}/${ROCKSDB_JAR}$<SEMICOLON>${JAVA_RUN_TESTCLASSPATH}$<SEMICOLON>${ROCKSDBJNI_CLASSES_TEST_JAR_FILE} org.fluss.rocksdb.test.RocksJunitRunner ${CLAZZ}
    )
  else()
    add_test(
      NAME jtest_${CLAZZ}
            COMMAND ${Java_JAVA_EXECUTABLE} ${JVMARGS} -ea -Xcheck:jni -Djava.library.path=${PROJECT_BINARY_DIR}/java -classpath ${JAVA_RUN_TESTCLASSPATH}:${ROCKSDBJNI_CLASSES_TEST_JAR_FILE}:${CMAKE_CURRENT_BINARY_DIR}/${ROCKSDB_JAR} org.fluss.rocksdb.test.RocksJunitRunner ${CLAZZ}
    )
  endif()
endforeach(CLAZZ)
