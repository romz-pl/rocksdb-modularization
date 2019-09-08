#pragma once

#include <string>
#include <vector>

namespace rocksdb {

std::vector<int> ParseVectorInt(const std::string& value);

}
