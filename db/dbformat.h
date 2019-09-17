//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#pragma once
#include <stdio.h>
#include <memory>
#include <string>
#include <utility>
#include <rock/memtable/LookupKey.h>
#include "db/merge_context.h"
#include "logging/logging.h"
#include "monitoring/perf_context_imp.h"
#include <rock/slice/Comparator.h>
#include "rocksdb/db.h"
#include "rocksdb/filter_policy.h"
#include <rock/slice/Slice.h>
#include <rock/slice/SliceTransform.h>
#include "rocksdb/table.h"
#include "rocksdb/types.h"
#include <rock/coding/coding.h>
#include "util/user_comparator_wrapper.h"

#include <rock/numeric_limits/numeric_limits.h>
#include <rock/slice/ExtractUserKey.h>
#include <rock/dbformat/ValueType.h>
#include <rock/dbformat/kValueTypeForSeek.h>
#include <rock/dbformat/kValueTypeForSeekForPrev.h>
#include <rock/dbformat/IsExtendedValueType.h>
#include <rock/dbformat/kMaxSequenceNumber.h>
#include <rock/dbformat/IsValueType.h>
#include <rock/dbformat/kDisableGlobalSequenceNumber.h>
#include <rock/dbformat/ParsedInternalKey.h>
#include <rock/dbformat/InternalKeyEncodingLength.h>
#include <rock/dbformat/PackSequenceAndType.h>
#include <rock/dbformat/UnPackSequenceAndType.h>
#include <rock/dbformat/GetEntryType.h>
#include <rock/dbformat/AppendInternalKey.h>
#include <rock/dbformat/AppendInternalKeyFooter.h>
#include <rock/dbformat/ParseInternalKey.h>
#include <rock/dbformat/ExtractUserKeyAndStripTimestamp.h>
#include <rock/dbformat/StripTimestampFromUserKey.h>
#include <rock/dbformat/ExtractInternalKeyFooter.h>
#include <rock/dbformat/ExtractValueType.h>


namespace rocksdb {


class InternalKey;





// A comparator for internal keys that uses a specified comparator for
// the user key portion and breaks ties by decreasing sequence number.
class InternalKeyComparator
#ifdef NDEBUG
    final
#endif
    : public Comparator {
 private:
  UserComparatorWrapper user_comparator_;
  std::string name_;

 public:
  explicit InternalKeyComparator(const Comparator* c)
      : user_comparator_(c),
        name_("rocksdb.InternalKeyComparator:" +
              std::string(user_comparator_.Name())) {}
  virtual ~InternalKeyComparator() {}

  virtual const char* Name() const override;
  virtual int Compare(const Slice& a, const Slice& b) const override;
  // Same as Compare except that it excludes the value type from comparison
  virtual int CompareKeySeq(const Slice& a, const Slice& b) const;
  virtual void FindShortestSeparator(std::string* start,
                                     const Slice& limit) const override;
  virtual void FindShortSuccessor(std::string* key) const override;

  const Comparator* user_comparator() const {
    return user_comparator_.user_comparator();
  }

  int Compare(const InternalKey& a, const InternalKey& b) const;
  int Compare(const ParsedInternalKey& a, const ParsedInternalKey& b) const;
  virtual const Comparator* GetRootComparator() const override {
    return user_comparator_.GetRootComparator();
  }
};

// The class represent the internal key in encoded form.
class InternalKey {
 private:
  std::string rep_;

 public:
  InternalKey() {}  // Leave rep_ as empty to indicate it is invalid
  InternalKey(const Slice& _user_key, SequenceNumber s, ValueType t) {
    AppendInternalKey(&rep_, ParsedInternalKey(_user_key, s, t));
  }

  // sets the internal key to be bigger or equal to all internal keys with this
  // user key
  void SetMaxPossibleForUserKey(const Slice& _user_key) {
    AppendInternalKey(
        &rep_, ParsedInternalKey(_user_key, 0, static_cast<ValueType>(0)));
  }

  // sets the internal key to be smaller or equal to all internal keys with this
  // user key
  void SetMinPossibleForUserKey(const Slice& _user_key) {
    AppendInternalKey(&rep_, ParsedInternalKey(_user_key, kMaxSequenceNumber,
                                               kValueTypeForSeek));
  }

  bool Valid() const {
    ParsedInternalKey parsed;
    return ParseInternalKey(Slice(rep_), &parsed);
  }

  void DecodeFrom(const Slice& s) { rep_.assign(s.data(), s.size()); }
  Slice Encode() const {
    assert(!rep_.empty());
    return rep_;
  }

  Slice user_key() const { return ExtractUserKey(rep_); }
  size_t size() { return rep_.size(); }

  void Set(const Slice& _user_key, SequenceNumber s, ValueType t) {
    SetFrom(ParsedInternalKey(_user_key, s, t));
  }

  void SetFrom(const ParsedInternalKey& p) {
    rep_.clear();
    AppendInternalKey(&rep_, p);
  }

  void Clear() { rep_.clear(); }

  // The underlying representation.
  // Intended only to be used together with ConvertFromUserKey().
  std::string* rep() { return &rep_; }

  // Assuming that *rep() contains a user key, this method makes internal key
  // out of it in-place. This saves a memcpy compared to Set()/SetFrom().
  void ConvertFromUserKey(SequenceNumber s, ValueType t) {
    AppendInternalKeyFooter(&rep_, s, t);
  }

  std::string DebugString(bool hex = false) const;
};

inline int InternalKeyComparator::Compare(const InternalKey& a,
                                          const InternalKey& b) const {
  return Compare(a.Encode(), b.Encode());
}



// Update the sequence number in the internal key.
// Guarantees not to invalidate ikey.data().
inline void UpdateInternalKey(std::string* ikey, uint64_t seq, ValueType t) {
  size_t ikey_sz = ikey->size();
  assert(ikey_sz >= 8);
  uint64_t newval = (seq << 8) | t;

  // Note: Since C++11, strings are guaranteed to be stored contiguously and
  // string::operator[]() is guaranteed not to change ikey.data().
  EncodeFixed64(&(*ikey)[ikey_sz - 8], newval);
}

// Get the sequence number from the internal key
inline uint64_t GetInternalKeySeqno(const Slice& internal_key) {
  const size_t n = internal_key.size();
  assert(n >= 8);
  uint64_t num = DecodeFixed64(internal_key.data() + n - 8);
  return num >> 8;
}

// The class to store keys in an efficient way. It allows:
// 1. Users can either copy the key into it, or have it point to an unowned
//    address.
// 2. For copied key, a short inline buffer is kept to reduce memory
//    allocation for smaller keys.
// 3. It tracks user key or internal key, and allow conversion between them.
class IterKey {
 public:
  IterKey()
      : buf_(space_),
        key_(buf_),
        key_size_(0),
        buf_size_(sizeof(space_)),
        is_user_key_(true) {}

  ~IterKey() { ResetBuffer(); }

  // The bool will be picked up by the next calls to SetKey
  void SetIsUserKey(bool is_user_key) { is_user_key_ = is_user_key; }

  // Returns the key in whichever format that was provided to KeyIter
  Slice GetKey() const { return Slice(key_, key_size_); }

  Slice GetInternalKey() const {
    assert(!IsUserKey());
    return Slice(key_, key_size_);
  }

  Slice GetUserKey() const {
    if (IsUserKey()) {
      return Slice(key_, key_size_);
    } else {
      assert(key_size_ >= 8);
      return Slice(key_, key_size_ - 8);
    }
  }

  size_t Size() const { return key_size_; }

  void Clear() { key_size_ = 0; }

  // Append "non_shared_data" to its back, from "shared_len"
  // This function is used in Block::Iter::ParseNextKey
  // shared_len: bytes in [0, shard_len-1] would be remained
  // non_shared_data: data to be append, its length must be >= non_shared_len
  void TrimAppend(const size_t shared_len, const char* non_shared_data,
                  const size_t non_shared_len) {
    assert(shared_len <= key_size_);
    size_t total_size = shared_len + non_shared_len;

    if (IsKeyPinned() /* key is not in buf_ */) {
      // Copy the key from external memory to buf_ (copy shared_len bytes)
      EnlargeBufferIfNeeded(total_size);
      memcpy(buf_, key_, shared_len);
    } else if (total_size > buf_size_) {
      // Need to allocate space, delete previous space
      char* p = new char[total_size];
      memcpy(p, key_, shared_len);

      if (buf_ != space_) {
        delete[] buf_;
      }

      buf_ = p;
      buf_size_ = total_size;
    }

    memcpy(buf_ + shared_len, non_shared_data, non_shared_len);
    key_ = buf_;
    key_size_ = total_size;
  }

  Slice SetKey(const Slice& key, bool copy = true) {
    // is_user_key_ expected to be set already via SetIsUserKey
    return SetKeyImpl(key, copy);
  }

  Slice SetUserKey(const Slice& key, bool copy = true) {
    is_user_key_ = true;
    return SetKeyImpl(key, copy);
  }

  Slice SetInternalKey(const Slice& key, bool copy = true) {
    is_user_key_ = false;
    return SetKeyImpl(key, copy);
  }

  // Copies the content of key, updates the reference to the user key in ikey
  // and returns a Slice referencing the new copy.
  Slice SetInternalKey(const Slice& key, ParsedInternalKey* ikey) {
    size_t key_n = key.size();
    assert(key_n >= 8);
    SetInternalKey(key);
    ikey->user_key = Slice(key_, key_n - 8);
    return Slice(key_, key_n);
  }

  // Copy the key into IterKey own buf_
  void OwnKey() {
    assert(IsKeyPinned() == true);

    Reserve(key_size_);
    memcpy(buf_, key_, key_size_);
    key_ = buf_;
  }

  // Update the sequence number in the internal key.  Guarantees not to
  // invalidate slices to the key (and the user key).
  void UpdateInternalKey(uint64_t seq, ValueType t) {
    assert(!IsKeyPinned());
    assert(key_size_ >= 8);
    uint64_t newval = (seq << 8) | t;
    EncodeFixed64(&buf_[key_size_ - 8], newval);
  }

  bool IsKeyPinned() const { return (key_ != buf_); }

  void SetInternalKey(const Slice& key_prefix, const Slice& user_key,
                      SequenceNumber s,
                      ValueType value_type = kValueTypeForSeek) {
    size_t psize = key_prefix.size();
    size_t usize = user_key.size();
    EnlargeBufferIfNeeded(psize + usize + sizeof(uint64_t));
    if (psize > 0) {
      memcpy(buf_, key_prefix.data(), psize);
    }
    memcpy(buf_ + psize, user_key.data(), usize);
    EncodeFixed64(buf_ + usize + psize, PackSequenceAndType(s, value_type));

    key_ = buf_;
    key_size_ = psize + usize + sizeof(uint64_t);
    is_user_key_ = false;
  }

  void SetInternalKey(const Slice& user_key, SequenceNumber s,
                      ValueType value_type = kValueTypeForSeek) {
    SetInternalKey(Slice(), user_key, s, value_type);
  }

  void Reserve(size_t size) {
    EnlargeBufferIfNeeded(size);
    key_size_ = size;
  }

  void SetInternalKey(const ParsedInternalKey& parsed_key) {
    SetInternalKey(Slice(), parsed_key);
  }

  void SetInternalKey(const Slice& key_prefix,
                      const ParsedInternalKey& parsed_key_suffix) {
    SetInternalKey(key_prefix, parsed_key_suffix.user_key,
                   parsed_key_suffix.sequence, parsed_key_suffix.type);
  }

  void EncodeLengthPrefixedKey(const Slice& key) {
    auto size = key.size();
    EnlargeBufferIfNeeded(size + static_cast<size_t>(VarintLength(size)));
    char* ptr = EncodeVarint32(buf_, static_cast<uint32_t>(size));
    memcpy(ptr, key.data(), size);
    key_ = buf_;
    is_user_key_ = true;
  }

  bool IsUserKey() const { return is_user_key_; }

 private:
  char* buf_;
  const char* key_;
  size_t key_size_;
  size_t buf_size_;
  char space_[32];  // Avoid allocation for short keys
  bool is_user_key_;

  Slice SetKeyImpl(const Slice& key, bool copy) {
    size_t size = key.size();
    if (copy) {
      // Copy key to buf_
      EnlargeBufferIfNeeded(size);
      memcpy(buf_, key.data(), size);
      key_ = buf_;
    } else {
      // Update key_ to point to external memory
      key_ = key.data();
    }
    key_size_ = size;
    return Slice(key_, key_size_);
  }

  void ResetBuffer() {
    if (buf_ != space_) {
      delete[] buf_;
      buf_ = space_;
    }
    buf_size_ = sizeof(space_);
    key_size_ = 0;
  }

  // Enlarge the buffer size if needed based on key_size.
  // By default, static allocated buffer is used. Once there is a key
  // larger than the static allocated buffer, another buffer is dynamically
  // allocated, until a larger key buffer is requested. In that case, we
  // reallocate buffer and delete the old one.
  void EnlargeBufferIfNeeded(size_t key_size) {
    // If size is smaller than buffer size, continue using current buffer,
    // or the static allocated one, as default
    if (key_size > buf_size_) {
      EnlargeBuffer(key_size);
    }
  }

  void EnlargeBuffer(size_t key_size);

  // No copying allowed
  IterKey(const IterKey&) = delete;
  void operator=(const IterKey&) = delete;
};

// Convert from a SliceTranform of user keys, to a SliceTransform of
// user keys.
class InternalKeySliceTransform : public SliceTransform {
 public:
  explicit InternalKeySliceTransform(const SliceTransform* transform)
      : transform_(transform) {}

  virtual const char* Name() const override { return transform_->Name(); }

  virtual Slice Transform(const Slice& src) const override {
    auto user_key = ExtractUserKey(src);
    return transform_->Transform(user_key);
  }

  virtual bool InDomain(const Slice& src) const override {
    auto user_key = ExtractUserKey(src);
    return transform_->InDomain(user_key);
  }

  virtual bool InRange(const Slice& dst) const override {
    auto user_key = ExtractUserKey(dst);
    return transform_->InRange(user_key);
  }

  const SliceTransform* user_prefix_extractor() const { return transform_; }

 private:
  // Like comparator, InternalKeySliceTransform will not take care of the
  // deletion of transform_
  const SliceTransform* const transform_;
};

// Read the key of a record from a write batch.
// if this record represent the default column family then cf_record
// must be passed as false, otherwise it must be passed as true.
extern bool ReadKeyFromWriteBatchEntry(Slice* input, Slice* key,
                                       bool cf_record);

// Read record from a write batch piece from input.
// tag, column_family, key, value and blob are return values. Callers own the
// Slice they point to.
// Tag is defined as ValueType.
// input will be advanced to after the record.
extern Status ReadRecordFromWriteBatch(Slice* input, char* tag,
                                       uint32_t* column_family, Slice* key,
                                       Slice* value, Slice* blob, Slice* xid);

// When user call DeleteRange() to delete a range of keys,
// we will store a serialized RangeTombstone in MemTable and SST.
// the struct here is a easy-understood form
// start/end_key_ is the start/end user key of the range to be deleted
struct RangeTombstone {
  Slice start_key_;
  Slice end_key_;
  SequenceNumber seq_;
  RangeTombstone() = default;
  RangeTombstone(Slice sk, Slice ek, SequenceNumber sn)
      : start_key_(sk), end_key_(ek), seq_(sn) {}

  RangeTombstone(ParsedInternalKey parsed_key, Slice value) {
    start_key_ = parsed_key.user_key;
    seq_ = parsed_key.sequence;
    end_key_ = value;
  }

  // be careful to use Serialize(), allocates new memory
  std::pair<InternalKey, Slice> Serialize() const {
    auto key = InternalKey(start_key_, seq_, kTypeRangeDeletion);
    Slice value = end_key_;
    return std::make_pair(std::move(key), std::move(value));
  }

  // be careful to use SerializeKey(), allocates new memory
  InternalKey SerializeKey() const {
    return InternalKey(start_key_, seq_, kTypeRangeDeletion);
  }

  // The tombstone end-key is exclusive, so we generate an internal-key here
  // which has a similar property. Using kMaxSequenceNumber guarantees that
  // the returned internal-key will compare less than any other internal-key
  // with the same user-key. This in turn guarantees that the serialized
  // end-key for a tombstone such as [a-b] will compare less than the key "b".
  //
  // be careful to use SerializeEndKey(), allocates new memory
  InternalKey SerializeEndKey() const {
    return InternalKey(end_key_, kMaxSequenceNumber, kTypeRangeDeletion);
  }
};

inline int InternalKeyComparator::Compare(const Slice& akey,
                                          const Slice& bkey) const {
  // Order by:
  //    increasing user key (according to user-supplied comparator)
  //    decreasing sequence number
  //    decreasing type (though sequence# should be enough to disambiguate)
  int r = user_comparator_.Compare(ExtractUserKey(akey), ExtractUserKey(bkey));
  if (r == 0) {
    const uint64_t anum = DecodeFixed64(akey.data() + akey.size() - 8);
    const uint64_t bnum = DecodeFixed64(bkey.data() + bkey.size() - 8);
    if (anum > bnum) {
      r = -1;
    } else if (anum < bnum) {
      r = +1;
    }
  }
  return r;
}

inline int InternalKeyComparator::CompareKeySeq(const Slice& akey,
                                                const Slice& bkey) const {
  // Order by:
  //    increasing user key (according to user-supplied comparator)
  //    decreasing sequence number
  int r = user_comparator_.Compare(ExtractUserKey(akey), ExtractUserKey(bkey));
  if (r == 0) {
    // Shift the number to exclude the last byte which contains the value type
    const uint64_t anum = DecodeFixed64(akey.data() + akey.size() - 8) >> 8;
    const uint64_t bnum = DecodeFixed64(bkey.data() + bkey.size() - 8) >> 8;
    if (anum > bnum) {
      r = -1;
    } else if (anum < bnum) {
      r = +1;
    }
  }
  return r;
}

// Wrap InternalKeyComparator as a comparator class for ParsedInternalKey.
struct ParsedInternalKeyComparator {
  explicit ParsedInternalKeyComparator(const InternalKeyComparator* c)
      : cmp(c) {}

  bool operator()(const ParsedInternalKey& a,
                  const ParsedInternalKey& b) const {
    return cmp->Compare(a, b) < 0;
  }

  const InternalKeyComparator* cmp;
};

}  // namespace rocksdb
