#pragma once

#include <rock/sn/SequenceNumber.h>
#include <rock/numeric_limits/numeric_limits.h>

namespace rocksdb {

static const SequenceNumber kDisableGlobalSequenceNumber = port::kMaxUint64;

}
