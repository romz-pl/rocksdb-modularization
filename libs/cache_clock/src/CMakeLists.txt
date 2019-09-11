set( LIB_NAME cache_clock )

add_library( ${LIB_NAME} STATIC
    clock_cache.cpp
    NewClockCache.cpp
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

