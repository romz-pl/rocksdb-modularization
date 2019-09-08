#pragma once

namespace rocksdb {

// These values match Linux definition
// https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/fcntl.h#n56
enum WriteLifeTimeHint {
  WLTH_NOT_SET = 0,  // No hint information set
  WLTH_NONE,         // No hints about write life time
  WLTH_SHORT,        // Data written has a short life time
  WLTH_MEDIUM,       // Data written has a medium life time
  WLTH_LONG,         // Data written has a long life time
  WLTH_EXTREME,      // Data written has an extremely long life time
};

}
