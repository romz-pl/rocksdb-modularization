#pragma once

#include <rock/memtable/MemTableRep.h>

namespace rocksdb {

class Logger;
class SliceTransform;

// This is the base class for all factories that are used by RocksDB to create
// new MemTableRep objects
class MemTableRepFactory {
 public:
  virtual ~MemTableRepFactory() {}

  virtual MemTableRep* CreateMemTableRep(const MemTableRep::KeyComparator&,
                                         Allocator*, const SliceTransform*,
                                         Logger* logger) = 0;
  virtual MemTableRep* CreateMemTableRep(
      const MemTableRep::KeyComparator& key_cmp, Allocator* allocator,
      const SliceTransform* slice_transform, Logger* logger,
      uint32_t /* column_family_id */) {
    return CreateMemTableRep(key_cmp, allocator, slice_transform, logger);
  }

  virtual const char* Name() const = 0;

  // Return true if the current MemTableRep supports concurrent inserts
  // Default: false
  virtual bool IsInsertConcurrentlySupported() const { return false; }

  // Return true if the current MemTableRep supports detecting duplicate
  // <key,seq> at insertion time. If true, then MemTableRep::Insert* returns
  // false when if the <key,seq> already exists.
  // Default: false
  virtual bool CanHandleDuplicatedKey() const { return false; }
};

}
