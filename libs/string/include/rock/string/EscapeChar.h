#pragma once

#include <string>

namespace rocksdb {

// If the input char is a control char, it will return the its
// associated escaped char.  Otherwise, the function will simply return
// the original input char.
char EscapeChar(const char c);

}
