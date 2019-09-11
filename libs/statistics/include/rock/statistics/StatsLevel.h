#pragma once

#include <cstdint>

namespace rocksdb {

// StatsLevel can be used to reduce statistics overhead by skipping certain
// types of stats in the stats collection process.
// Usage:
//   options.statistics->set_stats_level(StatsLevel::kExceptTimeForMutex);
enum StatsLevel : uint8_t {
  // Disable timer stats, and skip histogram stats
  kExceptHistogramOrTimers,
  // Skip timer stats
  kExceptTimers,
  // Collect all stats except time inside mutex lock AND time spent on
  // compression.
  kExceptDetailedTimers,
  // Collect all stats except the counters requiring to get time inside the
  // mutex lock.
  kExceptTimeForMutex,
  // Collect all stats, including measuring duration of mutex operations.
  // If getting time is expensive on the platform to run, it can
  // reduce scalability to more threads, especially for writes.
  kAll,
};

}
