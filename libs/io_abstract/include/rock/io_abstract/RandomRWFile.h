#pragma once

#include <rock/status/Status.h>
#include <rock/io_abstract/kDefaultPageSize.h>

namespace rocksdb {

// A file abstraction for random reading and writing.
class RandomRWFile {
 public:
  RandomRWFile() {}
  virtual ~RandomRWFile() {}

  // Indicates if the class makes use of direct I/O
  // If false you must pass aligned buffer to Write()
  virtual bool use_direct_io() const { return false; }

  // Use the returned alignment value to allocate
  // aligned buffer for Direct I/O
  virtual size_t GetRequiredBufferAlignment() const { return kDefaultPageSize; }

  // Write bytes in `data` at  offset `offset`, Returns Status::OK() on success.
  // Pass aligned buffer when use_direct_io() returns true.
  virtual Status Write(uint64_t offset, const Slice& data) = 0;

  // Read up to `n` bytes starting from offset `offset` and store them in
  // result, provided `scratch` size should be at least `n`.
  // Returns Status::OK() on success.
  virtual Status Read(uint64_t offset, size_t n, Slice* result,
                      char* scratch) const = 0;

  virtual Status Flush() = 0;

  virtual Status Sync() = 0;

  virtual Status Fsync() { return Sync(); }

  virtual Status Close() = 0;

  // If you're adding methods here, remember to add them to
  // RandomRWFileWrapper too.

  // No copying allowed
  RandomRWFile(const RandomRWFile&) = delete;
  RandomRWFile& operator=(const RandomRWFile&) = delete;
};


}
