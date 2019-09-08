#include <rock/status/Status.h>


namespace rocksdb {


Status::Status(const Status& s)
    : code_(s.code_), subcode_(s.subcode_), sev_(s.sev_) {
  state_ = (s.state_ == nullptr) ? nullptr : CopyState(s.state_);
}
Status::Status(const Status& s, Severity sev)
    : code_(s.code_), subcode_(s.subcode_), sev_(sev) {
  state_ = (s.state_ == nullptr) ? nullptr : CopyState(s.state_);
}
Status& Status::operator=(const Status& s) {
  // The following condition catches both aliasing (when this == &s),
  // and the common case where both s and *this are ok.
  if (this != &s) {
    code_ = s.code_;
    subcode_ = s.subcode_;
    sev_ = s.sev_;
    delete[] state_;
    state_ = (s.state_ == nullptr) ? nullptr : CopyState(s.state_);
  }
  return *this;
}

Status::Status(Status&& s)
#if !(defined _MSC_VER) || ((defined _MSC_VER) && (_MSC_VER >= 1900))
    noexcept
#endif
    : Status() {
  *this = std::move(s);
}

Status& Status::operator=(Status&& s)
#if !(defined _MSC_VER) || ((defined _MSC_VER) && (_MSC_VER >= 1900))
    noexcept
#endif
{
  if (this != &s) {
    code_ = std::move(s.code_);
    s.code_ = kOk;
    subcode_ = std::move(s.subcode_);
    s.subcode_ = kNone;
    sev_ = std::move(s.sev_);
    s.sev_ = kNoError;
    delete[] state_;
    state_ = nullptr;
    std::swap(state_, s.state_);
  }
  return *this;
}

bool Status::operator==(const Status& rhs) const {
  return (code_ == rhs.code_);
}

bool Status::operator!=(const Status& rhs) const {
  return !(*this == rhs);
}


const char* Status::CopyState(const char* state) {
#ifdef OS_WIN
  const size_t cch = std::strlen(state) + 1;  // +1 for the null terminator
  char* result = new char[cch];
  errno_t ret;
  ret = strncpy_s(result, cch, state, cch - 1);
  result[cch - 1] = '\0';
  assert(ret == 0);
  return result;
#else
  const size_t cch = std::strlen(state) + 1;  // +1 for the null terminator
  return std::strncpy(new char[cch], state, cch);
#endif
}

static const char* msgs[static_cast<int>(Status::kMaxSubCode)] = {
    "",                                                   // kNone
    "Timeout Acquiring Mutex",                            // kMutexTimeout
    "Timeout waiting to lock key",                        // kLockTimeout
    "Failed to acquire lock due to max_num_locks limit",  // kLockLimit
    "No space left on device",                            // kNoSpace
    "Deadlock",                                           // kDeadlock
    "Stale file handle",                                  // kStaleFile
    "Memory limit reached",                               // kMemoryLimit
    "Space limit reached",                                // kSpaceLimit
    "No such file or directory",                          // kPathNotFound
};

Status::Status(Code _code, SubCode _subcode, const Slice& msg,
               const Slice& msg2)
    : code_(_code), subcode_(_subcode), sev_(kNoError) {
  assert(code_ != kOk);
  assert(subcode_ != kMaxSubCode);
  const size_t len1 = msg.size();
  const size_t len2 = msg2.size();
  const size_t size = len1 + (len2 ? (2 + len2) : 0);
  char* const result = new char[size + 1];  // +1 for null terminator
  memcpy(result, msg.data(), len1);
  if (len2) {
    result[len1] = ':';
    result[len1 + 1] = ' ';
    memcpy(result + len1 + 2, msg2.data(), len2);
  }
  result[size] = '\0';  // null terminator for C style string
  state_ = result;
}

std::string Status::ToString() const {
  char tmp[30];
  const char* type;
  switch (code_) {
    case kOk:
      return "OK";
    case kNotFound:
      type = "NotFound: ";
      break;
    case kCorruption:
      type = "Corruption: ";
      break;
    case kNotSupported:
      type = "Not implemented: ";
      break;
    case kInvalidArgument:
      type = "Invalid argument: ";
      break;
    case kIOError:
      type = "IO error: ";
      break;
    case kMergeInProgress:
      type = "Merge in progress: ";
      break;
    case kIncomplete:
      type = "Result incomplete: ";
      break;
    case kShutdownInProgress:
      type = "Shutdown in progress: ";
      break;
    case kTimedOut:
      type = "Operation timed out: ";
      break;
    case kAborted:
      type = "Operation aborted: ";
      break;
    case kBusy:
      type = "Resource busy: ";
      break;
    case kExpired:
      type = "Operation expired: ";
      break;
    case kTryAgain:
      type = "Operation failed. Try again.: ";
      break;
    case kColumnFamilyDropped:
      type = "Column family dropped: ";
      break;
    default:
      snprintf(tmp, sizeof(tmp), "Unknown code(%d): ",
               static_cast<int>(code()));
      type = tmp;
      break;
  }
  std::string result(type);
  if (subcode_ != kNone) {
    uint32_t index = static_cast<int32_t>(subcode_);
    assert(sizeof(msgs) > index);
    result.append(msgs[index]);
  }

  if (state_ != nullptr) {
    result.append(state_);
  }
  return result;
}

}  // namespace rocksdb

