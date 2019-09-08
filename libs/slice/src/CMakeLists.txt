set( LIB_NAME slice )

add_library( ${LIB_NAME} STATIC
    Slice.cpp
    SliceParts.cpp
    AppendEscapedStringTo.cpp
    EscapeString.cpp
    ConsumeDecimalNumber.cpp
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

