#pragma once

namespace rocksdb {

class Iterator;

// Return an empty iterator (yields nothing).
extern Iterator* NewEmptyIterator();

}
