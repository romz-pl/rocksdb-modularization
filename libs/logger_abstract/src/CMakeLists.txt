set( LIB_NAME logger_abstract )

add_library( ${LIB_NAME} STATIC
    Debug.cpp
    Error.cpp
    Fatal.cpp
    Header.cpp
    Info.cpp
    InfoLogLevel.cpp
    Log.cpp
    LogFlush.cpp
    Logger.cpp
    LoggerWrapper.cpp
    ROCKSDB_PRINTF_FORMAT_ATTR.cpp
    Warn.cpp
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
    status
    PRIVATE
)

