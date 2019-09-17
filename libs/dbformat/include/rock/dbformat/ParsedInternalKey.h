#pragma once

#include <rock/slice/Slice.h>
#include <rock/sn/SequenceNumber.h>
#include <rock/dbformat/ValueType.h>
#include <rock/dbformat/kMaxSequenceNumber.h>

namespace rocksdb {

// The data structure that represents an internal key in the way that user_key,
// sequence number and type are stored in separated forms.
struct ParsedInternalKey {
  Slice user_key;
  SequenceNumber sequence;
  ValueType type;

  ParsedInternalKey()
      : sequence(kMaxSequenceNumber)  // Make code analyzer happy
  {}  // Intentionally left uninitialized (for speed)
  ParsedInternalKey(const Slice& u, const SequenceNumber& seq, ValueType t)
      : user_key(u), sequence(seq), type(t) {}
  std::string DebugString(bool hex = false) const;

  void clear() {
    user_key.clear();
    sequence = 0;
    type = kTypeDeletion;
  }
};


}
