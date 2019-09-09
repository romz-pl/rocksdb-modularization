set( LIB_NAME port )

set( SOURCES
    port.cpp
    port_dirent.cpp
    port_posix.cpp
    stack_trace.cpp
    sys_time.cpp
    xpress.cpp
)

if( NOT WIN32 )

    list(APPEND SOURCES port_posix.cpp )

endif()

add_library( ${LIB_NAME} STATIC ${SOURCES} )

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

