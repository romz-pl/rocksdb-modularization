#include <rock/string/BytesToHumanString.h>

#include <cstdio>

namespace rocksdb {

std::string BytesToHumanString(uint64_t bytes) {
  const char* size_name[] = {"KB", "MB", "GB", "TB"};
  double final_size = static_cast<double>(bytes);
  size_t size_idx;

  // always start with KB
  final_size /= 1024;
  size_idx = 0;

  while (size_idx < 3 && final_size >= 1024) {
    final_size /= 1024;
    size_idx++;
  }

  char buf[20];
  snprintf(buf, sizeof(buf), "%.2f %s", final_size, size_name[size_idx]);
  return std::string(buf);
}


}
