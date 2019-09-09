#pragma once

#if defined(__GNUC__) || defined(__clang__)
#define ROCKSDB_PRINTF_FORMAT_ATTR(format_param, dots_param) \
    __attribute__((__format__(__printf__, format_param, dots_param)))
#else
#define ROCKSDB_PRINTF_FORMAT_ATTR(format_param, dots_param)
#endif
