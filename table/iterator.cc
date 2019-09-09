//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <rock/iterator/Iterator.h>
#include "memory/arena.h"
#include "table/internal_iterator.h"
#include "table/iterator_wrapper.h"

#include <rock/iterator/EmptyIterator.h>
#include <rock/iterator/EmptyInternalIterator.h>

namespace rocksdb {



template <class TValue>
InternalIteratorBase<TValue>* NewErrorInternalIterator(const Status& status) {
  return new EmptyInternalIterator<TValue>(status);
}

template InternalIteratorBase<IndexValue>* NewErrorInternalIterator(
    const Status& status);

template InternalIteratorBase<Slice>* NewErrorInternalIterator(
    const Status& status);

template <class TValue>
InternalIteratorBase<TValue>* NewErrorInternalIterator(const Status& status,
                                                       Arena* arena) {
  if (arena == nullptr) {
    return NewErrorInternalIterator<TValue>(status);
  } else {
    auto mem = arena->AllocateAligned(sizeof(EmptyInternalIterator<TValue>));
    return new (mem) EmptyInternalIterator<TValue>(status);
  }
}

template InternalIteratorBase<IndexValue>* NewErrorInternalIterator(
    const Status& status, Arena* arena);

template InternalIteratorBase<Slice>* NewErrorInternalIterator(
    const Status& status, Arena* arena);

template <class TValue>
InternalIteratorBase<TValue>* NewEmptyInternalIterator() {
  return new EmptyInternalIterator<TValue>(Status::OK());
}
template InternalIteratorBase<IndexValue>* NewEmptyInternalIterator();
template InternalIteratorBase<Slice>* NewEmptyInternalIterator();

template <class TValue>
InternalIteratorBase<TValue>* NewEmptyInternalIterator(Arena* arena) {
  if (arena == nullptr) {
    return NewEmptyInternalIterator<TValue>();
  } else {
    auto mem = arena->AllocateAligned(sizeof(EmptyInternalIterator<TValue>));
    return new (mem) EmptyInternalIterator<TValue>(Status::OK());
  }
}
template InternalIteratorBase<IndexValue>* NewEmptyInternalIterator(
    Arena* arena);
template InternalIteratorBase<Slice>* NewEmptyInternalIterator(Arena* arena);

}  // namespace rocksdb
