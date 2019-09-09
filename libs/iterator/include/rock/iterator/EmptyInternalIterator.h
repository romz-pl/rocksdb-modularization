#pragma once

#include <rock/iterator/InternalIteratorBase.h>

namespace rocksdb {

template <class TValue = Slice>
class EmptyInternalIterator : public InternalIteratorBase<TValue> {
 public:
  explicit EmptyInternalIterator(const Status& s) : status_(s) {}
  bool Valid() const override { return false; }
  void Seek(const Slice& /*target*/) override {}
  void SeekForPrev(const Slice& /*target*/) override {}
  void SeekToFirst() override {}
  void SeekToLast() override {}
  void Next() override { assert(false); }
  void Prev() override { assert(false); }
  Slice key() const override {
    assert(false);
    return Slice();
  }
  TValue value() const override {
    assert(false);
    return TValue();
  }
  Status status() const override { return status_; }

 private:
  Status status_;
};


}
