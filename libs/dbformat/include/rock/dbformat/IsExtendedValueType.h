#pragma once

#include <rock/dbformat/IsValueType.h>

namespace rocksdb {

// Checks whether a type is from user operation
// kTypeRangeDeletion is in meta block so this API is separated from above
inline bool IsExtendedValueType(ValueType t) {
  return IsValueType(t) || t == kTypeRangeDeletion;
}

}
