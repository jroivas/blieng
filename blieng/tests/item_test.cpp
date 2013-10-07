#include "item_test.h"
#include "test_tools.h"
#include <bliobject.h>
#include <item.h>
#include <memory>

using std::unique_ptr;

CPPUNIT_TEST_SUITE_REGISTRATION( ItemTest );

using blieng::Item;

void ItemTest::basic()
{
    mock_io_start();

    Item *obj = new Item();

    CPPUNIT_ASSERT( obj != NULL );

    mock_io_stop();
}
