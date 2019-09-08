#pragma once

#include <string>

namespace rocksdb {

// Append a human-readable time in micros.
int AppendHumanMicros(uint64_t micros, char* output, int len,
                      bool fixed_format);

}
