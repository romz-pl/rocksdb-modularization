#pragma once

#include <rock/status/Status.h>
#include <rock/io_abstract/SequentialFile.h>

namespace rocksdb {

class SequentialFileWrapper : public SequentialFile {
 public:
  explicit SequentialFileWrapper(SequentialFile* target) : target_(target) {}

  Status Read(size_t n, Slice* result, char* scratch) override {
    return target_->Read(n, result, scratch);
  }
  Status Skip(uint64_t n) override { return target_->Skip(n); }
  bool use_direct_io() const override { return target_->use_direct_io(); }
  size_t GetRequiredBufferAlignment() const override {
    return target_->GetRequiredBufferAlignment();
  }
  Status InvalidateCache(size_t offset, size_t length) override {
    return target_->InvalidateCache(offset, length);
  }
  Status PositionedRead(uint64_t offset, size_t n, Slice* result,
                        char* scratch) override {
    return target_->PositionedRead(offset, n, result, scratch);
  }

 private:
  SequentialFile* target_;
};


}
