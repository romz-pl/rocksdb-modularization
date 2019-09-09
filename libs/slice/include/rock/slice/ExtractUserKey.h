#pragma once

namespace rocksdb {

class Slice;

// Returns the user key portion of an internal key.
Slice ExtractUserKey(const Slice& internal_key);

}
