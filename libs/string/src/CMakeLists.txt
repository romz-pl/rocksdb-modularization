set( LIB_NAME string )

add_library( ${LIB_NAME} STATIC
    AppendHumanBytes.cpp
    AppendHumanMicros.cpp
    AppendNumberTo.cpp
    BytesToHumanString.cpp
    EscapeChar.cpp
    EscapeOptionString.cpp
    NumberToHumanString.cpp
    NumberToString.cpp
    ParseBoolean.cpp
    ParseDouble.cpp
    ParseInt.cpp
    ParseInt32.cpp
    ParseInt32.cpp
    ParseInt64.cpp
    ParseSizeT.cpp
    ParseUint32.cpp
    ParseUint64.cpp
    ParseVectorInt.cpp
    SerializeIntVector.cpp
    StringSplit.cpp
    TimeToHumanString.cpp
    ToString.cpp
    UnescapeChar.cpp
    UnescapeOptionString.cpp
    isSpecialChar.cpp
    kNullptrString.cpp
    trim.cpp
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
    numeric_limits
    PRIVATE
)

