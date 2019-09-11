#pragma once

#include <memory>

namespace rocksdb {

class Statistics;

// Create a concrete DBStatistics object
std::shared_ptr<Statistics> CreateDBStatistics();

}
