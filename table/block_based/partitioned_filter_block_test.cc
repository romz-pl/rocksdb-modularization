//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include <map>

#include "rocksdb/filter_policy.h"

#include "table/block_based/block_based_table_reader.h"
#include "table/block_based/partitioned_filter_block.h"
#include "table/full_filter_bits_builder.h"

#include "index_builder.h"
#include "logging/logging.h"
#include "test_util/testharness.h"
#include "test_util/testutil.h"
#include "util/coding.h"
#include <rock/hash/hash.h>

#include <rock/slice/NewFixedPrefixTransform.h>

namespace rocksdb {

std::map<uint64_t, Slice> slices;

class MockedBlockBasedTable : public BlockBasedTable {
 public:
  MockedBlockBasedTable(Rep* rep, PartitionedIndexBuilder* pib)
      : BlockBasedTable(rep, /*block_cache_tracer=*/nullptr) {
    // Initialize what Open normally does as much as necessary for the test
    rep->index_key_includes_seq = pib->seperator_is_key_plus_seq();
    rep->index_value_is_full = !pib->get_use_value_delta_encoding();
  }
};

class MyPartitionedFilterBlockReader : public PartitionedFilterBlockReader {
 public:
  MyPartitionedFilterBlockReader(BlockBasedTable* t,
                                 CachableEntry<Block>&& filter_block)
      : PartitionedFilterBlockReader(t, std::move(filter_block)) {
    for (const auto& pair : slices) {
      const uint64_t offset = pair.first;
      const Slice& slice = pair.second;

      CachableEntry<BlockContents> block(
          new BlockContents(slice), nullptr /* cache */,
          nullptr /* cache_handle */, true /* own_value */);
      filter_map_[offset] = std::move(block);
    }
  }
};

class PartitionedFilterBlockTest
    : public testing::Test,
      virtual public ::testing::WithParamInterface<uint32_t> {
 public:
  Options options_;
  ImmutableCFOptions ioptions_;
  EnvOptions env_options_;
  BlockBasedTableOptions table_options_;
  InternalKeyComparator icomp_;
  std::unique_ptr<BlockBasedTable> table_;
  std::shared_ptr<Cache> cache_;

  PartitionedFilterBlockTest()
      : ioptions_(options_),
        env_options_(options_),
        icomp_(options_.comparator) {
    table_options_.filter_policy.reset(NewBloomFilterPolicy(10, false));
    table_options_.format_version = GetParam();
    table_options_.index_block_restart_interval = 3;
  }

  ~PartitionedFilterBlockTest() override {}

  const std::string keys[4] = {"afoo", "bar", "box", "hello"};
  const std::string missing_keys[2] = {"missing", "other"};

  uint64_t MaxIndexSize() {
    int num_keys = sizeof(keys) / sizeof(*keys);
    uint64_t max_key_size = 0;
    for (int i = 1; i < num_keys; i++) {
      max_key_size = std::max(max_key_size, static_cast<uint64_t>(keys[i].size()));
    }
    uint64_t max_index_size = num_keys * (max_key_size + 8 /*handle*/);
    return max_index_size;
  }

  uint64_t MaxFilterSize() {
    uint32_t dont_care1, dont_care2;
    int num_keys = sizeof(keys) / sizeof(*keys);
    auto filter_bits_reader = dynamic_cast<rocksdb::FullFilterBitsBuilder*>(
        table_options_.filter_policy->GetFilterBitsBuilder());
    assert(filter_bits_reader);
    auto partition_size =
        filter_bits_reader->CalculateSpace(num_keys, &dont_care1, &dont_care2);
    delete filter_bits_reader;
    return partition_size +
               partition_size * table_options_.block_size_deviation / 100;
  }

  uint64_t last_offset = 10;
  BlockHandle Write(const Slice& slice) {
    BlockHandle bh(last_offset + 1, slice.size());
    slices[bh.offset()] = slice;
    last_offset += bh.size();
    return bh;
  }

  PartitionedIndexBuilder* NewIndexBuilder() {
    const bool kValueDeltaEncoded = true;
    return PartitionedIndexBuilder::CreateIndexBuilder(
        &icomp_, !kValueDeltaEncoded, table_options_);
  }

  PartitionedFilterBlockBuilder* NewBuilder(
      PartitionedIndexBuilder* const p_index_builder,
      const SliceTransform* prefix_extractor = nullptr) {
    assert(table_options_.block_size_deviation <= 100);
    auto partition_size = static_cast<uint32_t>(
             ((table_options_.metadata_block_size *
               (100 - table_options_.block_size_deviation)) +
              99) /
             100);
    partition_size = std::max(partition_size, static_cast<uint32_t>(1));
    const bool kValueDeltaEncoded = true;
    return new PartitionedFilterBlockBuilder(
        prefix_extractor, table_options_.whole_key_filtering,
        table_options_.filter_policy->GetFilterBitsBuilder(),
        table_options_.index_block_restart_interval, !kValueDeltaEncoded,
        p_index_builder, partition_size);
  }

  PartitionedFilterBlockReader* NewReader(
      PartitionedFilterBlockBuilder* builder, PartitionedIndexBuilder* pib) {
    BlockHandle bh;
    Status status;
    Slice slice;
    do {
      slice = builder->Finish(bh, &status);
      bh = Write(slice);
    } while (status.IsIncomplete());

    constexpr bool skip_filters = false;
    constexpr int level = 0;
    constexpr bool immortal_table = false;
    table_.reset(new MockedBlockBasedTable(
        new BlockBasedTable::Rep(ioptions_, env_options_, table_options_,
                                 icomp_, skip_filters, level, immortal_table),
        pib));
    BlockContents contents(slice);
    CachableEntry<Block> block(
        new Block(std::move(contents), kDisableGlobalSequenceNumber,
                  0 /* read_amp_bytes_per_bit */, nullptr),
        nullptr /* cache */, nullptr /* cache_handle */, true /* own_value */);
    auto reader =
        new MyPartitionedFilterBlockReader(table_.get(), std::move(block));
    return reader;
  }

  void VerifyReader(PartitionedFilterBlockBuilder* builder,
                    PartitionedIndexBuilder* pib, bool empty = false,
                    const SliceTransform* prefix_extractor = nullptr) {
    std::unique_ptr<PartitionedFilterBlockReader> reader(
        NewReader(builder, pib));
    // Querying added keys
    const bool no_io = true;
    for (auto key : keys) {
      auto ikey = InternalKey(key, 0, ValueType::kTypeValue);
      const Slice ikey_slice = Slice(*ikey.rep());
      ASSERT_TRUE(reader->KeyMayMatch(key, prefix_extractor, kNotValid, !no_io,
                                      &ikey_slice, /*get_context=*/nullptr,
                                      /*lookup_context=*/nullptr));
    }
    {
      // querying a key twice
      auto ikey = InternalKey(keys[0], 0, ValueType::kTypeValue);
      const Slice ikey_slice = Slice(*ikey.rep());
      ASSERT_TRUE(reader->KeyMayMatch(
          keys[0], prefix_extractor, kNotValid, !no_io, &ikey_slice,
          /*get_context=*/nullptr, /*lookup_context=*/nullptr));
    }
    // querying missing keys
    for (auto key : missing_keys) {
      auto ikey = InternalKey(key, 0, ValueType::kTypeValue);
      const Slice ikey_slice = Slice(*ikey.rep());
      if (empty) {
        ASSERT_TRUE(reader->KeyMayMatch(
            key, prefix_extractor, kNotValid, !no_io, &ikey_slice,
            /*get_context=*/nullptr, /*lookup_context=*/nullptr));
      } else {
        // assuming a good hash function
        ASSERT_FALSE(reader->KeyMayMatch(
            key, prefix_extractor, kNotValid, !no_io, &ikey_slice,
            /*get_context=*/nullptr, /*lookup_context=*/nullptr));
      }
    }
  }

  int TestBlockPerKey() {
    std::unique_ptr<PartitionedIndexBuilder> pib(NewIndexBuilder());
    std::unique_ptr<PartitionedFilterBlockBuilder> builder(
        NewBuilder(pib.get()));
    int i = 0;
    builder->Add(keys[i]);
    CutABlock(pib.get(), keys[i], keys[i + 1]);
    i++;
    builder->Add(keys[i]);
    CutABlock(pib.get(), keys[i], keys[i + 1]);
    i++;
    builder->Add(keys[i]);
    builder->Add(keys[i]);
    CutABlock(pib.get(), keys[i], keys[i + 1]);
    i++;
    builder->Add(keys[i]);
    CutABlock(pib.get(), keys[i]);

    VerifyReader(builder.get(), pib.get());
    return CountNumOfIndexPartitions(pib.get());
  }

  void TestBlockPerTwoKeys(const SliceTransform* prefix_extractor = nullptr) {
    std::unique_ptr<PartitionedIndexBuilder> pib(NewIndexBuilder());
    std::unique_ptr<PartitionedFilterBlockBuilder> builder(
        NewBuilder(pib.get(), prefix_extractor));
    int i = 0;
    builder->Add(keys[i]);
    i++;
    builder->Add(keys[i]);
    CutABlock(pib.get(), keys[i], keys[i + 1]);
    i++;
    builder->Add(keys[i]);
    builder->Add(keys[i]);
    i++;
    builder->Add(keys[i]);
    CutABlock(pib.get(), keys[i]);

    VerifyReader(builder.get(), pib.get(), prefix_extractor);
  }

  void TestBlockPerAllKeys() {
    std::unique_ptr<PartitionedIndexBuilder> pib(NewIndexBuilder());
    std::unique_ptr<PartitionedFilterBlockBuilder> builder(
        NewBuilder(pib.get()));
    int i = 0;
    builder->Add(keys[i]);
    i++;
    builder->Add(keys[i]);
    i++;
    builder->Add(keys[i]);
    builder->Add(keys[i]);
    i++;
    builder->Add(keys[i]);
    CutABlock(pib.get(), keys[i]);

    VerifyReader(builder.get(), pib.get());
  }

  void CutABlock(PartitionedIndexBuilder* builder,
                 const std::string& user_key) {
    // Assuming a block is cut, add an entry to the index
    std::string key =
        std::string(*InternalKey(user_key, 0, ValueType::kTypeValue).rep());
    BlockHandle dont_care_block_handle(1, 1);
    builder->AddIndexEntry(&key, nullptr, dont_care_block_handle);
  }

  void CutABlock(PartitionedIndexBuilder* builder, const std::string& user_key,
                 const std::string& next_user_key) {
    // Assuming a block is cut, add an entry to the index
    std::string key =
        std::string(*InternalKey(user_key, 0, ValueType::kTypeValue).rep());
    std::string next_key = std::string(
        *InternalKey(next_user_key, 0, ValueType::kTypeValue).rep());
    BlockHandle dont_care_block_handle(1, 1);
    Slice slice = Slice(next_key.data(), next_key.size());
    builder->AddIndexEntry(&key, &slice, dont_care_block_handle);
  }

  int CountNumOfIndexPartitions(PartitionedIndexBuilder* builder) {
    IndexBuilder::IndexBlocks dont_care_ib;
    BlockHandle dont_care_bh(10, 10);
    Status s;
    int cnt = 0;
    do {
      s = builder->Finish(&dont_care_ib, dont_care_bh);
      cnt++;
    } while (s.IsIncomplete());
    return cnt - 1;  // 1 is 2nd level index
  }
};

INSTANTIATE_TEST_CASE_P(FormatDef, PartitionedFilterBlockTest,
                        testing::Values(test::kDefaultFormatVersion));
INSTANTIATE_TEST_CASE_P(FormatLatest, PartitionedFilterBlockTest,
                        testing::Values(test::kLatestFormatVersion));

TEST_P(PartitionedFilterBlockTest, EmptyBuilder) {
  std::unique_ptr<PartitionedIndexBuilder> pib(NewIndexBuilder());
  std::unique_ptr<PartitionedFilterBlockBuilder> builder(NewBuilder(pib.get()));
  const bool empty = true;
  VerifyReader(builder.get(), pib.get(), empty);
}

TEST_P(PartitionedFilterBlockTest, OneBlock) {
  uint64_t max_index_size = MaxIndexSize();
  for (uint64_t i = 1; i < max_index_size + 1; i++) {
    table_options_.metadata_block_size = i;
    TestBlockPerAllKeys();
  }
}

TEST_P(PartitionedFilterBlockTest, TwoBlocksPerKey) {
  uint64_t max_index_size = MaxIndexSize();
  for (uint64_t i = 1; i < max_index_size + 1; i++) {
    table_options_.metadata_block_size = i;
    TestBlockPerTwoKeys();
  }
}

// This reproduces the bug that a prefix is the same among multiple consecutive
// blocks but the bug would add it only to the first block.
TEST_P(PartitionedFilterBlockTest, SamePrefixInMultipleBlocks) {
  // some small number to cause partition cuts
  table_options_.metadata_block_size = 1;
  std::unique_ptr<const SliceTransform> prefix_extractor
      (rocksdb::NewFixedPrefixTransform(1));
  std::unique_ptr<PartitionedIndexBuilder> pib(NewIndexBuilder());
  std::unique_ptr<PartitionedFilterBlockBuilder> builder(
      NewBuilder(pib.get(), prefix_extractor.get()));
  const std::string pkeys[3] = {"p-key1", "p-key2", "p-key3"};
  builder->Add(pkeys[0]);
  CutABlock(pib.get(), pkeys[0], pkeys[1]);
  builder->Add(pkeys[1]);
  CutABlock(pib.get(), pkeys[1], pkeys[2]);
  builder->Add(pkeys[2]);
  CutABlock(pib.get(), pkeys[2]);
  std::unique_ptr<PartitionedFilterBlockReader> reader(
      NewReader(builder.get(), pib.get()));
  for (auto key : pkeys) {
    auto ikey = InternalKey(key, 0, ValueType::kTypeValue);
    const Slice ikey_slice = Slice(*ikey.rep());
    ASSERT_TRUE(reader->PrefixMayMatch(
        prefix_extractor->Transform(key), prefix_extractor.get(), kNotValid,
        /*no_io=*/false, &ikey_slice, /*get_context=*/nullptr,
        /*lookup_context=*/nullptr));
  }
}

TEST_P(PartitionedFilterBlockTest, OneBlockPerKey) {
  uint64_t max_index_size = MaxIndexSize();
  for (uint64_t i = 1; i < max_index_size + 1; i++) {
    table_options_.metadata_block_size = i;
    TestBlockPerKey();
  }
}

TEST_P(PartitionedFilterBlockTest, PartitionCount) {
  int num_keys = sizeof(keys) / sizeof(*keys);
  table_options_.metadata_block_size =
      std::max(MaxIndexSize(), MaxFilterSize());
  int partitions = TestBlockPerKey();
  ASSERT_EQ(partitions, 1);
  // A low number ensures cutting a block after each key
  table_options_.metadata_block_size = 1;
  partitions = TestBlockPerKey();
  ASSERT_EQ(partitions, num_keys - 1 /* last two keys make one flush */);
}

}  // namespace rocksdb

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
