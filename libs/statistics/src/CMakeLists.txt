set( LIB_NAME statistics )

add_library( ${LIB_NAME} STATIC
    CreateDBStatistics.cpp
    Histogram.cpp
    HistogramBucketMapper.cpp
    HistogramData.cpp
    HistogramImpl.cpp
    HistogramStat.cpp
    Histograms.cpp
    HistogramsInternal.cpp
    HistogramsNameMap.cpp
    RecordInHistogram.cpp
    RecordTick.cpp
    RecordTimeToHistogram.cpp
    SetTickerCount.cpp
    Statistics.cpp
    StatisticsImpl.cpp
    StatsLevel.cpp
    Tickers.cpp
    TickersInternal.cpp
    TickersNameMap.cpp
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
    container
    mutex
    status
    PRIVATE
)

