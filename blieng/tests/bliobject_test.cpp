#include "bliobject_test.h"
#include "test_tools.h"
#include <bliobject.h>
#include <algorithm>
#include <string>
#include <boost/scoped_ptr.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( BliObjectTest );

using blieng::BliObject;

void BliObjectTest::values()
{
    BliObject *obj = new BliObject();

    CPPUNIT_ASSERT( !obj->isValue("test1") );

    obj->setValue("test1", (int)42);
    CPPUNIT_ASSERT( obj->isValue("test1") );

    boost::any res = obj->getValue("test1");
    CPPUNIT_ASSERT( !res.empty() );
    CPPUNIT_ASSERT( boost::any_cast<int>(res) == (int)42 );

    CPPUNIT_ASSERT_THROW( boost::any_cast<std::string>(res), boost::bad_any_cast );
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

    CPPUNIT_ASSERT( obj->getIntValue("test_int") == -42 );
    CPPUNIT_ASSERT( obj->getUIntValue("test_int") == (unsigned int)-42 );
    CPPUNIT_ASSERT( obj->getUIntValue("test_uint") == 42 );
    CPPUNIT_ASSERT( obj->getIntValue("test_uint") == 42 );
    CPPUNIT_ASSERT( obj->getDoubleValue("test_double") == 4.22 );
    CPPUNIT_ASSERT( obj->getIntValue("test_double") == 4 );
    CPPUNIT_ASSERT( obj->getStringValue("test_string") == "test42" );
    CPPUNIT_ASSERT( obj->getBoolValue("test_bool1") );
    CPPUNIT_ASSERT( !obj->getBoolValue("test_bool2") );

    {
        std::stringstream buffer;
        boost::scoped_ptr<cerr_redirect> cd(new cerr_redirect(buffer.rdbuf()));
        CPPUNIT_ASSERT( obj->getStringValue("test_int") == "" );
        std::string outp = buffer.str();

        CPPUNIT_ASSERT(outp.find("Error, not a String value at: test_int") != std::string::npos);
    }

    {
        std::stringstream buffer;
        boost::scoped_ptr<cerr_redirect> cd(new cerr_redirect(buffer.rdbuf()));
        CPPUNIT_ASSERT( obj->getBoolValue("test_int") == false );
        std::string outp = buffer.str();

        CPPUNIT_ASSERT(outp.find("Error, not a Bool value at: test_int") != std::string::npos);
    }

    {
        std::stringstream buffer;
        boost::scoped_ptr<cerr_redirect> cd(new cerr_redirect(buffer.rdbuf()));
        CPPUNIT_ASSERT_THROW( obj->getValue("test_nothing"), std::string);
        std::string outp = buffer.str();

        CPPUNIT_ASSERT(outp.find("Error, key not found: test_nothing") != std::string::npos);
    }

    CPPUNIT_ASSERT( *obj->getValueType("test_int") == typeid(int) );
    CPPUNIT_ASSERT( *obj->getValueType("test_uint") == typeid(unsigned int) );
    CPPUNIT_ASSERT( *obj->getValueType("test_double") == typeid(double) );
    CPPUNIT_ASSERT( *obj->getValueType("test_string") == typeid(std::string) );
    CPPUNIT_ASSERT( *obj->getValueType("test_bool1") == typeid(bool) );
    CPPUNIT_ASSERT( *obj->getValueType("test_bool2") == typeid(bool) );

    std::list<std::string> keys = obj->getKeys();

    CPPUNIT_ASSERT( std::find(keys.begin(), keys.end(), "test_int") != keys.end() );
    CPPUNIT_ASSERT( std::find(keys.begin(), keys.end(), "test_uint") != keys.end() );
    CPPUNIT_ASSERT( std::find(keys.begin(), keys.end(), "test_double") != keys.end() );
    CPPUNIT_ASSERT( std::find(keys.begin(), keys.end(), "test_string") != keys.end() );
    CPPUNIT_ASSERT( std::find(keys.begin(), keys.end(), "test_bool1") != keys.end() );
    CPPUNIT_ASSERT( std::find(keys.begin(), keys.end(), "test_bool2") != keys.end() );
    CPPUNIT_ASSERT( std::find(keys.begin(), keys.end(), "test_dummy") == keys.end() );
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
    CPPUNIT_ASSERT( s.find("test1: 42") != std::string::npos );
    CPPUNIT_ASSERT( s.find("test2: 99") != std::string::npos );
    CPPUNIT_ASSERT( s.find("test3: -55") != std::string::npos );
    CPPUNIT_ASSERT( s.find("test4: hello") != std::string::npos );
    CPPUNIT_ASSERT( s.find("test5: 5.99") != std::string::npos );
    CPPUNIT_ASSERT( s.find("test6: true") != std::string::npos );
    CPPUNIT_ASSERT( s.find("test9") == std::string::npos );
}

void BliObjectTest::assign()
{
    BliObject *obj1 = new BliObject();
    obj1->setValue("test1", (unsigned int)42);
    obj1->setValue("test2", (int)-5);

    BliObject *obj2 = new BliObject();
    CPPUNIT_ASSERT( obj1->isValue("test1") );
    CPPUNIT_ASSERT( obj1->isValue("test2") );
    CPPUNIT_ASSERT( !obj2->isValue("test1") );
    CPPUNIT_ASSERT( !obj2->isValue("test2") );

    obj2->assignObject(obj1);
    CPPUNIT_ASSERT( obj1->isValue("test1") );
    CPPUNIT_ASSERT( obj1->isValue("test2") );
    CPPUNIT_ASSERT( obj2->isValue("test1") );
    CPPUNIT_ASSERT( obj2->isValue("test2") );

    CPPUNIT_ASSERT( obj1->getUIntValue("test2") == obj2->getUIntValue("test2") );

    obj2->setValue("test2", (int)-4);

    CPPUNIT_ASSERT( obj1->getUIntValue("test2") != obj2->getUIntValue("test2") );
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

    CPPUNIT_ASSERT( cnt > 0 );
    CPPUNIT_ASSERT( a != b );
    CPPUNIT_ASSERT( (0 <= a) && (a <= 100));
    CPPUNIT_ASSERT( (0 <= b) && (b <= 100));

    double da = BliObject::getRandomInt(0, 1);
    double db = BliObject::getRandomInt(0, 1);
    cnt = 10;
    while (da == db && cnt > 0) {
        db = BliObject::getRandomInt(0, 100);
        cnt--;
    }

    CPPUNIT_ASSERT( cnt > 0 );
    CPPUNIT_ASSERT( da != db );

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
    CPPUNIT_ASSERT( min <= 2 );
    CPPUNIT_ASSERT( max >= 98 );
}

void BliObjectTest::incdec()
{
    BliObject *obj = new BliObject();

    obj->setValue("test1", (unsigned int)42);
    obj->setValue("test2", (int)-55);
    obj->setValue("test3", 5.99);

    obj->setValue("test1-max", (unsigned int)43);
    obj->setValue("test3-max", (unsigned int)10);

    CPPUNIT_ASSERT( obj->increase("test1") );
    CPPUNIT_ASSERT( obj->increase("test2") );
    CPPUNIT_ASSERT( obj->increase("test3") );

    CPPUNIT_ASSERT( obj->getUIntValue("test1") == 43 );
    CPPUNIT_ASSERT( obj->getIntValue("test2") == -54 );
    CPPUNIT_ASSERT( obj->getDoubleValue("test3") == 6.99 );

    CPPUNIT_ASSERT( !obj->increase("test1") );
    CPPUNIT_ASSERT( obj->increase("test2") );
    CPPUNIT_ASSERT( obj->increase("test3") );

    CPPUNIT_ASSERT( obj->getUIntValue("test1") == 43 );
    CPPUNIT_ASSERT( obj->getIntValue("test2") == -53 );
    CPPUNIT_ASSERT( obj->getDoubleValue("test3") == 7.99 );

    CPPUNIT_ASSERT( obj->increase("test3") );
    CPPUNIT_ASSERT( obj->getDoubleValue("test3") == 8.99 );

    CPPUNIT_ASSERT( obj->increase("test3") );
    CPPUNIT_ASSERT( obj->getDoubleValue("test3") == 9.99 );

    CPPUNIT_ASSERT( !obj->increase("test3") );
    CPPUNIT_ASSERT( obj->getDoubleValue("test3") == 9.99 );

    CPPUNIT_ASSERT( obj->decrease("test1") );
    CPPUNIT_ASSERT( obj->decrease("test2") );
    CPPUNIT_ASSERT( obj->decrease("test3") );

    CPPUNIT_ASSERT( obj->getUIntValue("test1") == 42 );
    CPPUNIT_ASSERT( obj->getIntValue("test2") == -54 );
    CPPUNIT_ASSERT( obj->getDoubleValue("test3") == 8.99 );
}
