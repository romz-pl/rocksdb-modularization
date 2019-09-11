#pragma once

#include <rock/status/Status.h>
#include <rock/statistics/StatsLevel.h>
#include <rock/statistics/Histograms.h>

#include <atomic>
#include <map>

namespace rocksdb {

struct HistogramData;

// Analyze the performance of a db by providing cumulative stats over time.
// Usage:
//  Options options;
//  options.statistics = rocksdb::CreateDBStatistics();
//  Status s = DB::Open(options, kDBPath, &db);
//  ...
//  options.statistics->getTickerCount(NUMBER_BLOCK_COMPRESSED);
//  HistogramData hist;
//  options.statistics->histogramData(FLUSH_TIME, &hist);
class Statistics {
 public:
  virtual ~Statistics() {}
  static const char* Type() { return "Statistics"; }
  virtual uint64_t getTickerCount(uint32_t tickerType) const = 0;
  virtual void histogramData(uint32_t type,
                             HistogramData* const data) const = 0;
  virtual std::string getHistogramString(uint32_t /*type*/) const { return ""; }
  virtual void recordTick(uint32_t tickerType, uint64_t count = 0) = 0;
  virtual void setTickerCount(uint32_t tickerType, uint64_t count) = 0;
  virtual uint64_t getAndResetTickerCount(uint32_t tickerType) = 0;
  virtual void reportTimeToHistogram(uint32_t histogramType, uint64_t time) {
    if (get_stats_level() <= StatsLevel::kExceptTimers) {
      return;
    }
    recordInHistogram(histogramType, time);
  }
  // The function is here only for backward compatibility reason.
  // Users implementing their own Statistics class should override
  // recordInHistogram() instead and leave measureTime() as it is.
  virtual void measureTime(uint32_t /*histogramType*/, uint64_t /*time*/) {
    // This is not supposed to be called.
    assert(false);
  }
  virtual void recordInHistogram(uint32_t histogramType, uint64_t time) {
    // measureTime() is the old and inaccurate function name.
    // To keep backward compatible. If users implement their own
    // statistics, which overrides meareTime() but doesn't override
    // this function. We forward to measureTime().
    measureTime(histogramType, time);
  }

  // Resets all ticker and histogram stats
  virtual Status Reset() { return Status::NotSupported("Not implemented"); }

  // String representation of the statistic object.
  virtual std::string ToString() const {
    // Do nothing by default
    return std::string("ToString(): not implemented");
  }

  virtual bool getTickerMap(std::map<std::string, uint64_t>*) const {
    // Do nothing by default
    return false;
  }

  // Override this function to disable particular histogram collection
  virtual bool HistEnabledForType(uint32_t type) const {
    return type < HISTOGRAM_ENUM_MAX;
  }
  void set_stats_level(StatsLevel sl) {
    stats_level_.store(sl, std::memory_order_relaxed);
  }
  StatsLevel get_stats_level() const {
    return stats_level_.load(std::memory_order_relaxed);
  }

 private:
  std::atomic<StatsLevel> stats_level_{kExceptDetailedTimers};
};


}
