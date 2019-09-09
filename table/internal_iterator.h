// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//

#pragma once

#include <string>
#include <rock/slice/ExtractUserKey.h>
#include <rock/slice/Comparator.h>
#include <rock/iterator/Iterator.h>
#include <rock/status/Status.h>
#include "table/format.h"

#include <rock/iterator/InternalIteratorBase.h>
#include <rock/iterator/InternalIterator.h>

namespace rocksdb {


// Return an empty iterator (yields nothing).
template <class TValue = Slice>
extern InternalIteratorBase<TValue>* NewEmptyInternalIterator();

// Return an empty iterator with the specified status.
template <class TValue = Slice>
extern InternalIteratorBase<TValue>* NewErrorInternalIterator(
    const Status& status);

// Return an empty iterator with the specified status, allocated arena.
template <class TValue = Slice>
extern InternalIteratorBase<TValue>* NewErrorInternalIterator(
    const Status& status, Arena* arena);

}  // namespace rocksdb
