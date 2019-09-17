#include <rock/dbformat/PackSequenceAndType.h>

#include <rock/dbformat/kMaxSequenceNumber.h>
#include <rock/dbformat/IsExtendedValueType.h>

#include <cassert>

namespace rocksdb {

uint64_t PackSequenceAndType(uint64_t seq, ValueType t) {
  assert(seq <= kMaxSequenceNumber);
  assert(IsExtendedValueType(t));
  return (seq << 8) | t;
}

}
