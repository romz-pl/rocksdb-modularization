#pragma once

#include <string>

namespace rocksdb {

class Slice;

// Return a human-readable version of "value".
// Escapes any non-printable characters found in "value".
extern std::string EscapeString(const Slice& value);

}
