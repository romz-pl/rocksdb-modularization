#pragma once

namespace rocksdb {

class Slice;
struct ParsedInternalKey;

// Attempt to parse an internal key from "internal_key".  On success,
// stores the parsed data in "*result", and returns true.
//
// On error, returns false, leaves "*result" in an undefined state.
extern bool ParseInternalKey(const Slice& internal_key,
                             ParsedInternalKey* result);

}
