#pragma once

#include <rock/status/Status.h>
#include <rock/io_abstract/WritableFile.h>

namespace rocksdb {
class WritableFileWrapper : public WritableFile {
 public:
  explicit WritableFileWrapper(WritableFile* t) : target_(t) {}

  Status Append(const Slice& data) override { return target_->Append(data); }
  Status PositionedAppend(const Slice& data, uint64_t offset) override {
    return target_->PositionedAppend(data, offset);
  }
  Status Truncate(uint64_t size) override { return target_->Truncate(size); }
  Status Close() override { return target_->Close(); }
  Status Flush() override { return target_->Flush(); }
  Status Sync() override { return target_->Sync(); }
  Status Fsync() override { return target_->Fsync(); }
  bool IsSyncThreadSafe() const override { return target_->IsSyncThreadSafe(); }

  bool use_direct_io() const override { return target_->use_direct_io(); }

  size_t GetRequiredBufferAlignment() const override {
    return target_->GetRequiredBufferAlignment();
  }

  void SetIOPriority(IOPriority pri) override {
    target_->SetIOPriority(pri);
  }

  IOPriority GetIOPriority() override { return target_->GetIOPriority(); }

  void SetWriteLifeTimeHint(WriteLifeTimeHint hint) override {
    target_->SetWriteLifeTimeHint(hint);
  }

  WriteLifeTimeHint GetWriteLifeTimeHint() override {
    return target_->GetWriteLifeTimeHint();
  }

  uint64_t GetFileSize() override { return target_->GetFileSize(); }

  void SetPreallocationBlockSize(size_t size) override {
    target_->SetPreallocationBlockSize(size);
  }

  void GetPreallocationStatus(size_t* block_size,
                              size_t* last_allocated_block) override {
    target_->GetPreallocationStatus(block_size, last_allocated_block);
  }

  size_t GetUniqueId(char* id, size_t max_size) const override {
    return target_->GetUniqueId(id, max_size);
  }

  Status InvalidateCache(size_t offset, size_t length) override {
    return target_->InvalidateCache(offset, length);
  }

  Status RangeSync(uint64_t offset, uint64_t nbytes) override {
    return target_->RangeSync(offset, nbytes);
  }

  void PrepareWrite(size_t offset, size_t len) override {
    target_->PrepareWrite(offset, len);
  }

  Status Allocate(uint64_t offset, uint64_t len) override {
    return target_->Allocate(offset, len);
  }

 private:
  WritableFile* target_;
};


}
