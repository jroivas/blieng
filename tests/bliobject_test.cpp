#include "bliobject_test.h"
#include "test_tools.h"
#include <bliobject.h>
#include <blieng.h>
#include <algorithm>
#include <string>
#include <boost/scoped_ptr.hpp>
#include "test_tools.h"

CPPUNIT_TEST_SUITE_REGISTRATION( BliObjectTest );

using blieng::BliObject;
using blieng::BliAny;

void BliObjectTest::setUp()
{
    //mock_add_folder("data");
    mock_io_start();
}

void BliObjectTest::tearDown()
{
    mock_io_stop();
}

void BliObjectTest::values()
{
    BliObject *obj = new BliObject();

    CPPUNIT_ASSERT(!obj->isValue("test1"));

    obj->setValue("test1", (int)42);
    CPPUNIT_ASSERT(obj->isValue("test1"));

    blieng::BliAny res = obj->getValue("test1");
    CPPUNIT_ASSERT(!res.empty());
    CPPUNIT_ASSERT(boost::any_cast<int>(res) == (int)42);

    CPPUNIT_ASSERT_THROW(boost::any_cast<std::string>(res), boost::bad_any_cast);
}

void BliObjectTest::getValuesLimits()
{
    BliObject *obj = new BliObject();
    obj->setValue("test_int", (int)42);
    obj->setValue("test_int_min", (int)-42);
    obj->setValue("test_uint", (unsigned int)42);
    obj->setValue("test_double", (double)4.22);
    obj->setValue("test_char", (char)3);
    obj->setValue("test_char_min", (char)-1);
    obj->setValue("test_long", (long)123235);
    obj->setValue("test_long_long", (long long)1234567890123L);


    CPPUNIT_ASSERT(obj->getIntValue("test_int") == 42);
    CPPUNIT_ASSERT(obj->getIntValue("test_int_min") == -42);

    CPPUNIT_ASSERT(obj->getIntValue("test_uint") == 42);
    CPPUNIT_ASSERT(obj->getIntValue("test_double") == 4);
    CPPUNIT_ASSERT(obj->getDoubleValue("test_double") == 4.22);

    CPPUNIT_ASSERT(obj->getIntValue("test_char") == 3);
    CPPUNIT_ASSERT(obj->getIntValue("test_char_min") == -1);

    CPPUNIT_ASSERT(obj->getIntValue("test_long") == 123235);

    CPPUNIT_ASSERT_THROW(
        obj->getIntValue("test_long_long"),
        std::string);
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


    CPPUNIT_ASSERT(obj->getIntValue("test_int") == -42 );
    CPPUNIT_ASSERT_THROW(
        obj->getUIntValue("test_int"),
        std::string);
    CPPUNIT_ASSERT( obj->getUIntValue("test_uint") == 42 );
    CPPUNIT_ASSERT( obj->getIntValue("test_uint") == 42 );
    CPPUNIT_ASSERT( obj->getDoubleValue("test_double") == 4.22 );
    CPPUNIT_ASSERT( obj->getIntValue("test_double") == 4 );
    CPPUNIT_ASSERT( obj->getStringValue("test_string") == "test42" );
    CPPUNIT_ASSERT( obj->getBoolValue("test_bool1") );
    CPPUNIT_ASSERT( !obj->getBoolValue("test_bool2") );
    CPPUNIT_ASSERT_THROW(
        obj->getBoolValue("test_int"),
        std::string);
    CPPUNIT_ASSERT_THROW(
        obj->getStringValue("test_int"),
        std::string);

    {
        std::stringstream buffer;
        boost::scoped_ptr<cerr_redirect> cd(new cerr_redirect(buffer.rdbuf()));
        CPPUNIT_ASSERT_THROW( obj->getValue("test_nothing"), std::string);
        std::string outp = buffer.str();

        CPPUNIT_ASSERT(
            outp.find("Error, key not found: test_nothing")
            != std::string::npos);
    }


    CPPUNIT_ASSERT( *obj->getValueType("test_int") == typeid(int) );
    CPPUNIT_ASSERT( *obj->getValueType("test_uint") == typeid(unsigned int) );
    CPPUNIT_ASSERT( *obj->getValueType("test_double") == typeid(double) );
    CPPUNIT_ASSERT( *obj->getValueType("test_string") == typeid(std::string) );
    CPPUNIT_ASSERT( *obj->getValueType("test_bool1") == typeid(bool) );
    CPPUNIT_ASSERT( *obj->getValueType("test_bool2") == typeid(bool) );

    std::vector<std::string> keys = obj->getKeys();

    CPPUNIT_ASSERT(
        std::find(keys.begin(), keys.end(), "test_int")
        != keys.end());
    CPPUNIT_ASSERT(
        std::find(keys.begin(), keys.end(), "test_uint")
        != keys.end());
    CPPUNIT_ASSERT(
        std::find(keys.begin(), keys.end(), "test_double")
        != keys.end());
    CPPUNIT_ASSERT(
        std::find(keys.begin(), keys.end(), "test_string")
        != keys.end());
    CPPUNIT_ASSERT(
        std::find(keys.begin(), keys.end(), "test_bool1")
        != keys.end());
    CPPUNIT_ASSERT(
        std::find(keys.begin(), keys.end(), "test_bool2")
        != keys.end());
    CPPUNIT_ASSERT(
        std::find(keys.begin(), keys.end(), "test_dummy")
        == keys.end());
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

void BliObjectTest::percentageString()
{
    CPPUNIT_ASSERT(blieng::percentageString(0.1) == "10%");
// Unfortunately Ubuntu 12.04 has bug in glibc which causes these to fail on i386
#ifndef __i386__
    CPPUNIT_ASSERT(blieng::percentageString(0.12) == "12%");

    CPPUNIT_ASSERT(blieng::percentageString(0.01) == "1%");
    CPPUNIT_ASSERT(blieng::percentageString(0.012) == "1%");
    CPPUNIT_ASSERT(blieng::percentageString(1.0) == "100%");
    CPPUNIT_ASSERT(blieng::percentageString(1.23) == "123%");

    CPPUNIT_ASSERT(blieng::percentageString(0.12345678) == "12%");
    CPPUNIT_ASSERT(blieng::percentageString(0.12345678, 1) == "12.3%");
    CPPUNIT_ASSERT(blieng::percentageString(0.12345678, 2) == "12.34%");
    CPPUNIT_ASSERT(blieng::percentageString(0.12345678, 3) == "12.345%");
    CPPUNIT_ASSERT(blieng::percentageString(0.12345678, 4) == "12.3456%");

    CPPUNIT_ASSERT(blieng::percentageString(12345678) == "1234567800%");
    CPPUNIT_ASSERT(blieng::percentageString(123456.78) == "12345678%");
    CPPUNIT_ASSERT(blieng::percentageString(123456.78901234567) == "12345678%");
    CPPUNIT_ASSERT(blieng::percentageString(123456.78901234567, 1) == "12345678.9%");
    CPPUNIT_ASSERT(blieng::percentageString(123456.78901234567, 2) == "12345678.90%");
    CPPUNIT_ASSERT(blieng::percentageString(123456.78901234567, 3) == "12345678.901%");
    CPPUNIT_ASSERT(blieng::percentageString(123456.78901234567, 5) == "12345678.90123%");

    CPPUNIT_ASSERT(blieng::percentageString(-0.1) == "-10%");
    CPPUNIT_ASSERT(blieng::percentageString(-0.012) == "-1%");
    CPPUNIT_ASSERT(blieng::percentageString(-0.12345678) == "-12%");
    CPPUNIT_ASSERT(blieng::percentageString(-0.12345678, 1) == "-12.3%");
    CPPUNIT_ASSERT(blieng::percentageString(-0.12345678, 2) == "-12.34%");
    CPPUNIT_ASSERT(blieng::percentageString(-0.12345678, 4) == "-12.3456%");
#endif
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

    CPPUNIT_ASSERT( obj1->getIntValue("test1") == obj2->getIntValue("test1") );
    CPPUNIT_ASSERT( obj1->getIntValue("test2") == obj2->getIntValue("test2") );

    obj2->setValue("test2", (int)-4);

    CPPUNIT_ASSERT( obj1->getIntValue("test2") != obj2->getIntValue("test2") );
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

void BliObjectTest::bliany()
{
    BliAny tmp1((unsigned int)1);
    BliAny tmp2(2.3);
    BliAny tmp3(std::string("res"));
    BliAny tmp4(true);
    BliAny tmp5(false);
    BliAny tmp6(-1);

    CPPUNIT_ASSERT( tmp1.asUInt() == 1);
    CPPUNIT_ASSERT( tmp1.asNumber() == 1);
    CPPUNIT_ASSERT( tmp2.asDouble() == 2.3);
    CPPUNIT_ASSERT( tmp2.asNumber() == 2);
    CPPUNIT_ASSERT( tmp3.asString() == "res");
    CPPUNIT_ASSERT( tmp4.asBool() );
    CPPUNIT_ASSERT( !tmp5.asBool() );
    CPPUNIT_ASSERT( tmp6.asInt() == -1 );
    CPPUNIT_ASSERT( tmp6.asNumber() == -1 );

    CPPUNIT_ASSERT_THROW( tmp1.asDouble(), std::string );
    CPPUNIT_ASSERT_THROW( tmp2.asInt(), std::string );
}

void BliObjectTest::serialize()
{
    BliObject *obj = new BliObject();
    obj->setValue("test_int", (int)42);
    obj->setValue("test_int_min", (int)-42);
    obj->setValue("test_uint", (unsigned int)42);
    obj->setValue("test_double", (double)4.22);
    obj->setValue("test_char", (char)3);
    obj->setValue("test_char_min", (char)-1);
    obj->setValue("test_long", (long)123235);
    obj->setValue("test_long_long", (long long)1234567890123L);

    std::string res = obj->serialize();

    CPPUNIT_ASSERT( res != "" );

    BliObject *obj2 = new BliObject();
    CPPUNIT_ASSERT(obj->isValue("test_int"));
    CPPUNIT_ASSERT(!obj2->isValue("test_int"));

    obj2->deserialize(res);
    CPPUNIT_ASSERT(obj2->isValue("test_int"));
    CPPUNIT_ASSERT_EQUAL(obj2->getIntValue("test_int"), (int)42);
    CPPUNIT_ASSERT_EQUAL(obj2->getCharValue("test_char"), (char)3);
    CPPUNIT_ASSERT_EQUAL(obj2->getCharValue("test_char_min"), (char)-1);
    CPPUNIT_ASSERT_EQUAL(obj2->getDoubleValue("test_double"), (double)4.22);
    CPPUNIT_ASSERT_EQUAL(obj2->getLongLongValue("test_long_long"), (long long)1234567890123L);

    std::string res2 = obj->serialize("Random");
    CPPUNIT_ASSERT(obj->serializedType(res) == "BliObject");
    CPPUNIT_ASSERT(BliObject::serializedType(res) == "BliObject");
    CPPUNIT_ASSERT(obj->serializedType(res2) == "Random");
}

void BliObjectTest::compress()
{
    std::string original = "inadub792b91beaaaaaaaaaaaaaaabbbbbbbbbbbbbbbb";
    char *comp = nullptr;
    unsigned int len;
    std::tie(
        comp,
        len) = blieng::compress(original.c_str(), original.size());

    CPPUNIT_ASSERT(comp != nullptr);
    CPPUNIT_ASSERT(len > 0);
    CPPUNIT_ASSERT(len < original.size());
    CPPUNIT_ASSERT(!blieng::isCompressed(original.c_str(), original.size()));
    CPPUNIT_ASSERT(blieng::isCompressed(comp, len));

    char *uncomp = nullptr;
    unsigned int uncomp_len;
    std::tie(
        uncomp,
        uncomp_len) = blieng::decompress(comp, len);

    CPPUNIT_ASSERT(uncomp != nullptr);
    CPPUNIT_ASSERT(uncomp_len > 0);
    CPPUNIT_ASSERT(uncomp_len == original.size());
    CPPUNIT_ASSERT(std::string(uncomp, uncomp_len) == original);

    CPPUNIT_ASSERT(blieng::isCompressed(comp, len));
    CPPUNIT_ASSERT(!blieng::isCompressed(uncomp, uncomp_len));

    delete [] uncomp;
    delete [] comp;
}

void BliObjectTest::uuid()
{
    blieng::BliObject tmp1;

    for (unsigned int i = 0; i < 100; ++i) {
        blieng::BliObject tmp2;
        CPPUNIT_ASSERT( tmp1.uuid() != tmp2.uuid() );
    }
}
