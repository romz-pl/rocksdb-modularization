#include <rock/memtable/LookupKey.h>

namespace rocksdb {

LookupKey::~LookupKey() {
  if (start_ != space_) delete[] start_;
}

}
