#pragma once

#include <string>

namespace rocksdb {

class Slice;

extern const char* EncodeKey(std::string* scratch, const Slice& target);

}
