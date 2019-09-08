#include <rock/string/ParseVectorInt.h>

#include <rock/string/ParseInt.h>

namespace rocksdb {

std::vector<int> ParseVectorInt(const std::string& value) {
  std::vector<int> result;
  size_t start = 0;
  while (start < value.size()) {
    size_t end = value.find(':', start);
    if (end == std::string::npos) {
      result.push_back(ParseInt(value.substr(start)));
      break;
    } else {
      result.push_back(ParseInt(value.substr(start, end - start)));
      start = end + 1;
    }
  }
  return result;
}

}
