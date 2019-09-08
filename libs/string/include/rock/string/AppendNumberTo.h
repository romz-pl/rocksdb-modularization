#pragma once

#include <string>

namespace rocksdb {

// Append a human-readable printout of "num" to *str
extern void AppendNumberTo(std::string* str, uint64_t num);

}
