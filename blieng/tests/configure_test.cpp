#include "configure_test.h"
#include "test_tools.h"
#include <configure.h>
#include <data.h>
#include <memory>
#include <vector>

using std::unique_ptr;

CPPUNIT_TEST_SUITE_REGISTRATION( ConfigureTest );

void ConfigureTest::basic()
{
    shared_ptr<blieng::Data> data(new blieng::Data());

    blieng::Configure *obj = new blieng::Configure(data);

    obj->addKey("key1", blieng::Configure::KeyString);
    obj->addKey("key2", blieng::Configure::KeyUInt);
    obj->addKey("key3", blieng::Configure::KeyDouble);
    obj->addKey("key4", blieng::Configure::KeyIntList);

    CPPUNIT_ASSERT( !obj->validate() );

    obj->setValue("key1", std::string("val"));
    obj->setValue("key2", (unsigned int)42);
    obj->setValue("key3", (double)3.14159);
    obj->setValue("key4", 123);

    CPPUNIT_ASSERT( obj->validate() );
    CPPUNIT_ASSERT_THROW( obj->validateValues(), std::string );

    std::vector<int> int_list;
    int_list.push_back(5);
    int_list.push_back(4);
    int_list.push_back(42);

    obj->setValue("key4", int_list);

    CPPUNIT_ASSERT( obj->validate() );
    CPPUNIT_ASSERT( obj->validateValues() );
}
