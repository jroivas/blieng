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

    shared_ptr<blieng::Data> _data(new blieng::Data());
    shared_ptr<blieng::Configure> _config(new blieng::Configure(_data));
    Item *obj = new Item(_config, _data);

    CPPUNIT_ASSERT( obj != NULL );

    mock_io_stop();
}
