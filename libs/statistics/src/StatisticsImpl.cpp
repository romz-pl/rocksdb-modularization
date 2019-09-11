#include <rock/statistics/StatisticsImpl.h>

#include <rock/mutex/MutexLock.h>
#include <rock/statistics/TickersNameMap.h>
#include <rock/statistics/HistogramsNameMap.h>
#include <rock/statistics/HistogramData.h>

#include <cinttypes>

namespace rocksdb {

StatisticsImpl::StatisticsImpl(std::shared_ptr<Statistics> stats)
    : stats_(std::move(stats)) {}

StatisticsImpl::~StatisticsImpl() {}

uint64_t StatisticsImpl::getTickerCount(uint32_t tickerType) const {
  MutexLock lock(&aggregate_lock_);
  return getTickerCountLocked(tickerType);
}

uint64_t StatisticsImpl::getTickerCountLocked(uint32_t tickerType) const {
  assert(tickerType < TICKER_ENUM_MAX);
  uint64_t res = 0;
  for (size_t core_idx = 0; core_idx < per_core_stats_.Size(); ++core_idx) {
    res += per_core_stats_.AccessAtCore(core_idx)->tickers_[tickerType];
  }
  return res;
}

void StatisticsImpl::histogramData(uint32_t histogramType,
                                   HistogramData* const data) const {
  MutexLock lock(&aggregate_lock_);
  getHistogramImplLocked(histogramType)->Data(data);
}

std::unique_ptr<HistogramImpl> StatisticsImpl::getHistogramImplLocked(
    uint32_t histogramType) const {
  assert(histogramType < HISTOGRAM_ENUM_MAX);
  std::unique_ptr<HistogramImpl> res_hist(new HistogramImpl());
  for (size_t core_idx = 0; core_idx < per_core_stats_.Size(); ++core_idx) {
    res_hist->Merge(
        per_core_stats_.AccessAtCore(core_idx)->histograms_[histogramType]);
  }
  return res_hist;
}

std::string StatisticsImpl::getHistogramString(uint32_t histogramType) const {
  MutexLock lock(&aggregate_lock_);
  return getHistogramImplLocked(histogramType)->ToString();
}



void StatisticsImpl::setTickerCount(uint32_t tickerType, uint64_t count) {
  {
    MutexLock lock(&aggregate_lock_);
    setTickerCountLocked(tickerType, count);
  }
  if (stats_ && tickerType < TICKER_ENUM_MAX) {
    stats_->setTickerCount(tickerType, count);
  }
}

void StatisticsImpl::setTickerCountLocked(uint32_t tickerType, uint64_t count) {
  assert(tickerType < TICKER_ENUM_MAX);
  for (size_t core_idx = 0; core_idx < per_core_stats_.Size(); ++core_idx) {
    if (core_idx == 0) {
      per_core_stats_.AccessAtCore(core_idx)->tickers_[tickerType] = count;
    } else {
      per_core_stats_.AccessAtCore(core_idx)->tickers_[tickerType] = 0;
    }
  }
}

uint64_t StatisticsImpl::getAndResetTickerCount(uint32_t tickerType) {
  uint64_t sum = 0;
  {
    MutexLock lock(&aggregate_lock_);
    assert(tickerType < TICKER_ENUM_MAX);
    for (size_t core_idx = 0; core_idx < per_core_stats_.Size(); ++core_idx) {
      sum +=
          per_core_stats_.AccessAtCore(core_idx)->tickers_[tickerType].exchange(
              0, std::memory_order_relaxed);
    }
  }
  if (stats_ && tickerType < TICKER_ENUM_MAX) {
    stats_->setTickerCount(tickerType, 0);
  }
  return sum;
}

void StatisticsImpl::recordTick(uint32_t tickerType, uint64_t count) {
  assert(tickerType < TICKER_ENUM_MAX);
  per_core_stats_.Access()->tickers_[tickerType].fetch_add(
      count, std::memory_order_relaxed);
  if (stats_ && tickerType < TICKER_ENUM_MAX) {
    stats_->recordTick(tickerType, count);
  }
}

void StatisticsImpl::recordInHistogram(uint32_t histogramType, uint64_t value) {
  assert(histogramType < HISTOGRAM_ENUM_MAX);
  if (get_stats_level() <= StatsLevel::kExceptHistogramOrTimers) {
    return;
  }
  per_core_stats_.Access()->histograms_[histogramType].Add(value);
  if (stats_ && histogramType < HISTOGRAM_ENUM_MAX) {
    stats_->recordInHistogram(histogramType, value);
  }
}

Status StatisticsImpl::Reset() {
  MutexLock lock(&aggregate_lock_);
  for (uint32_t i = 0; i < TICKER_ENUM_MAX; ++i) {
    setTickerCountLocked(i, 0);
  }
  for (uint32_t i = 0; i < HISTOGRAM_ENUM_MAX; ++i) {
    for (size_t core_idx = 0; core_idx < per_core_stats_.Size(); ++core_idx) {
      per_core_stats_.AccessAtCore(core_idx)->histograms_[i].Clear();
    }
  }
  return Status::OK();
}

namespace {

// a buffer size used for temp string buffers
const int kTmpStrBufferSize = 200;

} // namespace

std::string StatisticsImpl::ToString() const {
  MutexLock lock(&aggregate_lock_);
  std::string res;
  res.reserve(20000);
  for (const auto& t : TickersNameMap) {
    assert(t.first < TICKER_ENUM_MAX);
    char buffer[kTmpStrBufferSize];
    snprintf(buffer, kTmpStrBufferSize, "%s COUNT : %" PRIu64 "\n",
             t.second.c_str(), getTickerCountLocked(t.first));
    res.append(buffer);
  }
  for (const auto& h : HistogramsNameMap) {
    assert(h.first < HISTOGRAM_ENUM_MAX);
    char buffer[kTmpStrBufferSize];
    HistogramData hData;
    getHistogramImplLocked(h.first)->Data(&hData);
    // don't handle failures - buffer should always be big enough and arguments
    // should be provided correctly
    int ret =
        snprintf(buffer, kTmpStrBufferSize,
                 "%s P50 : %f P95 : %f P99 : %f P100 : %f COUNT : %" PRIu64
                 " SUM : %" PRIu64 "\n",
                 h.second.c_str(), hData.median, hData.percentile95,
                 hData.percentile99, hData.max, hData.count, hData.sum);
    if (ret < 0 || ret >= kTmpStrBufferSize) {
      assert(false);
      continue;
    }
    res.append(buffer);
  }
  res.shrink_to_fit();
  return res;
}

bool StatisticsImpl::getTickerMap(
    std::map<std::string, uint64_t>* stats_map) const {
  assert(stats_map);
  if (!stats_map) return false;
  stats_map->clear();
  MutexLock lock(&aggregate_lock_);
  for (const auto& t : TickersNameMap) {
    assert(t.first < TICKER_ENUM_MAX);
    (*stats_map)[t.second.c_str()] = getTickerCountLocked(t.first);
  }
  return true;
}

bool StatisticsImpl::HistEnabledForType(uint32_t type) const {
  return type < HISTOGRAM_ENUM_MAX;
}

}
