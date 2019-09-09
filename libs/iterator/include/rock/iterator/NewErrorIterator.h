#pragma once

namespace rocksdb {

class Iterator;
class Status;

// Return an empty iterator with the specified status.
extern Iterator* NewErrorIterator(const Status& status);

}
