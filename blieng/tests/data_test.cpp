#include "data_test.h"
#include <data.h>

using blieng::Data;

void DataTest::object()
{
    blieng::Data *obj = blieng::Data::getInstance();
    blieng::Data *obj2 = blieng::Data::getInstance();

    TEST_ASSERT( obj != NULL );
    TEST_ASSERT( obj2 != NULL );

    TEST_ASSERT( obj == obj2  );
}
