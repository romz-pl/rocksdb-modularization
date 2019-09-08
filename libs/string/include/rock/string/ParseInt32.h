#pragma once

#ifndef ROCKSDB_LITE

#include <string>

namespace rocksdb {

int32_t ParseInt32(const std::string& value);

}

#endif
