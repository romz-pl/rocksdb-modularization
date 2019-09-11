set( LIB_NAME container )

add_library( ${LIB_NAME} STATIC
    AlignedBuffer.cpp
    autovector.cpp
    BinaryHeap.cpp
    HashMap.cpp
    CoreLocalArray.cpp
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
    numeric_limits
    likely
    port
    random
    PRIVATE
)

