#include "configure_test.h"
#include "test_tools.h"
#include <configure.h>
#include <data.h>
#include <memory>
#include <vector>

using std::unique_ptr;

CPPUNIT_TEST_SUITE_REGISTRATION( ConfigureTest );

void ConfigureTest::setUp()
{
    mock_add_folder("data");
    mock_io_start();
}

void ConfigureTest::tearDown()
{
    mock_io_stop();
}

void ConfigureTest::basic()
{
    boost::shared_ptr<blieng::BliengState> _state(new blieng::BliengState());
    _state->setData(new blieng::Data());
    blieng::Configure *obj = new blieng::Configure(_state);
    _state->setConfig(obj);

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

void ConfigureTest::json()
{
    std::string origdata = "{ \"strkey\": \"strval\", \"intkey\": 42, \"doublekey\": 3.14, \"test\": [1, 2, 4] }";
    mock_set_file("data/data.json", origdata);

    boost::shared_ptr<blieng::BliengState> _state(new blieng::BliengState());
    _state->setData(new blieng::Data());
    blieng::Configure *obj = new blieng::Configure(_state);
    _state->setConfig(obj);

    obj->addKey("strkey", blieng::Configure::KeyString);
    obj->addKey("intkey", blieng::Configure::KeyUInt);
    obj->addKey("doublekey", blieng::Configure::KeyDouble);
    obj->addKey("test", blieng::Configure::KeyIntList);

    CPPUNIT_ASSERT( obj->load("data.json") );
    CPPUNIT_ASSERT( obj->validate() );
    CPPUNIT_ASSERT( obj->validateValues() );

    CPPUNIT_ASSERT_EQUAL( std::string("strval"), obj->getStringValue("strkey") );
    CPPUNIT_ASSERT_EQUAL( 42, obj->getIntValue("intkey"));
    CPPUNIT_ASSERT_EQUAL( (double)3.14, obj->getDoubleValue("doublekey") );

    std::vector<int> ilist = obj->getIntValues("test");

    CPPUNIT_ASSERT(ilist.size() == 3);
    CPPUNIT_ASSERT_EQUAL(ilist[0], 1);
    CPPUNIT_ASSERT_EQUAL(ilist[1], 2);
    CPPUNIT_ASSERT_EQUAL(ilist[2], 4);

}

void ConfigureTest::optional()
{
    std::string origdata = "{ \"strkey\": \"strval\", \"intkey\": 42 }";
    std::string origdata2 = "{ \"strkey\": \"strval\", \"intkey\": 42, \"extra\": 123 }";
    mock_set_file("data/data.json", origdata);
    mock_set_file("data/data2.json", origdata2);

    boost::shared_ptr<blieng::BliengState> _state(new blieng::BliengState());
    _state->setData(new blieng::Data());
    blieng::Configure *obj = new blieng::Configure(_state);
    _state->setConfig(obj);

    obj->addKey("strkey", blieng::Configure::KeyString);
    obj->addKey("intkey", blieng::Configure::KeyUInt);
    obj->addOptionalKey("extra", blieng::Configure::KeyUInt);

    CPPUNIT_ASSERT( obj->load("data.json") );
    CPPUNIT_ASSERT( obj->validate() );
    CPPUNIT_ASSERT( obj->validateValues() );

    CPPUNIT_ASSERT_EQUAL( std::string("strval"), obj->getStringValue("strkey") );
    CPPUNIT_ASSERT_EQUAL( 42, obj->getIntValue("intkey") );
    CPPUNIT_ASSERT( !obj->isValue("extra") );

    CPPUNIT_ASSERT( obj->load("data2.json") );

    CPPUNIT_ASSERT_EQUAL( obj->getStringValue("strkey"), std::string("strval") );
    CPPUNIT_ASSERT_EQUAL( obj->getIntValue("intkey"), 42 );
    CPPUNIT_ASSERT( obj->isValue("extra") );
    CPPUNIT_ASSERT_EQUAL( obj->getIntValue("extra"), 123 );
}
