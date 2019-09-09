set( LIB_NAME slice )

add_library( ${LIB_NAME} STATIC
    AppendEscapedStringTo.cpp
    BytewiseComparator.cpp
    BytewiseComparatorImpl.cpp
    CappedPrefixTransform.cpp
    Comparator.cpp
    ConsumeDecimalNumber.cpp
    EscapeString.cpp
    ExtractUserKey.cpp
    FixedPrefixTransform.cpp
    NewCappedPrefixTransform.cpp
    NewFixedPrefixTransform.cpp
    NewNoopTransform.cpp
    NoopTransform.cpp
    ReverseBytewiseComparator.cpp
    ReverseBytewiseComparatorImpl.cpp
    Slice.cpp
    SliceParts.cpp
    SliceTransform.cpp
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
    string
    PRIVATE
)

