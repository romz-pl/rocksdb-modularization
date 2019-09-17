set( LIB_NAME dbformat )

add_library( ${LIB_NAME} STATIC
    ValueType.cpp
    kValueTypeForSeek.cpp
    kValueTypeForSeekForPrev.cpp
    IsValueType.cpp
    IsExtendedValueType.cpp
    kMaxSequenceNumber.cpp
    kDisableGlobalSequenceNumber.cpp
    ParsedInternalKey.cpp
    InternalKeyEncodingLength.cpp
    PackSequenceAndType.cpp
    UnPackSequenceAndType.cpp
    EntryType.cpp
    GetEntryType.cpp
    AppendInternalKey.cpp
    AppendInternalKeyFooter.cpp
    ParseInternalKey.cpp
    ExtractUserKeyAndStripTimestamp.cpp
    StripTimestampFromUserKey.cpp
    ExtractInternalKeyFooter.cpp
    ExtractValueType.cpp
    InternalKeyComparator.cpp
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
    coding
    sn
    PRIVATE
)

