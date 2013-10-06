#include "data_test.h"
#include "test_tools.h"
#include <data.h>
#include <boost/random/random_device.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( DataTest );

using blieng::Data;

void DataTest::setUp()
{
    mock_add_folder("data");
}

void DataTest::object()
{
    mock_io_start();

    blieng::Data *obj = blieng::Data::getInstance();
    blieng::Data *obj2 = blieng::Data::getInstance();

    CPPUNIT_ASSERT( obj != NULL );
    CPPUNIT_ASSERT( obj2 != NULL );

    CPPUNIT_ASSERT( obj == obj2  );

    mock_io_stop();
}

void DataTest::gen()
{
    mock_io_start();

    blieng::Data *obj = blieng::Data::getInstance();
    CPPUNIT_ASSERT( obj != NULL );

    boost::random::random_device *gen = obj->getGen();
    CPPUNIT_ASSERT( gen != NULL);

    mock_io_stop();
}

void DataTest::readString()
{
    mock_set_file("data/string_file", "This file\nContains\nSome random strings\n\nEnd.");

    mock_io_start();

    blieng::Data *obj = blieng::Data::getInstance();
    std::string res = obj->readString("string_file");

    CPPUNIT_ASSERT( res != "" );

    mock_io_stop();
}
