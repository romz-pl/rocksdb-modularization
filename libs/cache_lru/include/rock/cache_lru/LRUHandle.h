#pragma once

#include <rock/slice/Slice.h>
#include <rock/cache/Cache.h>

namespace rocksdb {

// An entry is a variable length heap-allocated structure.
// Entries are referenced by cache and/or by any external entity.
// The cache keeps all its entries in a hash table. Some elements
// are also stored on LRU list.
//
// LRUHandle can be in these states:
// 1. Referenced externally AND in hash table.
//    In that case the entry is *not* in the LRU list
//    (refs >= 1 && in_cache == true)
// 2. Not referenced externally AND in hash table.
//    In that case the entry is in the LRU list and can be freed.
//    (refs == 0 && in_cache == true)
// 3. Referenced externally AND not in hash table.
//    In that case the entry is not in the LRU list and not in hash table.
//    The entry can be freed when refs becomes 0.
//    (refs >= 1 && in_cache == false)
//
// All newly created LRUHandles are in state 1. If you call
// LRUCacheShard::Release on entry in state 1, it will go into state 2.
// To move from state 1 to state 3, either call LRUCacheShard::Erase or
// LRUCacheShard::Insert with the same key (but possibly different value).
// To move from state 2 to state 1, use LRUCacheShard::Lookup.
// Before destruction, make sure that no handles are in state 1. This means
// that any successful LRUCacheShard::Lookup/LRUCacheShard::Insert have a
// matching LRUCache::Release (to move into state 2) or LRUCacheShard::Erase
// (to move into state 3).
struct LRUHandle {
  void* value;
  void (*deleter)(const Slice&, void* value);
  LRUHandle* next_hash;
  LRUHandle* next;
  LRUHandle* prev;
  size_t charge;  // TODO(opt): Only allow uint32_t?
  size_t key_length;
  // The hash of key(). Used for fast sharding and comparisons.
  uint32_t hash;
  // The number of external refs to this entry. The cache itself is not counted.
  uint32_t refs;

  enum Flags : uint8_t {
    // Whether this entry is referenced by the hash table.
    IN_CACHE = (1 << 0),
    // Whether this entry is high priority entry.
    IS_HIGH_PRI = (1 << 1),
    // Whether this entry is in high-pri pool.
    IN_HIGH_PRI_POOL = (1 << 2),
    // Wwhether this entry has had any lookups (hits).
    HAS_HIT = (1 << 3),
  };

  uint8_t flags;

  // Beginning of the key (MUST BE THE LAST FIELD IN THIS STRUCT!)
  char key_data[1];

  Slice key() const { return Slice(key_data, key_length); }

  // Increase the reference count by 1.
  void Ref() { refs++; }

  // Just reduce the reference count by 1. Return true if it was last reference.
  bool Unref() {
    assert(refs > 0);
    refs--;
    return refs == 0;
  }

  // Return true if there are external refs, false otherwise.
  bool HasRefs() const { return refs > 0; }

  bool InCache() const { return flags & IN_CACHE; }
  bool IsHighPri() const { return flags & IS_HIGH_PRI; }
  bool InHighPriPool() const { return flags & IN_HIGH_PRI_POOL; }
  bool HasHit() const { return flags & HAS_HIT; }

  void SetInCache(bool in_cache) {
    if (in_cache) {
      flags |= IN_CACHE;
    } else {
      flags &= ~IN_CACHE;
    }
  }

  void SetPriority(Cache::Priority priority) {
    if (priority == Cache::Priority::HIGH) {
      flags |= IS_HIGH_PRI;
    } else {
      flags &= ~IS_HIGH_PRI;
    }
  }

  void SetInHighPriPool(bool in_high_pri_pool) {
    if (in_high_pri_pool) {
      flags |= IN_HIGH_PRI_POOL;
    } else {
      flags &= ~IN_HIGH_PRI_POOL;
    }
  }

  void SetHit() { flags |= HAS_HIT; }

  void Free() {
    assert(refs == 0);
    if (deleter) {
      (*deleter)(key(), value);
    }
    delete[] reinterpret_cast<char*>(this);
  }
};


}
