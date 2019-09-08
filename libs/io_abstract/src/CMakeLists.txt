set( LIB_NAME io_abstract )

add_library( ${LIB_NAME} STATIC
    SequentialFile.cpp
    ReadRequest.cpp
    RandomAccessFile.cpp
    kDefaultPageSize.cpp
    WritableFile.cpp
    RandomRWFile.cpp
    MemoryMappedFileBuffer.cpp
    Directory.cpp
    SequentialFileWrapper.cpp
    RandomAccessFileWrapper.cpp
    RandomRWFileWrapper.cpp
    DirectoryWrapper.cpp
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

