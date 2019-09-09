#include <rock/iterator/NewEmptyIterator.h>

#include <rock/iterator/EmptyIterator.h>

namespace rocksdb {

Iterator* NewEmptyIterator() {
    return new EmptyIterator(Status::OK());
}

}
