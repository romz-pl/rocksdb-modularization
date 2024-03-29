set( LIB_NAME memtable )

add_library( ${LIB_NAME} STATIC
    SkipList.cpp
    InlineSkipList.cpp
    KeyHandle.cpp
    MemTableRep.cpp
    LookupKey.cpp
    MemTableRepFactory.cpp
    SkipListFactory.cpp
    VectorRepFactory.cpp
    NewHashSkipListRepFactory.cpp
    NewHashLinkListRepFactory.cpp
    HashSkipListRepFactory.cpp
    HashLinkListRepFactory.cpp
    Key.cpp
    MemtableSkipList.cpp
    Pointer.cpp
    BucketHeader.cpp
    SkipListBucketHeader.cpp
    Node.cpp
    HashLinkListRep.cpp
    EncodeKey.cpp
)

target_compile_options( ${LIB_NAME} PRIVATE -fPIC )

target_include_directories(
    ${LIB_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(
    ${LIB_NAME}
    PUBLIC
    arena
    hash
    sn
    PRIVATE
)

