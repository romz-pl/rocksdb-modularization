#pragma once

#include <rock/iterator/Cleanable.h>
#include <rock/slice/Slice.h>

namespace rocksdb {

/**
 * A Slice that can be pinned with some cleanup tasks, which will be run upon
 * ::Reset() or object destruction, whichever is invoked first. This can be used
 * to avoid memcpy by having the PinnableSlice object referring to the data
 * that is locked in the memory and release them after the data is consumed.
 */
class PinnableSlice : public Slice, public Cleanable {
 public:
  PinnableSlice() { buf_ = &self_space_; }
  explicit PinnableSlice(std::string* buf) { buf_ = buf; }

  // No copy constructor and copy assignment allowed.
  PinnableSlice(PinnableSlice&) = delete;
  PinnableSlice& operator=(PinnableSlice&) = delete;

  inline void PinSlice(const Slice& s, CleanupFunction f, void* arg1,
                       void* arg2) {
    assert(!pinned_);
    pinned_ = true;
    data_ = s.data();
    size_ = s.size();
    RegisterCleanup(f, arg1, arg2);
    assert(pinned_);
  }

  inline void PinSlice(const Slice& s, Cleanable* cleanable) {
    assert(!pinned_);
    pinned_ = true;
    data_ = s.data();
    size_ = s.size();
    cleanable->DelegateCleanupsTo(this);
    assert(pinned_);
  }

  inline void PinSelf(const Slice& slice) {
    assert(!pinned_);
    buf_->assign(slice.data(), slice.size());
    data_ = buf_->data();
    size_ = buf_->size();
    assert(!pinned_);
  }

  inline void PinSelf() {
    assert(!pinned_);
    data_ = buf_->data();
    size_ = buf_->size();
    assert(!pinned_);
  }

  void remove_suffix(size_t n) {
    assert(n <= size());
    if (pinned_) {
      size_ -= n;
    } else {
      buf_->erase(size() - n, n);
      PinSelf();
    }
  }

  void remove_prefix(size_t /*n*/) {
    assert(0);  // Not implemented
  }

  void Reset() {
    Cleanable::Reset();
    pinned_ = false;
    size_ = 0;
  }

  inline std::string* GetSelf() { return buf_; }

  inline bool IsPinned() { return pinned_; }

 private:
  friend class PinnableSlice4Test;
  std::string self_space_;
  std::string* buf_;
  bool pinned_ = false;
};

}  // namespace rocksdb
