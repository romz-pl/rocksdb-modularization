#include <rock/statistics/HistogramImpl.h>

#include <cstring>

#include "util/cast_util.h"

namespace rocksdb {

void HistogramImpl::Clear() {
  std::lock_guard<std::mutex> lock(mutex_);
  stats_.Clear();
}

bool HistogramImpl::Empty() const {
  return stats_.Empty();
}

void HistogramImpl::Add(uint64_t value) {
  stats_.Add(value);
}

void HistogramImpl::Merge(const Histogram& other) {
  if (strcmp(Name(), other.Name()) == 0) {
    Merge(
        *static_cast_with_check<const HistogramImpl, const Histogram>(&other));
  }
}

void HistogramImpl::Merge(const HistogramImpl& other) {
  std::lock_guard<std::mutex> lock(mutex_);
  stats_.Merge(other.stats_);
}

double HistogramImpl::Median() const {
  return stats_.Median();
}

double HistogramImpl::Percentile(double p) const {
  return stats_.Percentile(p);
}

double HistogramImpl::Average() const {
  return stats_.Average();
}

double HistogramImpl::StandardDeviation() const {
 return stats_.StandardDeviation();
}

std::string HistogramImpl::ToString() const {
  return stats_.ToString();
}

void HistogramImpl::Data(HistogramData * const data) const {
  stats_.Data(data);
}


}
