#pragma once

#include <memory>

namespace rocksdb {

class Logger;

extern void LogFlush(const std::shared_ptr<Logger>& info_log);

extern void LogFlush(Logger* info_log);

}
