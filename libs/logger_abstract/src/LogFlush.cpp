#include <rock/logger_abstract/LogFlush.h>

#include <rock/logger_abstract/Logger.h>

namespace rocksdb {

void LogFlush(const std::shared_ptr<Logger>& info_log) {
  LogFlush(info_log.get());
}

void LogFlush(Logger *info_log) {
  if (info_log) {
    info_log->Flush();
  }
}

}
