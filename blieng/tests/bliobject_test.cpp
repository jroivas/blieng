#include "bliobject_test.h"
#include <bliobject.h>

using blieng::BliObject;

void BliObjectTest::values()
{
    BliObject *obj = new BliObject();

    TEST_ASSERT( !obj->isValue("test1") );

    obj->setValue("test1", (int)42);
    TEST_ASSERT( obj->isValue("test1") );

    boost::any res = obj->getValue("test1");
    TEST_ASSERT( boost::any_cast<int>(res) == (int)42 );

    TEST_THROWS( boost::any_cast<std::string>(res), boost::bad_any_cast );
}

void BliObjectTest::getValues()
{
    BliObject *obj = new BliObject();
    obj->setValue("test_int", (int)-42);
    obj->setValue("test_uint", (unsigned int)42);
    obj->setValue("test_double", (double)4.22);
    obj->setValue("test_string", std::string("test42"));
    obj->setValue("test_bool1", true);
    obj->setValue("test_bool2", false);

    TEST_ASSERT( obj->getIntValue("test_int") == -42 );
    TEST_ASSERT( obj->getUIntValue("test_int") == (unsigned int)-42 );
    TEST_ASSERT( obj->getUIntValue("test_uint") == 42 );
    TEST_ASSERT( obj->getIntValue("test_uint") == 42 );
    TEST_ASSERT( obj->getDoubleValue("test_double") == 4.22 );
    TEST_ASSERT( obj->getIntValue("test_double") == 4 );
    TEST_ASSERT( obj->getStringValue("test_string") == "test42" );
    TEST_ASSERT( obj->getBoolValue("test_bool1") );
    TEST_ASSERT( !obj->getBoolValue("test_bool2") );
}
