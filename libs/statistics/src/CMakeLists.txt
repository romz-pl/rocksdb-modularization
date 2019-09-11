set( LIB_NAME statistics )

add_library( ${LIB_NAME} STATIC
    Tickers.cpp
    TickersNameMap.cpp
    Histograms.cpp
    HistogramsNameMap.cpp
    HistogramData.cpp
    StatsLevel.cpp
    Statistics.cpp
    CreateDBStatistics.cpp
    HistogramBucketMapper.cpp
    HistogramStat.cpp
    Histogram.cpp
    HistogramImpl.cpp
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
    port
    status
    PRIVATE
)

