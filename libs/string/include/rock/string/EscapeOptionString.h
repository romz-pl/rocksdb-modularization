#pragma once

#include <string>

namespace rocksdb {

// Converts a raw string to an escaped string.  Escaped-characters are
// defined via the isSpecialChar() function.  When a char in the input
// string "raw_string" is classified as a special characters, then it
// will be prefixed by '\' in the output.
//
// It's inverse function is UnescapeOptionString().
// @param raw_string the input string
// @return the '\' escaped string of the input "raw_string"
// @see isSpecialChar, UnescapeOptionString
std::string EscapeOptionString(const std::string& raw_string);

}
