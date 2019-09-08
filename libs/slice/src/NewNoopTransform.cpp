#include <rock/slice/NewNoopTransform.h>

#include <rock/slice/NoopTransform.h>

namespace rocksdb {

const SliceTransform* NewNoopTransform() {
  return new NoopTransform;
}

}
