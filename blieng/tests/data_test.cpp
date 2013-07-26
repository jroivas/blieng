#include "data_test.h"
#include <data.h>

CPPUNIT_TEST_SUITE_REGISTRATION( DataTest );

using blieng::Data;

void DataTest::object()
{
    blieng::Data *obj = blieng::Data::getInstance();
    blieng::Data *obj2 = blieng::Data::getInstance();

    CPPUNIT_ASSERT( obj != NULL );
    CPPUNIT_ASSERT( obj2 != NULL );

    CPPUNIT_ASSERT( obj == obj2  );
}
