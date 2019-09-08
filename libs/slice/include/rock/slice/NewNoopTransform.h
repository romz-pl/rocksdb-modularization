#pragma once

namespace rocksdb {

class SliceTransform;

extern const SliceTransform* NewNoopTransform();

}
