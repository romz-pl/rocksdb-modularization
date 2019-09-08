#include <rock/string/TimeToHumanString.h>

#include <cassert>
#include <ctime>

namespace rocksdb {

std::string TimeToHumanString(int unixtime) {
  char time_buffer[80];
  time_t rawtime = unixtime;
  struct tm tInfo;
  struct tm* timeinfo = localtime_r(&rawtime, &tInfo);
  assert(timeinfo == &tInfo);
  strftime(time_buffer, 80, "%c", timeinfo);
  return std::string(time_buffer);
}

}
