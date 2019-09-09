#pragma once

#include <rock/logger_abstract/ROCKSDB_PRINTF_FORMAT_ATTR.h>
#include <memory>

namespace rocksdb {

class Logger;

extern void Info(const std::shared_ptr<Logger>& info_log, const char* format,
                 ...) ROCKSDB_PRINTF_FORMAT_ATTR(2, 3);

extern void Info(Logger* info_log, const char* format, ...)
    ROCKSDB_PRINTF_FORMAT_ATTR(2, 3);

}
