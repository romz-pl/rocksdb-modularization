#include <rock/cache_lru/kDefaultToAdaptiveMutex.h>

namespace rocksdb {

// We want to give users opportunity to default all the mutexes to adaptive if
// not specified otherwise. This enables a quick way to conduct various
// performance related experiements.
//
// NB! Support for adaptive mutexes is turned on by definining
// ROCKSDB_PTHREAD_ADAPTIVE_MUTEX during the compilation. If you use RocksDB
// build environment then this happens automatically; otherwise it's up to the
// consumer to define the identifier.
#ifdef ROCKSDB_DEFAULT_TO_ADAPTIVE_MUTEX

extern const bool kDefaultToAdaptiveMutex = true;

#else

extern const bool kDefaultToAdaptiveMutex = false;

#endif

}
