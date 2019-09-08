#pragma once

#include <string>

namespace rocksdb {

// The inverse function of EscapeOptionString.  It converts
// an '\' escaped string back to a raw string.
//
// @param escaped_string the input '\' escaped string
// @return the raw string of the input "escaped_string"
std::string UnescapeOptionString(const std::string& escaped_string);

}
