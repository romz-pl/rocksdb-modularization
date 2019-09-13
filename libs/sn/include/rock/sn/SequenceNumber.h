#pragma once

#include <cstdint>

namespace rocksdb {

// Represents a sequence number in a WAL file.
typedef uint64_t SequenceNumber;

const SequenceNumber kMinUnCommittedSeq = 1;  // 0 is always committed

}
