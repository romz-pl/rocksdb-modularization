set( LIB_NAME memtable )

add_library( ${LIB_NAME} STATIC
    SkipList.cpp
    InlineSkipList.cpp
    KeyHandle.cpp
    MemTableRep.cpp
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
    arena
    PRIVATE
)

