set( LIB_NAME cache_lru )

add_library( ${LIB_NAME} STATIC
    LRUHandle.cpp
    LRUHandleTable.cpp
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
    cache
    PRIVATE
)

