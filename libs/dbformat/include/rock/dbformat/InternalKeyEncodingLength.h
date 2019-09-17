#pragma once

#include <rock/dbformat/ParsedInternalKey.h>

namespace rocksdb {

// Return the length of the encoding of "key".
inline size_t InternalKeyEncodingLength(const ParsedInternalKey& key) {
  return key.user_key.size() + 8;
}

}
