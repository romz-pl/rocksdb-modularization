set( LIB_NAME memory_allocator )

add_library( ${LIB_NAME} STATIC
    MemoryAllocator.cpp
    JemallocAllocatorOptions.cpp
    NewJemallocNodumpAllocator.cpp
    CustomDeleter.cpp
    
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
    posrt
    status
    PRIVATE
)

