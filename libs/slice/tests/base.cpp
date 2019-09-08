#include <gtest/gtest.h>

#include <rock/slice/Slice.h>
#include <rock/slice/SliceParts.h>

//
//
TEST( slice, constructor )
{
    EXPECT_NO_THROW( rocksdb::Slice() );
    EXPECT_NO_THROW( rocksdb::SliceParts() );
}


