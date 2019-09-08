set( LIB_NAME numeric_limits )

add_library( ${LIB_NAME} STATIC
    numeric_limits.cpp
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
    PRIVATE
)

