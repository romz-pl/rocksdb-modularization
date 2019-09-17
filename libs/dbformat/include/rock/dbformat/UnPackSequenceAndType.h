#pragma once

#include <rock/dbformat/ValueType.h>

#include <cstdint>

namespace rocksdb {

// Given the result of PackSequenceAndType, store the sequence number in *seq
// and the ValueType in *t.
extern void UnPackSequenceAndType(uint64_t packed, uint64_t* seq, ValueType* t);

}
