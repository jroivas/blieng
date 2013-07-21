#include "bliobject_test.h"
#include <bliobject.h>
#include <algorithm>

using blieng::BliObject;

void BliObjectTest::values()
{
    BliObject *obj = new BliObject();

    TEST_ASSERT( !obj->isValue("test1") );

    obj->setValue("test1", (int)42);
    TEST_ASSERT( obj->isValue("test1") );

    boost::any res = obj->getValue("test1");
    TEST_ASSERT( !res.empty() );
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

    // TODO: Check for output...
    TEST_ASSERT( obj->getStringValue("test_int") == "" );
    TEST_ASSERT( obj->getBoolValue("test_int") == false );

    TEST_ASSERT( *obj->getValueType("test_int") == typeid(int) );
    TEST_ASSERT( *obj->getValueType("test_uint") == typeid(unsigned int) );
    TEST_ASSERT( *obj->getValueType("test_double") == typeid(double) );
    TEST_ASSERT( *obj->getValueType("test_string") == typeid(std::string) );
    TEST_ASSERT( *obj->getValueType("test_bool1") == typeid(bool) );
    TEST_ASSERT( *obj->getValueType("test_bool2") == typeid(bool) );

    std::list<std::string> keys = obj->getKeys();

    TEST_ASSERT( std::find(keys.begin(), keys.end(), "test_int") != keys.end() );
    TEST_ASSERT( std::find(keys.begin(), keys.end(), "test_uint") != keys.end() );
    TEST_ASSERT( std::find(keys.begin(), keys.end(), "test_double") != keys.end() );
    TEST_ASSERT( std::find(keys.begin(), keys.end(), "test_string") != keys.end() );
    TEST_ASSERT( std::find(keys.begin(), keys.end(), "test_bool1") != keys.end() );
    TEST_ASSERT( std::find(keys.begin(), keys.end(), "test_bool2") != keys.end() );
    TEST_ASSERT( std::find(keys.begin(), keys.end(), "test_dummy") == keys.end() );
}

void BliObjectTest::toString()
{
    BliObject *obj = new BliObject();
    obj->setValue("test1", (unsigned int)42);
    obj->setValue("test2", (unsigned int)99);
    obj->setValue("test3", (int)-55);
    obj->setValue("test4", std::string("hello"));
    obj->setValue("test5", 5.99);
    obj->setValue("test6", true);

    std::string s = obj->toString();
    TEST_ASSERT( s.find("test1: 42") != std::string::npos )
    TEST_ASSERT( s.find("test2: 99") != std::string::npos )
    TEST_ASSERT( s.find("test3: -55") != std::string::npos )
    TEST_ASSERT( s.find("test4: hello") != std::string::npos )
    TEST_ASSERT( s.find("test5: 5.99") != std::string::npos )
    TEST_ASSERT( s.find("test6: true") != std::string::npos )
    TEST_ASSERT( s.find("test9") == std::string::npos )
}

void BliObjectTest::assign()
{
    BliObject *obj1 = new BliObject();
    obj1->setValue("test1", (unsigned int)42);
    obj1->setValue("test2", (int)-5);

    BliObject *obj2 = new BliObject();
    TEST_ASSERT( obj1->isValue("test1") );
    TEST_ASSERT( obj1->isValue("test2") );
    TEST_ASSERT( !obj2->isValue("test1") );
    TEST_ASSERT( !obj2->isValue("test2") );

    obj2->assignObject(obj1);
    TEST_ASSERT( obj1->isValue("test1") );
    TEST_ASSERT( obj1->isValue("test2") );
    TEST_ASSERT( obj2->isValue("test1") );
    TEST_ASSERT( obj2->isValue("test2") );

    TEST_ASSERT( obj1->getUIntValue("test2") == obj2->getUIntValue("test2") );

    obj2->setValue("test2", (int)-4);

    TEST_ASSERT( obj1->getUIntValue("test2") != obj2->getUIntValue("test2") );
}

void BliObjectTest::random()
{
    int a = BliObject::getRandomInt(0, 100);
    int b = BliObject::getRandomInt(0, 100);
    int cnt = 10;
    while (a == b && cnt > 0) {
        b = BliObject::getRandomInt(0, 100);
        cnt--;
    }

    TEST_ASSERT( cnt > 0 );
    TEST_ASSERT( a != b );
    TEST_ASSERT( (0 <= a) && (a <= 100));
    TEST_ASSERT( (0 <= b) && (b <= 100));

    double da = BliObject::getRandomInt(0, 1);
    double db = BliObject::getRandomInt(0, 1);
    cnt = 10;
    while (da == db && cnt > 0) {
        db = BliObject::getRandomInt(0, 100);
        cnt--;
    }

    TEST_ASSERT( cnt > 0 );
    TEST_ASSERT( da != db );

    int min = 200;
    int max = -1;
    cnt = 2000;
    while (cnt > 0) {
        a = BliObject::getRandomInt(0, 100);
        if (a > max) max = a;
        if (a < min) min = a;
        cnt--;
    }
    // In fact this can fail, but probably will not...
    TEST_ASSERT( min <= 2 );
    TEST_ASSERT( max >= 98 );
}
