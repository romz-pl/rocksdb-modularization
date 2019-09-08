set( LIB_NAME string )

add_library( ${LIB_NAME} STATIC
    ToString.cpp
    StringSplit.cpp
    AppendNumberTo.cpp
    NumberToString.cpp
    NumberToHumanString.cpp
    BytesToHumanString.cpp
    TimeToHumanString.cpp
    AppendHumanMicros.cpp
    AppendHumanBytes.cpp
    isSpecialChar.cpp
    UnescapeChar.cpp
    EscapeChar.cpp
    EscapeOptionString.cpp
    UnescapeOptionString.cpp
    trim.cpp
    ParseBoolean.cpp
    ParseUint32.cpp
    ParseInt32.cpp
    ParseUint64.cpp
    ParseInt64.cpp
    ParseInt.cpp
    ParseDouble.cpp
    ParseSizeT.cpp
    ParseVectorInt.cpp
    SerializeIntVector.cpp
    kNullptrString.cpp
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
    PRIVATE
)

