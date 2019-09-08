#pragma once

#include <string>

namespace rocksdb {

// Append a human-readable size in bytes
int AppendHumanBytes(uint64_t bytes, char* output, int len);

}
