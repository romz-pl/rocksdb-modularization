#include <rock/dbformat/AppendInternalKeyFooter.h>

#include <rock/coding/coding.h>
#include <rock/dbformat/PackSequenceAndType.h>

namespace rocksdb {

void AppendInternalKeyFooter(std::string* result, SequenceNumber s,
                             ValueType t) {
  PutFixed64(result, PackSequenceAndType(s, t));
}

}
