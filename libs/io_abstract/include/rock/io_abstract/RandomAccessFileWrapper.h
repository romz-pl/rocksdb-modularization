#pragma once

#include <rock/status/Status.h>
#include <rock/io_abstract/RandomAccessFile.h>

namespace rocksdb {


class RandomAccessFileWrapper : public RandomAccessFile {
 public:
  explicit RandomAccessFileWrapper(RandomAccessFile* target)
      : target_(target) {}

  Status Read(uint64_t offset, size_t n, Slice* result,
              char* scratch) const override {
    return target_->Read(offset, n, result, scratch);
  }
  Status MultiRead(ReadRequest* reqs, size_t num_reqs) override {
    return target_->MultiRead(reqs, num_reqs);
  }
  Status Prefetch(uint64_t offset, size_t n) override {
    return target_->Prefetch(offset, n);
  }
  size_t GetUniqueId(char* id, size_t max_size) const override {
    return target_->GetUniqueId(id, max_size);
  };
  void Hint(AccessPattern pattern) override { target_->Hint(pattern); }
  bool use_direct_io() const override { return target_->use_direct_io(); }
  size_t GetRequiredBufferAlignment() const override {
    return target_->GetRequiredBufferAlignment();
  }
  Status InvalidateCache(size_t offset, size_t length) override {
    return target_->InvalidateCache(offset, length);
  }

 private:
  RandomAccessFile* target_;
};

}
