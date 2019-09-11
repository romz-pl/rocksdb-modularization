#pragma once

#include <rock/statistics/Histograms.h>

namespace rocksdb {

enum HistogramsInternal : uint32_t {
  INTERNAL_HISTOGRAM_START = HISTOGRAM_ENUM_MAX,
  INTERNAL_HISTOGRAM_ENUM_MAX
};

}
