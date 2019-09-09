#pragma once

#include <rock/logger_abstract/Logger.h>

namespace rocksdb {


class LoggerWrapper : public Logger {
 public:
  explicit LoggerWrapper(Logger* target) : target_(target) {}

  Status Close() override { return target_->Close(); }
  void LogHeader(const char* format, va_list ap) override {
    return target_->LogHeader(format, ap);
  }
  void Logv(const char* format, va_list ap) override {
    return target_->Logv(format, ap);
  }
  void Logv(const InfoLogLevel log_level, const char* format,
            va_list ap) override {
    return target_->Logv(log_level, format, ap);
  }
  size_t GetLogFileSize() const override { return target_->GetLogFileSize(); }
  void Flush() override { return target_->Flush(); }
  InfoLogLevel GetInfoLogLevel() const override {
    return target_->GetInfoLogLevel();
  }
  void SetInfoLogLevel(const InfoLogLevel log_level) override {
    return target_->SetInfoLogLevel(log_level);
  }

 private:
  Logger* target_;
};




}
