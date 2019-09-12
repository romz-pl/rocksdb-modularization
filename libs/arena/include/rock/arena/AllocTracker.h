#pragma once

#include <atomic>
#include <cstddef>

namespace rocksdb {

class WriteBufferManager;

class AllocTracker {
 public:
  explicit AllocTracker(WriteBufferManager* write_buffer_manager);
  ~AllocTracker();
  void Allocate(size_t bytes);
  // Call when we're finished allocating memory so we can free it from
  // the write buffer's limit.
  void DoneAllocating();

  void FreeMem();

  bool is_freed() const { return write_buffer_manager_ == nullptr || freed_; }

 private:
  WriteBufferManager* write_buffer_manager_;
  std::atomic<size_t> bytes_allocated_;
  bool done_allocating_;
  bool freed_;

  // No copying allowed
  AllocTracker(const AllocTracker&);
  void operator=(const AllocTracker&);
};

}
