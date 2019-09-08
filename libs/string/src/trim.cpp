#include <rock/string/trim.h>

#include <cctype>

namespace rocksdb {

std::string trim(const std::string& str) {
  if (str.empty()) return std::string();
  size_t start = 0;
  size_t end = str.size() - 1;
  while (isspace(str[start]) != 0 && start < end) {
    ++start;
  }
  while (isspace(str[end]) != 0 && start < end) {
    --end;
  }
  if (start <= end) {
    return str.substr(start, end - start + 1);
  }
  return std::string();
}

}
