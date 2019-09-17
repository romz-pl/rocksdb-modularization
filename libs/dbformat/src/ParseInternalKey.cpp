#include <rock/dbformat/ParseInternalKey.h>

#include <rock/dbformat/ParsedInternalKey.h>
#include <rock/dbformat/IsExtendedValueType.h>
#include <rock/coding/coding.h>

namespace rocksdb {

bool ParseInternalKey(const Slice& internal_key,
                             ParsedInternalKey* result) {
  const size_t n = internal_key.size();
  if (n < 8) return false;
  uint64_t num = DecodeFixed64(internal_key.data() + n - 8);
  unsigned char c = num & 0xff;
  result->sequence = num >> 8;
  result->type = static_cast<ValueType>(c);
  assert(result->type <= ValueType::kMaxValue);
  result->user_key = Slice(internal_key.data(), n - 8);
  return IsExtendedValueType(result->type);
}

}
