#pragma once

#include <string>
#include <vector>

namespace rocksdb {

bool SerializeIntVector(const std::vector<int>& vec, std::string* value);

}
