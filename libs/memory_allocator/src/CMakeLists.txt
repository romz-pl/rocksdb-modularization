set( LIB_NAME memory_allocator )

add_library( ${LIB_NAME} STATIC
    CustomDeleter.cpp
    MemoryAllocator.cpp
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
    likely
    status
    PRIVATE
)

