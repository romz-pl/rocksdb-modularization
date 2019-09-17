#pragma once

#include <rock/dbformat/ValueType.h>

namespace rocksdb {

// Checks whether a type is an inline value type
// (i.e. a type used in memtable skiplist and sst file datablock).
inline bool IsValueType(ValueType t) {
  return t <= kTypeMerge || t == kTypeSingleDeletion || t == kTypeBlobIndex;
}

}
