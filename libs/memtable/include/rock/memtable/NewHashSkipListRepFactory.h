#pragma once

#ifndef ROCKSDB_LITE

#include <cstdint>
#include <cstddef>

namespace rocksdb {

class MemTableRepFactory;

// This class contains a fixed array of buckets, each
// pointing to a skiplist (null if the bucket is empty).
// bucket_count: number of fixed array buckets
// skiplist_height: the max height of the skiplist
// skiplist_branching_factor: probabilistic size ratio between adjacent
//                            link lists in the skiplist
extern MemTableRepFactory* NewHashSkipListRepFactory(
    size_t bucket_count = 1000000, int32_t skiplist_height = 4,
    int32_t skiplist_branching_factor = 4);


}

#endif  // ROCKSDB_LITE
