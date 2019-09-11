#include <rock/statistics/CreateDBStatistics.h>

#include <rock/statistics/StatisticsImpl.h>

namespace rocksdb {

std::shared_ptr<Statistics> CreateDBStatistics() {
  return std::make_shared<StatisticsImpl>(nullptr);
}

}
