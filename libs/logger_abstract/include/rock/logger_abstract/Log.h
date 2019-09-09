#pragma once

#include <rock/logger_abstract/InfoLogLevel.h>
#include <rock/logger_abstract/ROCKSDB_PRINTF_FORMAT_ATTR.h>
#include <memory>

namespace rocksdb {

class Logger;

extern void Log(const InfoLogLevel log_level,
                const std::shared_ptr<Logger>& info_log, const char* format,
                ...) ROCKSDB_PRINTF_FORMAT_ATTR(3, 4);


// Log the specified data to *info_log if info_log is non-nullptr.
// The default info log level is InfoLogLevel::INFO_LEVEL.
extern void Log(const std::shared_ptr<Logger>& info_log, const char* format,
                ...) ROCKSDB_PRINTF_FORMAT_ATTR(2, 3);


extern void Log(const InfoLogLevel log_level, Logger* info_log,
                const char* format, ...) ROCKSDB_PRINTF_FORMAT_ATTR(3, 4);

// The default info log level is InfoLogLevel::INFO_LEVEL.
extern void Log(Logger* info_log, const char* format, ...)
    ROCKSDB_PRINTF_FORMAT_ATTR(2, 3);


}
