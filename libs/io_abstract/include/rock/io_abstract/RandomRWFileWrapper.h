#pragma once

#include <rock/status/Status.h>
#include <rock/io_abstract/RandomRWFile.h>

namespace rocksdb {

class RandomRWFileWrapper : public RandomRWFile {
 public:
  explicit RandomRWFileWrapper(RandomRWFile* target) : target_(target) {}

  bool use_direct_io() const override { return target_->use_direct_io(); }
  size_t GetRequiredBufferAlignment() const override {
    return target_->GetRequiredBufferAlignment();
  }
  Status Write(uint64_t offset, const Slice& data) override {
    return target_->Write(offset, data);
  }
  Status Read(uint64_t offset, size_t n, Slice* result,
              char* scratch) const override {
    return target_->Read(offset, n, result, scratch);
  }
  Status Flush() override { return target_->Flush(); }
  Status Sync() override { return target_->Sync(); }
  Status Fsync() override { return target_->Fsync(); }
  Status Close() override { return target_->Close(); }

 private:
  RandomRWFile* target_;
};

}
