#pragma once

#include <rock/memtable/MemTableRepFactory.h>

namespace rocksdb {

// This uses a skip list to store keys. It is the default.
//
// Parameters:
//   lookahead: If non-zero, each iterator's seek operation will start the
//     search from the previously visited record (doing at most 'lookahead'
//     steps). This is an optimization for the access pattern including many
//     seeks with consecutive keys.
class SkipListFactory : public MemTableRepFactory {
 public:
  explicit SkipListFactory(size_t lookahead = 0) : lookahead_(lookahead) {}

  using MemTableRepFactory::CreateMemTableRep;
  virtual MemTableRep* CreateMemTableRep(const MemTableRep::KeyComparator&,
                                         Allocator*, const SliceTransform*,
                                         Logger* logger) override;
  virtual const char* Name() const override { return "SkipListFactory"; }

  bool IsInsertConcurrentlySupported() const override { return true; }

  bool CanHandleDuplicatedKey() const override { return true; }

 private:
  const size_t lookahead_;
};

}
