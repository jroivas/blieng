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
    boost::shared_ptr<blieng::BliengState> _state(new blieng::BliengState());
    _state->setData(new blieng::Data());
    _state->setConfig(new blieng::Configure(_state));

    Item *obj = new Item(_state);

    mock_io_stop();
    CPPUNIT_ASSERT( obj != NULL );
}
