#pragma once

#include <rock/dbformat/ValueType.h>
#include <rock/sn/SequenceNumber.h>

#include <string>

namespace rocksdb {

// Serialized internal key consists of user key followed by footer.
// This function appends the footer to *result, assuming that *result already
// contains the user key at the end.
extern void AppendInternalKeyFooter(std::string* result, SequenceNumber s,
                                    ValueType t);


}
