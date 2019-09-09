#pragma once

#include <rock/slice/Slice.h>
#include <rock/iterator/InternalIteratorBase.h>

namespace rocksdb {

using InternalIterator = InternalIteratorBase<Slice>;

}
