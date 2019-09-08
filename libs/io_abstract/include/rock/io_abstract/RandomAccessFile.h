#pragma once

#include <rock/status/Status.h>
#include <rock/io_abstract/ReadRequest.h>
#include <rock/io_abstract/kDefaultPageSize.h>

namespace rocksdb {


// A file abstraction for randomly reading the contents of a file.
class RandomAccessFile {
 public:
  RandomAccessFile() {}
  virtual ~RandomAccessFile();

  // Read up to "n" bytes from the file starting at "offset".
  // "scratch[0..n-1]" may be written by this routine.  Sets "*result"
  // to the data that was read (including if fewer than "n" bytes were
  // successfully read).  May set "*result" to point at data in
  // "scratch[0..n-1]", so "scratch[0..n-1]" must be live when
  // "*result" is used.  If an error was encountered, returns a non-OK
  // status.
  //
  // Safe for concurrent use by multiple threads.
  // If Direct I/O enabled, offset, n, and scratch should be aligned properly.
  virtual Status Read(uint64_t offset, size_t n, Slice* result,
                      char* scratch) const = 0;

  // Readahead the file starting from offset by n bytes for caching.
  virtual Status Prefetch(uint64_t /*offset*/, size_t /*n*/) {
    return Status::OK();
  }

  // Read a bunch of blocks as described by reqs. The blocks can
  // optionally be read in parallel. This is a synchronous call, i.e it
  // should return after all reads have completed. The reads will be
  // non-overlapping. If the function return Status is not ok, status of
  // individual requests will be ignored and return status will be assumed
  // for all read requests. The function return status is only meant for any
  // any errors that occur before even processing specific read requests
  virtual Status MultiRead(ReadRequest* reqs, size_t num_reqs) {
    assert(reqs != nullptr);
    for (size_t i = 0; i < num_reqs; ++i) {
      ReadRequest& req = reqs[i];
      req.status = Read(req.offset, req.len, &req.result, req.scratch);
    }
    return Status::OK();
  }

  // Tries to get an unique ID for this file that will be the same each time
  // the file is opened (and will stay the same while the file is open).
  // Furthermore, it tries to make this ID at most "max_size" bytes. If such an
  // ID can be created this function returns the length of the ID and places it
  // in "id"; otherwise, this function returns 0, in which case "id"
  // may not have been modified.
  //
  // This function guarantees, for IDs from a given environment, two unique ids
  // cannot be made equal to each other by adding arbitrary bytes to one of
  // them. That is, no unique ID is the prefix of another.
  //
  // This function guarantees that the returned ID will not be interpretable as
  // a single varint.
  //
  // Note: these IDs are only valid for the duration of the process.
  virtual size_t GetUniqueId(char* /*id*/, size_t /*max_size*/) const {
    return 0;  // Default implementation to prevent issues with backwards
               // compatibility.
  }

  enum AccessPattern { NORMAL, RANDOM, SEQUENTIAL, WILLNEED, DONTNEED };

  virtual void Hint(AccessPattern /*pattern*/) {}

  // Indicates the upper layers if the current RandomAccessFile implementation
  // uses direct IO.
  virtual bool use_direct_io() const { return false; }

  // Use the returned alignment value to allocate
  // aligned buffer for Direct I/O
  virtual size_t GetRequiredBufferAlignment() const { return kDefaultPageSize; }

  // Remove any kind of caching of data from the offset to offset+length
  // of this file. If the length is 0, then it refers to the end of file.
  // If the system is not caching the file contents, then this is a noop.
  virtual Status InvalidateCache(size_t /*offset*/, size_t /*length*/) {
    return Status::NotSupported("InvalidateCache not supported.");
  }

  // If you're adding methods here, remember to add them to
  // RandomAccessFileWrapper too.
};


}
