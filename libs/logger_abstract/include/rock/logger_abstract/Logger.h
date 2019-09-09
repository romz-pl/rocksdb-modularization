#pragma once

#include <rock/status/Status.h>
#include <rock/logger_abstract/InfoLogLevel.h>

#include <limits>
#include <cstdarg>

namespace rocksdb {

// An interface for writing log messages.
class Logger {
 public:
  size_t kDoNotSupportGetLogFileSize = (std::numeric_limits<size_t>::max)();

  explicit Logger(const InfoLogLevel log_level = InfoLogLevel::INFO_LEVEL)
      : closed_(false), log_level_(log_level) {}
  virtual ~Logger();

  // Close the log file. Must be called before destructor. If the return
  // status is NotSupported(), it means the implementation does cleanup in
  // the destructor
  virtual Status Close();

  // Write a header to the log file with the specified format
  // It is recommended that you log all header information at the start of the
  // application. But it is not enforced.
  virtual void LogHeader(const char* format, va_list ap) {
    // Default implementation does a simple INFO level log write.
    // Please override as per the logger class requirement.
    Logv(format, ap);
  }

  // Write an entry to the log file with the specified format.
  virtual void Logv(const char* format, va_list ap) = 0;

  // Write an entry to the log file with the specified log level
  // and format.  Any log with level under the internal log level
  // of *this (see @SetInfoLogLevel and @GetInfoLogLevel) will not be
  // printed.
  virtual void Logv(const InfoLogLevel log_level, const char* format,
                    va_list ap);

  virtual size_t GetLogFileSize() const { return kDoNotSupportGetLogFileSize; }
  // Flush to the OS buffers
  virtual void Flush() {}
  virtual InfoLogLevel GetInfoLogLevel() const { return log_level_; }
  virtual void SetInfoLogLevel(const InfoLogLevel log_level) {
    log_level_ = log_level;
  }

  // If you're adding methods here, remember to add them to LoggerWrapper too.

 protected:
  virtual Status CloseImpl();
  bool closed_;

 private:
  // No copying allowed
  Logger(const Logger&);
  void operator=(const Logger&);
  InfoLogLevel log_level_;
};


}
