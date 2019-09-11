#pragma once

#include <rock/statistics/Histograms.h>

#include <string>
#include <vector>

namespace rocksdb {

extern const std::vector<std::pair<Histograms, std::string>> HistogramsNameMap;

}
