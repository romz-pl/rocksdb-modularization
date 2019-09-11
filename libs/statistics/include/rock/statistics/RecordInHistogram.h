#pragma once

#include <rock/statistics/Statistics.h>

namespace rocksdb {

// Utility functions
inline void RecordInHistogram(Statistics* statistics, uint32_t histogram_type,
                              uint64_t value) {
  if (statistics) {
    statistics->recordInHistogram(histogram_type, value);
  }
}

}
