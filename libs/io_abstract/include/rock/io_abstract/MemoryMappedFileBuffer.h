#pragma once

#include <rock/status/Status.h>

namespace rocksdb {

// MemoryMappedFileBuffer object represents a memory-mapped file's raw buffer.
// Subclasses should release the mapping upon destruction.
class MemoryMappedFileBuffer {
 public:
  MemoryMappedFileBuffer(void* _base, size_t _length)
      : base_(_base), length_(_length) {}

  virtual ~MemoryMappedFileBuffer() = 0;

  // We do not want to unmap this twice. We can make this class
  // movable if desired, however, since
  MemoryMappedFileBuffer(const MemoryMappedFileBuffer&) = delete;
  MemoryMappedFileBuffer& operator=(const MemoryMappedFileBuffer&) = delete;

  void* GetBase() const { return base_; }
  size_t GetLen() const { return length_; }

 protected:
  void* base_;
  const size_t length_;
};


}
