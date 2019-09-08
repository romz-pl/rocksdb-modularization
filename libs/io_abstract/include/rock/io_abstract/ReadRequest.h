#pragma once

#include <rock/status/Status.h>

namespace rocksdb {

// A read IO request structure for use in MultiRead
struct ReadRequest {
  // File offset in bytes
  uint64_t offset;

  // Length to read in bytes
  size_t len;

  // A buffer that MultiRead()  can optionally place data in. It can
  // ignore this and allocate its own buffer
  char* scratch;

  // Output parameter set by MultiRead() to point to the data buffer, and
  // the number of valid bytes
  Slice result;

  // Status of read
  Status status;
};

}
