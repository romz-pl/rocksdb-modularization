#pragma once

#include <string>

namespace rocksdb {

// Return a human-readable version of bytes
// ex: 1048576 -> 1.00 GB
extern std::string BytesToHumanString(uint64_t bytes);

}
