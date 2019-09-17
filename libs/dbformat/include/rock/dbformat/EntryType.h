#pragma once

#include <rock/dbformat/ValueType.h>

namespace rocksdb {

// User-oriented representation of internal key types.
enum EntryType {
  kEntryPut,
  kEntryDelete,
  kEntrySingleDelete,
  kEntryMerge,
  kEntryRangeDeletion,
  kEntryBlobIndex,
  kEntryOther,
};

}
