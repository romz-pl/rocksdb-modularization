#pragma once

#include <string>
#include <vector>

namespace rocksdb {

extern std::vector<std::string> StringSplit(const std::string& arg, char delim);

}
