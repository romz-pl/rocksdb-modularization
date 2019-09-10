set( LIB_NAME hash )

add_library( ${LIB_NAME} STATIC
    hash.cpp
    murmurhash.cpp
    xxhash.cpp
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
    status
    PRIVATE
)

