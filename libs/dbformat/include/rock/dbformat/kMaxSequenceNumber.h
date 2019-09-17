#pragma once

#include <rock/sn/SequenceNumber.h>

namespace rocksdb {

// We leave eight bits empty at the bottom so a type and sequence#
// can be packed together into 64-bits.
static const SequenceNumber kMaxSequenceNumber = ((0x1ull << 56) - 1);

}
