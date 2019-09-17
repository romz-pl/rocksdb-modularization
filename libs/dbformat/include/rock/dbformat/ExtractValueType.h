#pragma once

#include <rock/slice/Slice.h>
#include <rock/dbformat/ExtractInternalKeyFooter.h>
#include <rock/dbformat/ValueType.h>

namespace rocksdb {

inline ValueType ExtractValueType(const Slice& internal_key) {
  uint64_t num = ExtractInternalKeyFooter(internal_key);
  unsigned char c = num & 0xff;
  return static_cast<ValueType>(c);
}

}
