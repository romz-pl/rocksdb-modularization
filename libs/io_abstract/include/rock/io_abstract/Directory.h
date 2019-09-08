#pragma once

#include <rock/status/Status.h>

namespace rocksdb {

// Directory object represents collection of files and implements
// filesystem operations that can be executed on directories.
class Directory {
 public:
  virtual ~Directory() {}
  // Fsync directory. Can be called concurrently from multiple threads.
  virtual Status Fsync() = 0;

  virtual size_t GetUniqueId(char* /*id*/, size_t /*max_size*/) const {
    return 0;
  }

  // If you're adding methods here, remember to add them to
  // DirectoryWrapper too.
};

}
