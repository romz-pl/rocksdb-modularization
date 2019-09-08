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

