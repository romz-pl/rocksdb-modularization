#pragma once

#include <string>

namespace rocksdb {

// Returns true if the input char "c" is considered as a special character
// that will be escaped when EscapeOptionString() is called.
//
// @param c the input char
// @return true if the input char "c" is considered as a special character.
// @see EscapeOptionString
bool isSpecialChar(const char c);

}
