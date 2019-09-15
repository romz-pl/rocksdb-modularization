#pragma once

#include <rock/memtable/Key.h>
#include <rock/memtable/MemTableRep.h>
#include <rock/memtable/SkipList.h>

namespace rocksdb {

typedef SkipList<Key, const MemTableRep::KeyComparator&> MemtableSkipList;

}
