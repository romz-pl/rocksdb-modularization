#include <rock/iterator/NewErrorIterator.h>

#include <rock/iterator/EmptyIterator.h>

namespace rocksdb {

Iterator* NewErrorIterator(const Status& status) {
  return new EmptyIterator(status);
}

}
