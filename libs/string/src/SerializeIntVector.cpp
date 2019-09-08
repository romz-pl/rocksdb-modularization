#include <rock/string/SerializeIntVector.h>

#include <rock/string/ToString.h>

namespace rocksdb {

bool SerializeIntVector(const std::vector<int>& vec, std::string* value) {
  *value = "";
  for (size_t i = 0; i < vec.size(); ++i) {
    if (i > 0) {
      *value += ":";
    }
    *value += ToString(vec[i]);
  }
  return true;
}

}
