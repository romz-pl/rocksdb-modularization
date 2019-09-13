#pragma once

#ifndef ROCKSDB_LITE

#include <rock/memtable/MemTableRepFactory.h>

namespace rocksdb {

// This creates MemTableReps that are backed by an std::vector. On iteration,
// the vector is sorted. This is useful for workloads where iteration is very
// rare and writes are generally not issued after reads begin.
//
// Parameters:
//   count: Passed to the constructor of the underlying std::vector of each
//     VectorRep. On initialization, the underlying array will be at least count
//     bytes reserved for usage.
class VectorRepFactory : public MemTableRepFactory {
  const size_t count_;

 public:
  explicit VectorRepFactory(size_t count = 0) : count_(count) {}

  using MemTableRepFactory::CreateMemTableRep;
  virtual MemTableRep* CreateMemTableRep(const MemTableRep::KeyComparator&,
                                         Allocator*, const SliceTransform*,
                                         Logger* logger) override;

  virtual const char* Name() const override { return "VectorRepFactory"; }
};


}

#endif  // ROCKSDB_LITE
