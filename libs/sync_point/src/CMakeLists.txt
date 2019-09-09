set( LIB_NAME sync_point )

add_library( ${LIB_NAME} STATIC
    sync_point.cpp
    sync_point_impl.cpp
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
    port
    random
    PRIVATE
)

