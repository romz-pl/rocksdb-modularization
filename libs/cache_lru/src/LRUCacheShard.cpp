#include <rock/cache_lru/LRUCacheShard.h>

#include <rock/mutex/MutexLock.h>

namespace rocksdb {

LRUCacheShard::LRUCacheShard(size_t capacity, bool strict_capacity_limit,
                             double high_pri_pool_ratio,
                             bool use_adaptive_mutex)
    : capacity_(0),
      high_pri_pool_usage_(0),
      strict_capacity_limit_(strict_capacity_limit),
      high_pri_pool_ratio_(high_pri_pool_ratio),
      high_pri_pool_capacity_(0),
      usage_(0),
      lru_usage_(0),
      mutex_(use_adaptive_mutex) {
  // Make empty circular linked list
  lru_.next = &lru_;
  lru_.prev = &lru_;
  lru_low_pri_ = &lru_;
  SetCapacity(capacity);
}

void LRUCacheShard::EraseUnRefEntries() {
  autovector<LRUHandle*> last_reference_list;
  {
    MutexLock l(&mutex_);
    while (lru_.next != &lru_) {
      LRUHandle* old = lru_.next;
      // LRU list contains only elements which can be evicted
      assert(old->InCache() && !old->HasRefs());
      LRU_Remove(old);
      table_.Remove(old->key(), old->hash);
      old->SetInCache(false);
      usage_ -= old->charge;
      last_reference_list.push_back(old);
    }
  }

  for (auto entry : last_reference_list) {
    entry->Free();
  }
}

void LRUCacheShard::ApplyToAllCacheEntries(void (*callback)(void*, size_t),
                                           bool thread_safe) {
  const auto applyCallback = [&]() {
    table_.ApplyToAllCacheEntries(
        [callback](LRUHandle* h) { callback(h->value, h->charge); });
  };

  if (thread_safe) {
    MutexLock l(&mutex_);
    applyCallback();
  } else {
    applyCallback();
  }
}

void LRUCacheShard::TEST_GetLRUList(LRUHandle** lru, LRUHandle** lru_low_pri) {
  MutexLock l(&mutex_);
  *lru = &lru_;
  *lru_low_pri = lru_low_pri_;
}

size_t LRUCacheShard::TEST_GetLRUSize() {
  MutexLock l(&mutex_);
  LRUHandle* lru_handle = lru_.next;
  size_t lru_size = 0;
  while (lru_handle != &lru_) {
    lru_size++;
    lru_handle = lru_handle->next;
  }
  return lru_size;
}

double LRUCacheShard::GetHighPriPoolRatio() {
  MutexLock l(&mutex_);
  return high_pri_pool_ratio_;
}

void LRUCacheShard::LRU_Remove(LRUHandle* e) {
  assert(e->next != nullptr);
  assert(e->prev != nullptr);
  if (lru_low_pri_ == e) {
    lru_low_pri_ = e->prev;
  }
  e->next->prev = e->prev;
  e->prev->next = e->next;
  e->prev = e->next = nullptr;
  lru_usage_ -= e->charge;
  if (e->InHighPriPool()) {
    assert(high_pri_pool_usage_ >= e->charge);
    high_pri_pool_usage_ -= e->charge;
  }
}

void LRUCacheShard::LRU_Insert(LRUHandle* e) {
  assert(e->next == nullptr);
  assert(e->prev == nullptr);
  if (high_pri_pool_ratio_ > 0 && (e->IsHighPri() || e->HasHit())) {
    // Inset "e" to head of LRU list.
    e->next = &lru_;
    e->prev = lru_.prev;
    e->prev->next = e;
    e->next->prev = e;
    e->SetInHighPriPool(true);
    high_pri_pool_usage_ += e->charge;
    MaintainPoolSize();
  } else {
    // Insert "e" to the head of low-pri pool. Note that when
    // high_pri_pool_ratio is 0, head of low-pri pool is also head of LRU list.
    e->next = lru_low_pri_->next;
    e->prev = lru_low_pri_;
    e->prev->next = e;
    e->next->prev = e;
    e->SetInHighPriPool(false);
    lru_low_pri_ = e;
  }
  lru_usage_ += e->charge;
}

void LRUCacheShard::MaintainPoolSize() {
  while (high_pri_pool_usage_ > high_pri_pool_capacity_) {
    // Overflow last entry in high-pri pool to low-pri pool.
    lru_low_pri_ = lru_low_pri_->next;
    assert(lru_low_pri_ != &lru_);
    lru_low_pri_->SetInHighPriPool(false);
    high_pri_pool_usage_ -= lru_low_pri_->charge;
  }
}

void LRUCacheShard::EvictFromLRU(size_t charge,
                                 autovector<LRUHandle*>* deleted) {
  while ((usage_ + charge) > capacity_ && lru_.next != &lru_) {
    LRUHandle* old = lru_.next;
    // LRU list contains only elements which can be evicted
    assert(old->InCache() && !old->HasRefs());
    LRU_Remove(old);
    table_.Remove(old->key(), old->hash);
    old->SetInCache(false);
    usage_ -= old->charge;
    deleted->push_back(old);
  }
}

void LRUCacheShard::SetCapacity(size_t capacity) {
  autovector<LRUHandle*> last_reference_list;
  {
    MutexLock l(&mutex_);
    capacity_ = capacity;
    high_pri_pool_capacity_ = capacity_ * high_pri_pool_ratio_;
    EvictFromLRU(0, &last_reference_list);
  }

  // Free the entries outside of mutex for performance reasons
  for (auto entry : last_reference_list) {
    entry->Free();
  }
}

void LRUCacheShard::SetStrictCapacityLimit(bool strict_capacity_limit) {
  MutexLock l(&mutex_);
  strict_capacity_limit_ = strict_capacity_limit;
}

Cache::Handle* LRUCacheShard::Lookup(const Slice& key, uint32_t hash) {
  MutexLock l(&mutex_);
  LRUHandle* e = table_.Lookup(key, hash);
  if (e != nullptr) {
    assert(e->InCache());
    if (!e->HasRefs()) {
      // The entry is in LRU since it's in hash and has no external references
      LRU_Remove(e);
    }
    e->Ref();
    e->SetHit();
  }
  return reinterpret_cast<Cache::Handle*>(e);
}

bool LRUCacheShard::Ref(Cache::Handle* h) {
  LRUHandle* e = reinterpret_cast<LRUHandle*>(h);
  MutexLock l(&mutex_);
  // To create another reference - entry must be already externally referenced
  assert(e->HasRefs());
  e->Ref();
  return true;
}

void LRUCacheShard::SetHighPriorityPoolRatio(double high_pri_pool_ratio) {
  MutexLock l(&mutex_);
  high_pri_pool_ratio_ = high_pri_pool_ratio;
  high_pri_pool_capacity_ = capacity_ * high_pri_pool_ratio_;
  MaintainPoolSize();
}

bool LRUCacheShard::Release(Cache::Handle* handle, bool force_erase) {
  if (handle == nullptr) {
    return false;
  }
  LRUHandle* e = reinterpret_cast<LRUHandle*>(handle);
  bool last_reference = false;
  {
    MutexLock l(&mutex_);
    last_reference = e->Unref();
    if (last_reference && e->InCache()) {
      // The item is still in cache, and nobody else holds a reference to it
      if (usage_ > capacity_ || force_erase) {
        // The LRU list must be empty since the cache is full
        assert(lru_.next == &lru_ || force_erase);
        // Take this opportunity and remove the item
        table_.Remove(e->key(), e->hash);
        e->SetInCache(false);
      } else {
        // Put the item back on the LRU list, and don't free it
        LRU_Insert(e);
        last_reference = false;
      }
    }
    if (last_reference) {
      usage_ -= e->charge;
    }
  }

  // Free the entry here outside of mutex for performance reasons
  if (last_reference) {
    e->Free();
  }
  return last_reference;
}

Status LRUCacheShard::Insert(const Slice& key, uint32_t hash, void* value,
                             size_t charge,
                             void (*deleter)(const Slice& key, void* value),
                             Cache::Handle** handle, Cache::Priority priority) {
  // Allocate the memory here outside of the mutex
  // If the cache is full, we'll have to release it
  // It shouldn't happen very often though.
  LRUHandle* e = reinterpret_cast<LRUHandle*>(
      new char[sizeof(LRUHandle) - 1 + key.size()]);
  Status s = Status::OK();
  autovector<LRUHandle*> last_reference_list;

  e->value = value;
  e->deleter = deleter;
  e->charge = charge;
  e->key_length = key.size();
  e->flags = 0;
  e->hash = hash;
  e->refs = 0;
  e->next = e->prev = nullptr;
  e->SetInCache(true);
  e->SetPriority(priority);
  memcpy(e->key_data, key.data(), key.size());

  {
    MutexLock l(&mutex_);

    // Free the space following strict LRU policy until enough space
    // is freed or the lru list is empty
    EvictFromLRU(charge, &last_reference_list);

    if ((usage_ + charge) > capacity_ &&
        (strict_capacity_limit_ || handle == nullptr)) {
      if (handle == nullptr) {
        // Don't insert the entry but still return ok, as if the entry inserted
        // into cache and get evicted immediately.
        e->SetInCache(false);
        last_reference_list.push_back(e);
      } else {
        delete[] reinterpret_cast<char*>(e);
        *handle = nullptr;
        s = Status::Incomplete("Insert failed due to LRU cache being full.");
      }
    } else {
      // Insert into the cache. Note that the cache might get larger than its
      // capacity if not enough space was freed up.
      LRUHandle* old = table_.Insert(e);
      usage_ += e->charge;
      if (old != nullptr) {
        assert(old->InCache());
        old->SetInCache(false);
        if (!old->HasRefs()) {
          // old is on LRU because it's in cache and its reference count is 0
          LRU_Remove(old);
          usage_ -= old->charge;
          last_reference_list.push_back(old);
        }
      }
      if (handle == nullptr) {
        LRU_Insert(e);
      } else {
        e->Ref();
        *handle = reinterpret_cast<Cache::Handle*>(e);
      }
    }
  }

  // Free the entries here outside of mutex for performance reasons
  for (auto entry : last_reference_list) {
    entry->Free();
  }

  return s;
}

void LRUCacheShard::Erase(const Slice& key, uint32_t hash) {
  LRUHandle* e;
  bool last_reference = false;
  {
    MutexLock l(&mutex_);
    e = table_.Remove(key, hash);
    if (e != nullptr) {
      assert(e->InCache());
      e->SetInCache(false);
      if (!e->HasRefs()) {
        // The entry is in LRU since it's in hash and has no external references
        LRU_Remove(e);
        usage_ -= e->charge;
        last_reference = true;
      }
    }
  }

  // Free the entry here outside of mutex for performance reasons
  // last_reference will only be true if e != nullptr
  if (last_reference) {
    e->Free();
  }
}

size_t LRUCacheShard::GetUsage() const {
  MutexLock l(&mutex_);
  return usage_;
}

size_t LRUCacheShard::GetPinnedUsage() const {
  MutexLock l(&mutex_);
  assert(usage_ >= lru_usage_);
  return usage_ - lru_usage_;
}

std::string LRUCacheShard::GetPrintableOptions() const {
  const int kBufferSize = 200;
  char buffer[kBufferSize];
  {
    MutexLock l(&mutex_);
    snprintf(buffer, kBufferSize, "    high_pri_pool_ratio: %.3lf\n",
             high_pri_pool_ratio_);
  }
  return std::string(buffer);
}






}  // namespace rocksdb
