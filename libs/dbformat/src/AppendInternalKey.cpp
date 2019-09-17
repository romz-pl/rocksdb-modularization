#include <rock/dbformat/AppendInternalKey.h>

#include <rock/dbformat/ParsedInternalKey.h>
#include <rock/coding/coding.h>
#include <rock/dbformat/PackSequenceAndType.h>

namespace rocksdb {

void AppendInternalKey(std::string* result, const ParsedInternalKey& key) {
  result->append(key.user_key.data(), key.user_key.size());
  PutFixed64(result, PackSequenceAndType(key.sequence, key.type));
}

}
