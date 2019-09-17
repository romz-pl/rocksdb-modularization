#pragma once

#include <string>

namespace rocksdb {

struct ParsedInternalKey;

// Append the serialization of "key" to *result.
extern void AppendInternalKey(std::string* result,
                              const ParsedInternalKey& key);

}
