#pragma once

namespace rocksdb {

// Priority for requesting bytes in rate limiter scheduler
enum IOPriority {
    IO_LOW = 0,
    IO_HIGH = 1,
    IO_TOTAL = 2
};

}
