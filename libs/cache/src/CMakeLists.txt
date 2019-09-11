set( LIB_NAME cache )

add_library( ${LIB_NAME} STATIC
    Cache.cpp
    NewClockCache.cpp
    clock_cache.cpp
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
    statistics
    PRIVATE
)

