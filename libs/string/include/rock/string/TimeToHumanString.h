#pragma once

#include <string>

namespace rocksdb {

// Return a human-readable version of unix time
// ex: 1562116015 -> "Tue Jul  2 18:06:55 2019"
extern std::string TimeToHumanString(int unixtime);

}
