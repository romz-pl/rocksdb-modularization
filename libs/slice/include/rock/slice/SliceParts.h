#pragma once

namespace rocksdb {

class Slice;

// A set of Slices that are virtually concatenated together.  'parts' points
// to an array of Slices.  The number of elements in the array is 'num_parts'.
struct SliceParts {
  SliceParts(const Slice* _parts, int _num_parts)
      : parts(_parts), num_parts(_num_parts) {}
  SliceParts() : parts(nullptr), num_parts(0) {}

  const Slice* parts;
  int num_parts;
};

}
