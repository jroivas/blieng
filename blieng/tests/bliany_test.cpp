#include "bliany_test.h"
#include <bliany.h>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION( BliAnyTest );

void BliAnyTest::basic()
{
    blieng::BliAny tmp;

    CPPUNIT_ASSERT_THROW( tmp.asInt(), std::string );
    CPPUNIT_ASSERT_THROW( tmp.asString(), std::string );

#if 0
    tmp = blieng::BliAny(10);
    CPPUNIT_ASSERT( tmp.asInt() == 10 );

    tmp = blieng::BliAny(-10);
    CPPUNIT_ASSERT( tmp.asInt() == -10 );

    tmp = blieng::BliAny(123456789L);
    CPPUNIT_ASSERT( tmp.asUInt() == 123456789L );
#endif
}
