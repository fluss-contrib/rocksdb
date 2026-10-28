# Layout-based prefix extraction

A layout extractor finds the end of a prefix made of consecutive encoded byte
components. It does not decode rows, change keys, or know the application's
schema. The caller selects the components that make up its logical prefix.

## Interface

```java
PrefixLayout layout = PrefixLayout.of(
    PrefixLayout.varint32(), PrefixLayout.bytesVarint32());
options.useLayoutPrefixExtractor(layout);
```

Both `Options` and `ColumnFamilyOptions` support this method. `PrefixLayout` is
immutable, owns no native handles, and may be shared. Installation copies the
layout through JNI once. Flush, compaction, filter queries, and prefix iteration
execute native extraction without Java callbacks. The extracted `Slice` refers
to the original key bytes.

C++ callers use the existing options registry:

```cpp
ConfigOptions config;
config.ignore_unsupported_options = false;
Status status = SliceTransform::CreateFromString(
    config, "rocksdb.LayoutPrefix.v1:v32,b32", &options.prefix_extractor);
// Check status before using options.
```

The native execution plan coalesces adjacent fixed-width components while
preserving the serialized identity. Varint skip operations validate boundaries
without reconstructing numeric values; only length-prefixed payloads decode a
length. SST iterators check prefix filters before materializing readahead
prefixes and reuse the prefix string's capacity across seeks.

The versioned ID contains an ordered, comma-separated component list:

| Component | Meaning |
|-----------|---------|
| `fN` | Skip N bytes, where 0 < N <= INT32_MAX. |
| `v32` | Skip an unsigned base128 varint32, at most five bytes. |
| `v64` | Skip an unsigned base128 varint64, at most ten bytes. |
| `b32` | Read an unsigned varint32 length and skip that many bytes. |

Continuation bits and numeric overflow are checked. Non-minimal varints that
fit the declared width are accepted. An empty `b32` payload is valid. Payload
contents are not interpreted. The extractor stops after the last component;
trailing bytes are not parsed. All-fixed layouts use the built-in fixed-length
extractor with the sum of their lengths.

Unknown operations, versions, empty layouts, zero fixed lengths, and overflowing
fixed lengths are rejected during construction. Malformed or incomplete key
components are outside the extractor domain. `Transform` requires `InDomain`.

## Correctness and read modes

For any complete encoded prefix P and arbitrary suffix S, extraction satisfies
F(P || S) = P and F(P) = P. Each component determines its boundary independently
of trailing components. The accepted complete prefixes are prefix-free, so their
extension groups are contiguous under bytewise ordering. Other comparators must
be checked by the caller.

The layout must agree with every valid stored key, and a prefix query must
contain the complete extracted prefix. Do not infer this from a column count
without checking the actual stored key encoding. Layouts cannot repair encodings
where logical prefix values do not form contiguous bytewise key ranges.

Installing the extractor alone does not install a filter policy. Configure a
block-based Bloom filter, retain whole-key filtering when needed for point
lookups, and use an explicit prefix iterator mode. In this version,
`total_order_seek=true` together with `prefix_same_as_start=true` enables prefix
filtering while retaining total-order behavior for out-of-domain seek targets.
Use `total_order_seek=true`, without `prefix_same_as_start`, for unrestricted
scans. A query longer than the extracted prefix also needs its own iteration
upper bound (or an equivalent explicit stopping condition).

The variable-length extractor deliberately does not advertise a fixed maximum
prefix length. Do not rely on `auto_prefix_mode` to optimize every such query.

## SST compatibility

The canonical extractor ID includes the version and component parameters. SSTs
record that identity. The options registry reconstructs the extractor when
loading options or inspecting SST metadata, and RocksDB checks compatibility
before applying prefix filters.

Existing SSTs do not acquire prefix Bloom entries when an extractor is installed;
new flushes and compactions build them as files are rewritten. Disabling the
extractor leaves stored keys unchanged. Changing the extractor does not authorize
changing the stored key codec. Verify mixed-SST reads whenever adopting a new
layout version.

## Measurement

`db_bench` and `db_stress` can load the extractor through their existing
`--options_file` support. Put the versioned ID in the relevant column-family
section, alongside a compatible table filter policy:

```ini
[CFOptions "default"]
  prefix_extractor=rocksdb.LayoutPrefix.v1:v32,b32
```

The input keys must follow the described layout. Generic fixed-width benchmark
keys are not representative of arbitrary variable-length layouts. For stress
runs, disable fixed-prefix-specific operations unless the generated keys and
verification logic match the selected layout; exercise concurrent writes,
flushes, snapshots, compaction, and total-order reads separately.

Measure lookup hit and miss cases with overlapping SST ranges, both warm and cold
caches. Observe iterator filtering counters such as
`NON_LAST_LEVEL_SEEK_FILTERED`, data seeks, CPU, and latency. Bloom false positives
are allowed. Also measure flush/compaction CPU and filter size. A prefix present
in every SST may provide little filtering benefit while still incurring parsing
cost. Do not use wall-clock performance thresholds in correctness tests.
