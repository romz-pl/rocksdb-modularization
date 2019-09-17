#pragma once

#include <rock/dbformat/ValueType.h>
#include <rock/dbformat/EntryType.h>

namespace rocksdb {

EntryType GetEntryType(ValueType value_type);

}
