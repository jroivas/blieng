#include "bliany_test.h"
#include <bliany.h>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION( BliAnyTest );

void BliAnyTest::basic()
{
    blieng::BliAny tmp;

    CPPUNIT_ASSERT_THROW( tmp.asInt(), std::string );
    CPPUNIT_ASSERT_THROW( tmp.asString(), std::string );

    tmp = blieng::BliAny(10);
    CPPUNIT_ASSERT( tmp.asInt() == 10 );

    tmp = blieng::BliAny(-10);
    CPPUNIT_ASSERT( tmp.asInt() == -10 );

    tmp = blieng::BliAny(123456789U);
    CPPUNIT_ASSERT( tmp.asUInt() == 123456789U );

    tmp = blieng::BliAny(std::string("hello"));
    CPPUNIT_ASSERT( tmp.asString() == "hello" );

    tmp = blieng::BliAny((float)1.2);
    CPPUNIT_ASSERT( tmp.asFloat() == (float)1.2 );

    tmp = blieng::BliAny((double)1.2);
    CPPUNIT_ASSERT( tmp.asDouble() == 1.2 );

    tmp = blieng::BliAny(true);
    CPPUNIT_ASSERT( tmp.asBool() );

    tmp = blieng::BliAny(false);
    CPPUNIT_ASSERT( !tmp.asBool() );
}

void BliAnyTest::number()
{
    blieng::BliAny tmp;

    tmp = blieng::BliAny((int)42);
    CPPUNIT_ASSERT( tmp.asNumber() == 42 );

    tmp = blieng::BliAny((unsigned int)42);
    CPPUNIT_ASSERT( tmp.asNumber() == 42 );

    tmp = blieng::BliAny((unsigned long)42);
    CPPUNIT_ASSERT( tmp.asNumber() == 42 );

    tmp = blieng::BliAny((long)42);
    CPPUNIT_ASSERT( tmp.asNumber() == 42 );

    tmp = blieng::BliAny((float)42.1);
    CPPUNIT_ASSERT( tmp.asNumber() == 42 );

    tmp = blieng::BliAny((double)42.2);
    CPPUNIT_ASSERT( tmp.asNumber() == 42 );

    tmp = blieng::BliAny(std::string("Dummy"));
    CPPUNIT_ASSERT_THROW( tmp.asNumber(), std::string );
}
