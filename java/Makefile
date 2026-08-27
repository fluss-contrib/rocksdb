NATIVE_JAVA_CLASSES = \
	org.fluss.rocksdb.AbstractCompactionFilter\
	org.fluss.rocksdb.AbstractCompactionFilterFactory\
	org.fluss.rocksdb.AbstractComparator\
	org.fluss.rocksdb.AbstractEventListener\
	org.fluss.rocksdb.AbstractSlice\
	org.fluss.rocksdb.AbstractTableFilter\
	org.fluss.rocksdb.AbstractTraceWriter\
	org.fluss.rocksdb.AbstractTransactionNotifier\
	org.fluss.rocksdb.AbstractWalFilter\
	org.fluss.rocksdb.BackupEngine\
	org.fluss.rocksdb.BackupEngineOptions\
	org.fluss.rocksdb.BlockBasedTableConfig\
	org.fluss.rocksdb.BloomFilter\
	org.fluss.rocksdb.Checkpoint\
	org.fluss.rocksdb.ClockCache\
	org.fluss.rocksdb.Cache\
	org.fluss.rocksdb.CassandraCompactionFilter\
	org.fluss.rocksdb.CassandraValueMergeOperator\
	org.fluss.rocksdb.ColumnFamilyHandle\
	org.fluss.rocksdb.ColumnFamilyOptions\
	org.fluss.rocksdb.CompactionJobInfo\
	org.fluss.rocksdb.CompactionJobStats\
	org.fluss.rocksdb.CompactionOptions\
	org.fluss.rocksdb.CompactionOptionsFIFO\
	org.fluss.rocksdb.CompactionOptionsUniversal\
	org.fluss.rocksdb.CompactRangeOptions\
	org.fluss.rocksdb.ComparatorOptions\
	org.fluss.rocksdb.CompressionOptions\
	org.fluss.rocksdb.ConfigOptions\
	org.fluss.rocksdb.DBOptions\
	org.fluss.rocksdb.DirectSlice\
	org.fluss.rocksdb.Env\
	org.fluss.rocksdb.EnvOptions\
	org.fluss.rocksdb.FlushOptions\
	org.fluss.rocksdb.Filter\
	org.fluss.rocksdb.IngestExternalFileOptions\
	org.fluss.rocksdb.HashLinkedListMemTableConfig\
	org.fluss.rocksdb.HashSkipListMemTableConfig\
	org.fluss.rocksdb.ConcurrentTaskLimiter\
	org.fluss.rocksdb.ConcurrentTaskLimiterImpl\
	org.fluss.rocksdb.KeyMayExist\
	org.fluss.rocksdb.Logger\
	org.fluss.rocksdb.LRUCache\
	org.fluss.rocksdb.MemoryUsageType\
	org.fluss.rocksdb.MemoryUtil\
	org.fluss.rocksdb.MergeOperator\
	org.fluss.rocksdb.NativeComparatorWrapper\
	org.fluss.rocksdb.OptimisticTransactionDB\
	org.fluss.rocksdb.OptimisticTransactionOptions\
	org.fluss.rocksdb.Options\
	org.fluss.rocksdb.OptionsUtil\
	org.fluss.rocksdb.PersistentCache\
	org.fluss.rocksdb.PerfContext\
	org.fluss.rocksdb.PerfLevel\
	org.fluss.rocksdb.PlainTableConfig\
	org.fluss.rocksdb.RateLimiter\
	org.fluss.rocksdb.ReadOptions\
	org.fluss.rocksdb.RemoveEmptyValueCompactionFilter\
	org.fluss.rocksdb.RestoreOptions\
	org.fluss.rocksdb.RocksCallbackObject\
	org.fluss.rocksdb.RocksDB\
	org.fluss.rocksdb.RocksEnv\
	org.fluss.rocksdb.RocksIterator\
	org.fluss.rocksdb.RocksMemEnv\
	org.fluss.rocksdb.SkipListMemTableConfig\
	org.fluss.rocksdb.Slice\
	org.fluss.rocksdb.SstFileManager\
	org.fluss.rocksdb.SstFileWriter\
	org.fluss.rocksdb.SstFileReader\
	org.fluss.rocksdb.SstFileReaderIterator\
	org.fluss.rocksdb.SstPartitionerFactory\
	org.fluss.rocksdb.SstPartitionerFixedPrefixFactory\
	org.fluss.rocksdb.Statistics\
	org.fluss.rocksdb.ThreadStatus\
	org.fluss.rocksdb.TimedEnv\
	org.fluss.rocksdb.Transaction\
	org.fluss.rocksdb.TransactionDB\
	org.fluss.rocksdb.TransactionDBOptions\
	org.fluss.rocksdb.TransactionOptions\
	org.fluss.rocksdb.TransactionLogIterator\
	org.fluss.rocksdb.TtlDB\
	org.fluss.rocksdb.VectorMemTableConfig\
	org.fluss.rocksdb.Snapshot\
	org.fluss.rocksdb.StringAppendOperator\
	org.fluss.rocksdb.UInt64AddOperator\
	org.fluss.rocksdb.WriteBatch\
	org.fluss.rocksdb.WriteBatch.Handler\
	org.fluss.rocksdb.WriteOptions\
	org.fluss.rocksdb.WriteBatchWithIndex\
	org.fluss.rocksdb.WriteBufferManager\
	org.fluss.rocksdb.WBWIRocksIterator\
	org.fluss.rocksdb.util.StdErrLogger

NATIVE_JAVA_TEST_CLASSES = \
    org.fluss.rocksdb.RocksDBExceptionTest\
    org.fluss.rocksdb.test.TestableEventListener\
    org.fluss.rocksdb.NativeComparatorWrapperTest.NativeStringComparatorWrapper\
    org.fluss.rocksdb.WriteBatchTest\
    org.fluss.rocksdb.WriteBatchTestInternalHelper

ROCKSDB_MAJOR = $(shell grep -E "ROCKSDB_MAJOR.[0-9]" ../include/rocksdb/version.h | cut -d ' ' -f 3)
ROCKSDB_MINOR = $(shell grep -E "ROCKSDB_MINOR.[0-9]" ../include/rocksdb/version.h | cut -d ' ' -f 3)
ROCKSDB_PATCH = $(shell grep -E "ROCKSDB_PATCH.[0-9]" ../include/rocksdb/version.h | cut -d ' ' -f 3)

NATIVE_INCLUDE = ./include
ARCH := $(shell getconf LONG_BIT)
SHA256_CMD ?= sha256sum

JAVA_TESTS = \
	org.fluss.rocksdb.BackupEngineOptionsTest\
	org.fluss.rocksdb.BackupEngineTest\
	org.fluss.rocksdb.BlobOptionsTest\
	org.fluss.rocksdb.BlockBasedTableConfigTest\
	org.fluss.rocksdb.BuiltinComparatorTest\
	org.fluss.rocksdb.ByteBufferUnsupportedOperationTest\
	org.fluss.rocksdb.BytewiseComparatorRegressionTest\
	org.fluss.rocksdb.util.BytewiseComparatorTest\
	org.fluss.rocksdb.util.BytewiseComparatorIntTest\
	org.fluss.rocksdb.CheckPointTest\
	org.fluss.rocksdb.ClockCacheTest\
	org.fluss.rocksdb.ColumnFamilyOptionsTest\
	org.fluss.rocksdb.ColumnFamilyTest\
	org.fluss.rocksdb.CompactionFilterFactoryTest\
	org.fluss.rocksdb.CompactionJobInfoTest\
	org.fluss.rocksdb.CompactionJobStatsTest\
	org.fluss.rocksdb.CompactionOptionsTest\
	org.fluss.rocksdb.CompactionOptionsFIFOTest\
	org.fluss.rocksdb.CompactionOptionsUniversalTest\
	org.fluss.rocksdb.CompactionPriorityTest\
	org.fluss.rocksdb.CompactionStopStyleTest\
	org.fluss.rocksdb.ComparatorOptionsTest\
	org.fluss.rocksdb.CompressionOptionsTest\
	org.fluss.rocksdb.CompressionTypesTest\
	org.fluss.rocksdb.DBOptionsTest\
	org.fluss.rocksdb.DirectSliceTest\
	org.fluss.rocksdb.util.EnvironmentTest\
	org.fluss.rocksdb.EnvOptionsTest\
	org.fluss.rocksdb.EventListenerTest\
	org.fluss.rocksdb.IngestExternalFileOptionsTest\
	org.fluss.rocksdb.util.IntComparatorTest\
	org.fluss.rocksdb.util.JNIComparatorTest\
	org.fluss.rocksdb.FilterTest\
	org.fluss.rocksdb.FlushTest\
	org.fluss.rocksdb.ImportColumnFamilyTest\
	org.fluss.rocksdb.InfoLogLevelTest\
	org.fluss.rocksdb.KeyExistsTest \
	org.fluss.rocksdb.KeyMayExistTest\
	org.fluss.rocksdb.ConcurrentTaskLimiterTest\
	org.fluss.rocksdb.LoggerTest\
	org.fluss.rocksdb.LRUCacheTest\
	org.fluss.rocksdb.MemoryUtilTest\
	org.fluss.rocksdb.MemTableTest\
	org.fluss.rocksdb.MergeCFVariantsTest\
	org.fluss.rocksdb.MergeTest\
	org.fluss.rocksdb.MergeVariantsTest\
	org.fluss.rocksdb.MultiColumnRegressionTest \
	org.fluss.rocksdb.MultiGetManyKeysTest\
	org.fluss.rocksdb.MultiGetTest\
	org.fluss.rocksdb.MixedOptionsTest\
	org.fluss.rocksdb.MutableColumnFamilyOptionsTest\
	org.fluss.rocksdb.MutableDBOptionsTest\
	org.fluss.rocksdb.MutableOptionsGetSetTest \
	org.fluss.rocksdb.NativeComparatorWrapperTest\
	org.fluss.rocksdb.NativeLibraryLoaderTest\
	org.fluss.rocksdb.OptimisticTransactionTest\
	org.fluss.rocksdb.OptimisticTransactionDBTest\
	org.fluss.rocksdb.OptimisticTransactionOptionsTest\
	org.fluss.rocksdb.OptionsUtilTest\
	org.fluss.rocksdb.OptionsTest\
	org.fluss.rocksdb.PerfLevelTest \
	org.fluss.rocksdb.PerfContextTest \
	org.fluss.rocksdb.PutCFVariantsTest\
	org.fluss.rocksdb.PutVariantsTest\
	org.fluss.rocksdb.PlainTableConfigTest\
	org.fluss.rocksdb.RateLimiterTest\
	org.fluss.rocksdb.ReadOnlyTest\
	org.fluss.rocksdb.ReadOptionsTest\
	org.fluss.rocksdb.util.ReverseBytewiseComparatorIntTest\
	org.fluss.rocksdb.RocksDBTest\
	org.fluss.rocksdb.RocksDBExceptionTest\
	org.fluss.rocksdb.DefaultEnvTest\
	org.fluss.rocksdb.RocksIteratorTest\
	org.fluss.rocksdb.RocksMemEnvTest\
	org.fluss.rocksdb.util.SizeUnitTest\
	org.fluss.rocksdb.SecondaryDBTest\
	org.fluss.rocksdb.SliceTest\
	org.fluss.rocksdb.SnapshotTest\
	org.fluss.rocksdb.SstFileManagerTest\
	org.fluss.rocksdb.SstFileWriterTest\
	org.fluss.rocksdb.SstFileReaderTest\
	org.fluss.rocksdb.SstPartitionerTest\
	org.fluss.rocksdb.TableFilterTest\
	org.fluss.rocksdb.TimedEnvTest\
	org.fluss.rocksdb.TransactionTest\
	org.fluss.rocksdb.TransactionDBTest\
	org.fluss.rocksdb.TransactionOptionsTest\
	org.fluss.rocksdb.TransactionDBOptionsTest\
	org.fluss.rocksdb.TransactionLogIteratorTest\
	org.fluss.rocksdb.TtlDBTest\
	org.fluss.rocksdb.StatisticsTest\
	org.fluss.rocksdb.StatisticsCollectorTest\
	org.fluss.rocksdb.VerifyChecksumsTest\
	org.fluss.rocksdb.WalFilterTest\
	org.fluss.rocksdb.WALRecoveryModeTest\
	org.fluss.rocksdb.WriteBatchHandlerTest\
	org.fluss.rocksdb.WriteBatchTest\
	org.fluss.rocksdb.WriteBatchThreadedTest\
	org.fluss.rocksdb.WriteOptionsTest\
	org.fluss.rocksdb.WriteBatchWithIndexTest\
	org.fluss.rocksdb.util.StdErrLoggerTest

MAIN_SRC = src/main/java
TEST_SRC = src/test/java
OUTPUT = target
MAIN_CLASSES = $(OUTPUT)/classes
TEST_CLASSES = $(OUTPUT)/test-classes
JAVADOC = $(OUTPUT)/apidocs

BENCHMARK_MAIN_SRC = benchmark/src/main/java
BENCHMARK_OUTPUT = benchmark/target
BENCHMARK_MAIN_CLASSES = $(BENCHMARK_OUTPUT)/classes

SAMPLES_MAIN_SRC = samples/src/main/java
SAMPLES_OUTPUT = samples/target
SAMPLES_MAIN_CLASSES = $(SAMPLES_OUTPUT)/classes

JAVA_TEST_LIBDIR = test-libs
JAVA_JUNIT_VER = 4.13.1
JAVA_JUNIT_SHA256 = c30719db974d6452793fe191b3638a5777005485bae145924044530ffa5f6122
JAVA_JUNIT_JAR = junit-$(JAVA_JUNIT_VER).jar
JAVA_JUNIT_JAR_PATH = $(JAVA_TEST_LIBDIR)/$(JAVA_JUNIT_JAR)
JAVA_HAMCREST_VER = 2.2
JAVA_HAMCREST_SHA256 = 5e62846a89f05cd78cd9c1a553f340d002458380c320455dd1f8fc5497a8a1c1
JAVA_HAMCREST_JAR = hamcrest-$(JAVA_HAMCREST_VER).jar
JAVA_HAMCREST_JAR_PATH = $(JAVA_TEST_LIBDIR)/$(JAVA_HAMCREST_JAR)
JAVA_MOCKITO_VER = 1.10.19
JAVA_MOCKITO_SHA256 = d1a7a7ef14b3db5c0fc3e0a63a81b374b510afe85add9f7984b97911f4c70605
JAVA_MOCKITO_JAR = mockito-all-$(JAVA_MOCKITO_VER).jar
JAVA_MOCKITO_JAR_PATH = $(JAVA_TEST_LIBDIR)/$(JAVA_MOCKITO_JAR)
JAVA_CGLIB_VER = 3.3.0
JAVA_CGLIB_SHA256 = 9fe0c26d7464140ccdfe019ac687be1fb906122b508ab54beb810db0f09a9212
JAVA_CGLIB_JAR = cglib-$(JAVA_CGLIB_VER).jar
JAVA_CGLIB_JAR_PATH = $(JAVA_TEST_LIBDIR)/$(JAVA_CGLIB_JAR)
JAVA_ASSERTJ_VER = 2.9.0
JAVA_ASSERTJ_SHA256 = 5e88ea3ecbe3c48aa1346fec76c84979fa9c8d22499f11479011691230e8babf
JAVA_ASSERTJ_JAR = assertj-core-$(JAVA_ASSERTJ_VER).jar
JAVA_ASSERTJ_JAR_PATH = $(JAVA_TEST_LIBDIR)/$(JAVA_ASSERTJ_JAR)
JAVA_TESTCLASSPATH = $(JAVA_JUNIT_JAR_PATH):$(JAVA_HAMCREST_JAR_PATH):$(JAVA_MOCKITO_JAR_PATH):$(JAVA_CGLIB_JAR_PATH):$(JAVA_ASSERTJ_JAR_PATH)

MVN_LOCAL = ~/.m2/repository

# Set the path of the java commands
ifeq ($(JAVA_CMD),)
ifneq ($(JAVA_HOME),)
JAVA_CMD := $(JAVA_HOME)/bin/java
else
JAVA_CMD := java
endif
endif

ifeq ($(JAVAC_CMD),)
ifneq ($(JAVA_HOME),)
JAVAC_CMD := $(JAVA_HOME)/bin/javac
else
JAVAC_CMD := javac
endif
endif

ifeq ($(JAVADOC_CMD),)
ifneq ($(JAVA_HOME),)
JAVADOC_CMD := $(JAVA_HOME)/bin/javadoc
else
JAVADOC_CMD := javadoc
endif
endif

MAVEN_CMD := mvn

# Look for the Java version (1.6->6, 1.7->7, 1.8->8, 11.0->11, 13.0->13, 15.0->15 etc..)
JAVAC_VERSION := $(shell $(JAVAC_CMD) -version 2>&1)
JAVAC_MAJOR_VERSION := $(word 2,$(subst ., ,$(JAVAC_VERSION)))
ifeq ($(JAVAC_MAJOR_VERSION),1)
JAVAC_MAJOR_VERSION := $(word 3,$(subst ., ,$(JAVAC_VERSION)))
endif

# Test whether the version we see meets our minimum
MIN_JAVAC_MAJOR_VERSION := 8
JAVAC_VERSION_GE_MIN := $(shell [ $(JAVAC_MAJOR_VERSION) -ge $(MIN_JAVAC_MAJOR_VERSION) ] > /dev/null 2>&1 && echo true)

# Set the default JAVA_ARGS to "" for DEBUG_LEVEL=0
JAVA_ARGS ?=

JAVAC_ARGS ?=

# Read plugin configuration
PLUGIN_PATH = ../plugin
ROCKSDB_PLUGIN_MKS = $(foreach plugin, $(ROCKSDB_PLUGINS), $(PLUGIN_PATH)/$(plugin)/*.mk)
include $(ROCKSDB_PLUGIN_MKS)

# Add paths to Java sources in plugins
ROCKSDB_PLUGIN_JAVA_ROOTS = $(foreach plugin, $(ROCKSDB_PLUGINS), $(PLUGIN_PATH)/$(plugin)/java)
PLUGIN_SOURCES = $(foreach root, $(ROCKSDB_PLUGIN_JAVA_ROOTS), $(foreach pkg, org/fluss/rocksdb/util org/fluss/rocksdb, $(root)/$(MAIN_SRC)/$(pkg)/*.java))
CORE_SOURCES = $(foreach pkg, org/fluss/rocksdb/util org/fluss/rocksdb, $(MAIN_SRC)/$(pkg)/*.java)
SOURCES = $(wildcard $(CORE_SOURCES) $(PLUGIN_SOURCES))
PLUGIN_TEST_SOURCES = $(foreach root, $(ROCKSDB_PLUGIN_JAVA_ROOTS), $(foreach pkg, org/fluss/rocksdb/test org/fluss/rocksdb/util org/fluss/rocksdb, $(root)/$(TEST_SRC)/$(pkg)/*.java))
CORE_TEST_SOURCES = $(foreach pkg, org/fluss/rocksdb/test org/fluss/rocksdb/util org/fluss/rocksdb, $(TEST_SRC)/$(pkg)/*.java)
TEST_SOURCES = $(wildcard $(CORE_TEST_SOURCES) $(PLUGIN_TEST_SOURCES))

# Configure the plugin tests and java classes
ROCKSDB_PLUGIN_NATIVE_JAVA_CLASSES = $(foreach plugin, $(ROCKSDB_PLUGINS), $(foreach class, $($(plugin)_NATIVE_JAVA_CLASSES), $(class)))
NATIVE_JAVA_CLASSES = $(NATIVE_JAVA_CLASSES) $(ROCKSDB_PLUGIN_NATIVE_JAVA_CLASSES)
ROCKSDB_PLUGIN_JAVA_TESTS = $(foreach plugin, $(ROCKSDB_PLUGINS), $(foreach testclass, $($(plugin)_JAVA_TESTS), $(testclass)))
ALL_JAVA_TESTS = $(JAVA_TESTS) $(ROCKSDB_PLUGIN_JAVA_TESTS)

# When debugging add -Xcheck:jni to the java args
ifneq ($(DEBUG_LEVEL),0)
	JAVA_ARGS += -ea -Xcheck:jni
	JAVAC_ARGS += -Xlint:deprecation -Xlint:unchecked
endif

# Using a Facebook AWS account for S3 storage. (maven.org has a history
# of failing in Travis builds.)
DEPS_URL?=https://rocksdb-deps.s3-us-west-2.amazonaws.com/jars

java-version:
ifneq ($(JAVAC_VERSION_GE_MIN),true)
	echo 'Java version is $(JAVAC_VERSION), minimum required version is $(MIN_JAVAC_MAJOR_VERSION)'
	exit 1
endif

clean: clean-not-downloaded clean-downloaded

clean-not-downloaded:
	$(AM_V_at)rm -rf $(NATIVE_INCLUDE) $(OUTPUT) $(BENCHMARK_OUTPUT) $(SAMPLES_OUTPUT)

clean-downloaded:
	$(AM_V_at)rm -rf $(JAVA_TEST_LIBDIR)


javadocs: java
	$(AM_V_GEN)mkdir -p $(JAVADOC)
	$(AM_V_at)$(JAVADOC_CMD) -d $(JAVADOC) -sourcepath $(MAIN_SRC) -subpackages org

javalib: java java_test javadocs

java: java-version
	$(AM_V_GEN)mkdir -p $(MAIN_CLASSES)
	$(AM_V_at) $(JAVAC_CMD) $(JAVAC_ARGS) -h $(NATIVE_INCLUDE) -d $(MAIN_CLASSES) $(SOURCES)
	$(AM_V_at)@cp ../HISTORY.md ./HISTORY-CPP.md
	$(AM_V_at)@rm -f ./HISTORY-CPP.md

sample: java
	$(AM_V_GEN)mkdir -p $(SAMPLES_MAIN_CLASSES)
	$(AM_V_at)$(JAVAC_CMD) $(JAVAC_ARGS) -cp $(MAIN_CLASSES) -d $(SAMPLES_MAIN_CLASSES) $(SAMPLES_MAIN_SRC)/RocksDBSample.java
	$(AM_V_at)@rm -rf /tmp/rocksdbjni
	$(AM_V_at)@rm -rf /tmp/rocksdbjni_not_found
	$(JAVA_CMD) $(JAVA_ARGS) -Djava.library.path=target -cp $(MAIN_CLASSES):$(SAMPLES_MAIN_CLASSES) RocksDBSample /tmp/rocksdbjni
	$(AM_V_at)@rm -rf /tmp/rocksdbjni
	$(AM_V_at)@rm -rf /tmp/rocksdbjni_not_found

column_family_sample: java
	$(AM_V_GEN)mkdir -p $(SAMPLES_MAIN_CLASSES)
	$(AM_V_at)$(JAVAC_CMD) $(JAVAC_ARGS) -cp $(MAIN_CLASSES) -d $(SAMPLES_MAIN_CLASSES) $(SAMPLES_MAIN_SRC)/RocksDBColumnFamilySample.java
	$(AM_V_at)@rm -rf /tmp/rocksdbjni
	$(JAVA_CMD) $(JAVA_ARGS) -Djava.library.path=target -cp $(MAIN_CLASSES):$(SAMPLES_MAIN_CLASSES) RocksDBColumnFamilySample /tmp/rocksdbjni
	$(AM_V_at)@rm -rf /tmp/rocksdbjni

transaction_sample: java
	$(AM_V_GEN)mkdir -p $(SAMPLES_MAIN_CLASSES)
	$(AM_V_at)$(JAVAC_CMD) -cp $(MAIN_CLASSES) -d $(SAMPLES_MAIN_CLASSES) $(SAMPLES_MAIN_SRC)/TransactionSample.java
	$(AM_V_at)@rm -rf /tmp/rocksdbjni
	$(JAVA_CMD) -ea -Xcheck:jni -Djava.library.path=target -cp $(MAIN_CLASSES):$(SAMPLES_MAIN_CLASSES) TransactionSample /tmp/rocksdbjni
	$(AM_V_at)@rm -rf /tmp/rocksdbjni

optimistic_transaction_sample: java
	$(AM_V_GEN)mkdir -p $(SAMPLES_MAIN_CLASSES)
	$(AM_V_at)$(JAVAC_CMD) -cp $(MAIN_CLASSES) -d $(SAMPLES_MAIN_CLASSES) $(SAMPLES_MAIN_SRC)/OptimisticTransactionSample.java
	$(AM_V_at)@rm -rf /tmp/rocksdbjni
	$(JAVA_CMD) -ea -Xcheck:jni -Djava.library.path=target -cp $(MAIN_CLASSES):$(SAMPLES_MAIN_CLASSES) OptimisticTransactionSample /tmp/rocksdbjni
	$(AM_V_at)@rm -rf /tmp/rocksdbjni

$(JAVA_TEST_LIBDIR):
	mkdir -p "$(JAVA_TEST_LIBDIR)"

$(JAVA_JUNIT_JAR_PATH): $(JAVA_TEST_LIBDIR)
ifneq (,$(wildcard $(MVN_LOCAL)/junit/junit/$(JAVA_JUNIT_VER)/$(JAVA_JUNIT_JAR)))
	cp -v $(MVN_LOCAL)/junit/junit/$(JAVA_JUNIT_VER)/$(JAVA_JUNIT_JAR) $(JAVA_TEST_LIBDIR)
else
	curl --fail --insecure --output $(JAVA_JUNIT_JAR_PATH) --location $(DEPS_URL)/$(JAVA_JUNIT_JAR)
	JAVA_JUNIT_SHA256_ACTUAL=`$(SHA256_CMD) $(JAVA_JUNIT_JAR_PATH) | cut -d ' ' -f 1`; \
	if [ "$(JAVA_JUNIT_SHA256)" != "$$JAVA_JUNIT_SHA256_ACTUAL" ]; then \
		echo $(JAVA_JUNIT_JAR_PATH) checksum mismatch, expected=\"$(JAVA_JUNIT_SHA256)\" actual=\"$$JAVA_JUNIT_SHA256_ACTUAL\"; \
		exit 1; \
	fi
endif

$(JAVA_HAMCREST_JAR_PATH): $(JAVA_TEST_LIBDIR)
ifneq (,$(wildcard $(MVN_LOCAL)/org/hamcrest/hamcrest/$(JAVA_HAMCREST_VER)/$(JAVA_HAMCREST_JAR)))
	cp -v $(MVN_LOCAL)/org/hamcrest/hamcrest/$(JAVA_HAMCREST_VER)/$(JAVA_HAMCREST_JAR) $(JAVA_TEST_LIBDIR)
else
	curl --fail --insecure --output $(JAVA_HAMCREST_JAR_PATH) --location $(DEPS_URL)/$(JAVA_HAMCREST_JAR)
	JAVA_HAMCREST_SHA256_ACTUAL=`$(SHA256_CMD) $(JAVA_HAMCREST_JAR_PATH) | cut -d ' ' -f 1`; \
	if [ "$(JAVA_HAMCREST_SHA256)" != "$$JAVA_HAMCREST_SHA256_ACTUAL" ]; then \
		echo $(JAVA_HAMCREST_JAR_PATH) checksum mismatch, expected=\"$(JAVA_HAMCREST_SHA256)\" actual=\"$$JAVA_HAMCREST_SHA256_ACTUAL\"; \
		exit 1; \
	fi
endif

$(JAVA_MOCKITO_JAR_PATH): $(JAVA_TEST_LIBDIR)
ifneq (,$(wildcard $(MVN_LOCAL)/org/mockito/mockito-all/$(JAVA_MOCKITO_VER)/$(JAVA_MOCKITO_JAR)))
	cp -v $(MVN_LOCAL)/org/mockito/mockito-all/$(JAVA_MOCKITO_VER)/$(JAVA_MOCKITO_JAR) $(JAVA_TEST_LIBDIR)
else
	curl --fail --insecure --output "$(JAVA_MOCKITO_JAR_PATH)" --location $(DEPS_URL)/$(JAVA_MOCKITO_JAR)
	JAVA_MOCKITO_SHA256_ACTUAL=`$(SHA256_CMD) $(JAVA_MOCKITO_JAR_PATH) | cut -d ' ' -f 1`; \
	if [ "$(JAVA_MOCKITO_SHA256)" != "$$JAVA_MOCKITO_SHA256_ACTUAL" ]; then \
		echo $(JAVA_MOCKITO_JAR_PATH) checksum mismatch, expected=\"$(JAVA_MOCKITO_SHA256)\" actual=\"$$JAVA_MOCKITO_SHA256_ACTUAL\"; \
		exit 1; \
	fi
endif

$(JAVA_CGLIB_JAR_PATH): $(JAVA_TEST_LIBDIR)
ifneq (,$(wildcard $(MVN_LOCAL)/cglib/cglib/$(JAVA_CGLIB_VER)/$(JAVA_CGLIB_JAR)))
	cp -v $(MVN_LOCAL)/cglib/cglib/$(JAVA_CGLIB_VER)/$(JAVA_CGLIB_JAR) $(JAVA_TEST_LIBDIR)
else
	curl --fail --insecure --output "$(JAVA_CGLIB_JAR_PATH)" --location $(DEPS_URL)/$(JAVA_CGLIB_JAR)
	JAVA_CGLIB_SHA256_ACTUAL=`$(SHA256_CMD) $(JAVA_CGLIB_JAR_PATH) | cut -d ' ' -f 1`; \
	if [ "$(JAVA_CGLIB_SHA256)" != "$$JAVA_CGLIB_SHA256_ACTUAL" ]; then \
		echo $(JAVA_CGLIB_JAR_PATH) checksum mismatch, expected=\"$(JAVA_CGLIB_SHA256)\" actual=\"$$JAVA_CGLIB_SHA256_ACTUAL\"; \
		exit 1; \
	fi
endif

$(JAVA_ASSERTJ_JAR_PATH): $(JAVA_TEST_LIBDIR)
ifneq (,$(wildcard $(MVN_LOCAL)/org/assertj/assertj-core/$(JAVA_ASSERTJ_VER)/$(JAVA_ASSERTJ_JAR)))
	cp -v $(MVN_LOCAL)/org/assertj/assertj-core/$(JAVA_ASSERTJ_VER)/$(JAVA_ASSERTJ_JAR) $(JAVA_TEST_LIBDIR)
else
	curl --fail --insecure --output "$(JAVA_ASSERTJ_JAR_PATH)" --location $(DEPS_URL)/$(JAVA_ASSERTJ_JAR)
	JAVA_ASSERTJ_SHA256_ACTUAL=`$(SHA256_CMD) $(JAVA_ASSERTJ_JAR_PATH) | cut -d ' ' -f 1`; \
	if [ "$(JAVA_ASSERTJ_SHA256)" != "$$JAVA_ASSERTJ_SHA256_ACTUAL" ]; then \
		echo $(JAVA_ASSERTJ_JAR_PATH) checksum mismatch, expected=\"$(JAVA_ASSERTJ_SHA256)\" actual=\"$$JAVA_ASSERTJ_SHA256_ACTUAL\"; \
		exit 1; \
	fi
endif

resolve_test_deps: $(JAVA_JUNIT_JAR_PATH) $(JAVA_HAMCREST_JAR_PATH) $(JAVA_MOCKITO_JAR_PATH) $(JAVA_CGLIB_JAR_PATH) $(JAVA_ASSERTJ_JAR_PATH)

java_test: java resolve_test_deps
	$(AM_V_GEN)mkdir -p $(TEST_CLASSES)
	$(AM_V_at) $(JAVAC_CMD) $(JAVAC_ARGS) -cp $(MAIN_CLASSES):$(JAVA_TESTCLASSPATH) -h $(NATIVE_INCLUDE) -d $(TEST_CLASSES)\
		$(TEST_SOURCES)

test: java java_test
	$(MAKE) run_test

run_test:
	$(JAVA_CMD) $(JAVA_ARGS) -Djava.library.path=target -cp "$(MAIN_CLASSES):$(TEST_CLASSES):$(JAVA_TESTCLASSPATH):target/*" org.fluss.rocksdb.test.RocksJunitRunner $(ALL_JAVA_TESTS)
	$(JAVA_CMD) $(JAVA_ARGS) -Djava.library.path=target -cp "$(MAIN_CLASSES):$(TEST_CLASSES):$(JAVA_TESTCLASSPATH):target/*" org.fluss.rocksdb.test.RocksJunitRunner org.fluss.rocksdb.StatisticsTest

run_plugin_test:
	$(JAVA_CMD) $(JAVA_ARGS) -Djava.library.path=target -cp "$(MAIN_CLASSES):$(TEST_CLASSES):$(JAVA_TESTCLASSPATH):target/*" org.fluss.rocksdb.test.RocksJunitRunner $(ROCKSDB_PLUGIN_JAVA_TESTS)

db_bench: java
	$(AM_V_GEN)mkdir -p $(BENCHMARK_MAIN_CLASSES)
	$(AM_V_at)$(JAVAC_CMD) $(JAVAC_ARGS) -cp $(MAIN_CLASSES) -d $(BENCHMARK_MAIN_CLASSES) $(BENCHMARK_MAIN_SRC)/org/fluss/rocksdb/benchmark/*.java

pmd:
	$(MAVEN_CMD) pmd:pmd pmd:cpd pmd:check
