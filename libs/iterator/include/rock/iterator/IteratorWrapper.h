#pragma once

#include <rock/iterator/IteratorWrapperBase.h>

namespace rocksdb {

using IteratorWrapper = IteratorWrapperBase<Slice>;

}
