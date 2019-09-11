set( LIB_NAME jemalloc )

add_library( ${LIB_NAME} STATIC
    JemallocAllocatorOptions.cpp
    NewJemallocNodumpAllocator.cpp
    jemalloc_nodump_allocator.cpp
    jemalloc_helper.cpp
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
    memory_allocator
    port
    PRIVATE
)

