#pragma once

#include <rock/status/Status.h>
#include <rock/io_abstract/Directory.h>

namespace rocksdb {

class DirectoryWrapper : public Directory {
 public:
  explicit DirectoryWrapper(Directory* target) : target_(target) {}

  Status Fsync() override { return target_->Fsync(); }
  size_t GetUniqueId(char* id, size_t max_size) const override {
    return target_->GetUniqueId(id, max_size);
  }

 private:
  Directory* target_;
};

}
