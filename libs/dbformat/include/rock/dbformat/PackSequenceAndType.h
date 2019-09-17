#pragma once

#include <rock/dbformat/ValueType.h>

#include <cstdint>

namespace rocksdb {

// Pack a sequence number and a ValueType into a uint64_t
extern uint64_t PackSequenceAndType(uint64_t seq, ValueType t);

}
