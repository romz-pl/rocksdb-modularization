#include <gtest/gtest.h>

#include <rock/string/EscapeChar.h>

//
//
TEST( string, EscapeChar )
{
    EXPECT_NO_THROW( rocksdb::EscapeChar('a') );
}
