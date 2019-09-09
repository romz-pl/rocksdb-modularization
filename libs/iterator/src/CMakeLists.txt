set( LIB_NAME iterator )

add_library( ${LIB_NAME} STATIC
    Cleanable.cpp
    Iterator.cpp
    EmptyIterator.cpp
    EmptyInternalIterator.cpp
    NewEmptyIterator.cpp
    NewErrorIterator.cpp
    PinnableSlice.cpp
    InternalIteratorBase.cpp
    IterateResult.cpp
    InternalIterator.cpp
    EmptyInternalIterator.cpp
    IteratorWrapperBase.cpp
    IteratorWrapper.cpp
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

