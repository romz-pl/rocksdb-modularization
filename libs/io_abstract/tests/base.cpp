#include <gtest/gtest.h>

#include <rock/io_abstract/SequentialFileWrapper.h>


//
//
TEST( io_abstract, SequentialFile )
{
    rocksdb::SequentialFile* target = nullptr;
    EXPECT_NO_THROW( rocksdb::SequentialFileWrapper{ target } );
}


