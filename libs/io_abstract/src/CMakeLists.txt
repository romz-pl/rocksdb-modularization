set( LIB_NAME io_abstract )

add_library( ${LIB_NAME} STATIC
    Directory.cpp
    DirectoryWrapper.cpp
    IOPriority.cpp
    MemoryMappedFileBuffer.cpp
    RandomAccessFile.cpp
    RandomAccessFileWrapper.cpp
    RandomRWFile.cpp
    RandomRWFileWrapper.cpp
    ReadRequest.cpp
    SequentialFile.cpp
    SequentialFileWrapper.cpp
    WritableFile.cpp
    WritableFile.cpp
    WritableFileWrapper.cpp
    WriteLifeTimeHint.cpp
    kDefaultPageSize.cpp
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

