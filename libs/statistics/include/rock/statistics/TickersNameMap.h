#pragma once

#include <rock/statistics/Tickers.h>

#include <vector>
#include <string>

namespace rocksdb {

// The order of items listed in  Tickers should be the same as
// the order listed in TickersNameMap
extern const std::vector<std::pair<Tickers, std::string>> TickersNameMap;

}
