#pragma once

#include <rock/memtable/BucketHeader.h>
#include <rock/memtable/MemTableRep.h>
#include <rock/memtable/MemtableSkipList.h>

namespace rocksdb {

// A data structure used as the header of a skip list of a hash bucket.
struct SkipListBucketHeader {
  BucketHeader Counting_header;
  MemtableSkipList skip_list;

  explicit SkipListBucketHeader(const MemTableRep::KeyComparator& cmp,
                                Allocator* allocator, uint32_t count)
      : Counting_header(this,  // Pointing to itself to indicate header type.
                        count),
        skip_list(cmp, allocator) {}
};

}
