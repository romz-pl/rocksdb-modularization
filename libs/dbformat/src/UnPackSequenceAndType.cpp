#include <rock/dbformat/UnPackSequenceAndType.h>

#include <rock/dbformat/kMaxSequenceNumber.h>
#include <rock/dbformat/IsExtendedValueType.h>

#include <cassert>

namespace rocksdb {

void UnPackSequenceAndType(uint64_t packed, uint64_t* seq, ValueType* t) {
  *seq = packed >> 8;
  *t = static_cast<ValueType>(packed & 0xff);

  assert(*seq <= kMaxSequenceNumber);
  assert(IsExtendedValueType(*t));
}

}
