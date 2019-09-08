#pragma once

#ifndef ROCKSDB_LITE

#include <string>

namespace rocksdb {

uint32_t ParseUint32(const std::string& value);

}

#endif
