#pragma once

#ifndef ROCKSDB_LITE

#include <string>

namespace rocksdb {

bool ParseBoolean(const std::string& type, const std::string& value);

}

#endif
