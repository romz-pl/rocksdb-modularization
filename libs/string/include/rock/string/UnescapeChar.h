#pragma once

#include <string>

namespace rocksdb {

// If the input char is an escaped char, it will return the its
// associated raw-char.  Otherwise, the function will simply return
// the original input char.
char UnescapeChar(const char c);

}
