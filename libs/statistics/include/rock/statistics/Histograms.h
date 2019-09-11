#pragma once

#include <cstdint>

namespace rocksdb {
/**
 * Keep adding histogram's here.
 * Any histogram should have value less than HISTOGRAM_ENUM_MAX
 * Add a new Histogram by assigning it the current value of HISTOGRAM_ENUM_MAX
 * Add a string representation in HistogramsNameMap below
 * And increment HISTOGRAM_ENUM_MAX
 * Add a corresponding enum value to HistogramType.java in the java API
 */
enum Histograms : uint32_t {
  DB_GET = 0,
  DB_WRITE,
  COMPACTION_TIME,
  COMPACTION_CPU_TIME,
  SUBCOMPACTION_SETUP_TIME,
  TABLE_SYNC_MICROS,
  COMPACTION_OUTFILE_SYNC_MICROS,
  WAL_FILE_SYNC_MICROS,
  MANIFEST_FILE_SYNC_MICROS,
  // TIME SPENT IN IO DURING TABLE OPEN
  TABLE_OPEN_IO_MICROS,
  DB_MULTIGET,
  READ_BLOCK_COMPACTION_MICROS,
  READ_BLOCK_GET_MICROS,
  WRITE_RAW_BLOCK_MICROS,
  STALL_L0_SLOWDOWN_COUNT,
  STALL_MEMTABLE_COMPACTION_COUNT,
  STALL_L0_NUM_FILES_COUNT,
  HARD_RATE_LIMIT_DELAY_COUNT,
  SOFT_RATE_LIMIT_DELAY_COUNT,
  NUM_FILES_IN_SINGLE_COMPACTION,
  DB_SEEK,
  WRITE_STALL,
  SST_READ_MICROS,
  // The number of subcompactions actually scheduled during a compaction
  NUM_SUBCOMPACTIONS_SCHEDULED,
  // Value size distribution in each operation
  BYTES_PER_READ,
  BYTES_PER_WRITE,
  BYTES_PER_MULTIGET,

  // number of bytes compressed/decompressed
  // number of bytes is when uncompressed; i.e. before/after respectively
  BYTES_COMPRESSED,
  BYTES_DECOMPRESSED,
  COMPRESSION_TIMES_NANOS,
  DECOMPRESSION_TIMES_NANOS,
  // Number of merge operands passed to the merge operator in user read
  // requests.
  READ_NUM_MERGE_OPERANDS,

  // BlobDB specific stats
  // Size of keys written to BlobDB.
  BLOB_DB_KEY_SIZE,
  // Size of values written to BlobDB.
  BLOB_DB_VALUE_SIZE,
  // BlobDB Put/PutWithTTL/PutUntil/Write latency.
  BLOB_DB_WRITE_MICROS,
  // BlobDB Get lagency.
  BLOB_DB_GET_MICROS,
  // BlobDB MultiGet latency.
  BLOB_DB_MULTIGET_MICROS,
  // BlobDB Seek/SeekToFirst/SeekToLast/SeekForPrev latency.
  BLOB_DB_SEEK_MICROS,
  // BlobDB Next latency.
  BLOB_DB_NEXT_MICROS,
  // BlobDB Prev latency.
  BLOB_DB_PREV_MICROS,
  // Blob file write latency.
  BLOB_DB_BLOB_FILE_WRITE_MICROS,
  // Blob file read latency.
  BLOB_DB_BLOB_FILE_READ_MICROS,
  // Blob file sync latency.
  BLOB_DB_BLOB_FILE_SYNC_MICROS,
  // BlobDB garbage collection time.
  BLOB_DB_GC_MICROS,
  // BlobDB compression time.
  BLOB_DB_COMPRESSION_MICROS,
  // BlobDB decompression time.
  BLOB_DB_DECOMPRESSION_MICROS,
  // Time spent flushing memtable to disk
  FLUSH_TIME,
  SST_BATCH_SIZE,

  HISTOGRAM_ENUM_MAX,
};


}
