set( LIB_NAME arena )

add_library( ${LIB_NAME} STATIC
    Allocator.cpp
    AllocTracker.cpp
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
    write_buffer_manager
    PRIVATE
)

