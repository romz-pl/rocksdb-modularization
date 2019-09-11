#pragma once

#include <rock/statistics/Statistics.h>

namespace rocksdb {

inline void RecordTimeToHistogram(Statistics* statistics,
                                  uint32_t histogram_type, uint64_t value) {
  if (statistics) {
    statistics->reportTimeToHistogram(histogram_type, value);
  }
}

}
