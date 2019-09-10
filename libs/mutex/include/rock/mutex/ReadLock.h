#pragma once

#include <rock/port/port.h>

namespace rocksdb {

//
// Acquire a ReadLock on the specified RWMutex.
// The Lock will be automatically released then the
// object goes out of scope.
//
class ReadLock {
 public:
  explicit ReadLock(port::RWMutex *mu) : mu_(mu) {
    this->mu_->ReadLock();
  }
  ~ReadLock() { this->mu_->ReadUnlock(); }

 private:
  port::RWMutex *const mu_;
  // No copying allowed
  ReadLock(const ReadLock&);
  void operator=(const ReadLock&);
};

}
