#pragma once

#include <rock/iterator/InternalIteratorBase.h>

#undef TEST_SYNC_POINT
#define TEST_SYNC_POINT(x)

namespace rocksdb {

// A internal wrapper class with an interface similar to Iterator that caches
// the valid() and key() results for an underlying iterator.
// This can help avoid virtual function calls and also gives better
// cache locality.
template <class TValue = Slice>
class IteratorWrapperBase {
 public:
  IteratorWrapperBase() : iter_(nullptr), valid_(false) {}
  explicit IteratorWrapperBase(InternalIteratorBase<TValue>* _iter)
      : iter_(nullptr) {
    Set(_iter);
  }
  ~IteratorWrapperBase() {}
  InternalIteratorBase<TValue>* iter() const { return iter_; }

  // Set the underlying Iterator to _iter and return
  // previous underlying Iterator.
  InternalIteratorBase<TValue>* Set(InternalIteratorBase<TValue>* _iter) {
    InternalIteratorBase<TValue>* old_iter = iter_;

    iter_ = _iter;
    if (iter_ == nullptr) {
      valid_ = false;
    } else {
      Update();
    }
    return old_iter;
  }

  void DeleteIter(bool is_arena_mode) {
    if (iter_) {
      if (!is_arena_mode) {
        delete iter_;
      } else {
        iter_->~InternalIteratorBase<TValue>();
      }
    }
  }

  // Iterator interface methods
  bool Valid() const        { return valid_; }
  Slice key() const {
    assert(Valid());
    return result_.key;
  }
  TValue value() const {
    assert(Valid());
    return iter_->value();
  }
  // Methods below require iter() != nullptr
  Status status() const     { assert(iter_); return iter_->status(); }
  void Next() {
    assert(iter_);
    valid_ = iter_->NextAndGetResult(&result_);
    assert(!valid_ || iter_->status().ok());
  }
  void Prev()               { assert(iter_); iter_->Prev();        Update(); }
  void Seek(const Slice& k) {
    TEST_SYNC_POINT("IteratorWrapper::Seek:0");
    assert(iter_);
    iter_->Seek(k);
    Update();
  }
  void SeekForPrev(const Slice& k) {
    assert(iter_);
    iter_->SeekForPrev(k);
    Update();
  }
  void SeekToFirst()        { assert(iter_); iter_->SeekToFirst(); Update(); }
  void SeekToLast()         { assert(iter_); iter_->SeekToLast();  Update(); }

  bool MayBeOutOfLowerBound() {
    assert(Valid());
    return iter_->MayBeOutOfLowerBound();
  }

  bool MayBeOutOfUpperBound() {
    assert(Valid());
    return result_.may_be_out_of_upper_bound;
  }

  void SetPinnedItersMgr(PinnedIteratorsManager* pinned_iters_mgr) {
    assert(iter_);
    iter_->SetPinnedItersMgr(pinned_iters_mgr);
  }
  bool IsKeyPinned() const {
    assert(Valid());
    return iter_->IsKeyPinned();
  }
  bool IsValuePinned() const {
    assert(Valid());
    return iter_->IsValuePinned();
  }

 private:
  void Update() {
    valid_ = iter_->Valid();
    if (valid_) {
      assert(iter_->status().ok());
      result_.key = iter_->key();
      result_.may_be_out_of_upper_bound = true;
    }
  }

  InternalIteratorBase<TValue>* iter_;
  IterateResult result_;
  bool valid_;
};


}
