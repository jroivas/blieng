#ifndef __TEST_BLIANY_H
#define __TEST_BLIANY_H

#include <cppunit/extensions/HelperMacros.h>

class BliAnyTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( BliAnyTest );
    CPPUNIT_TEST( basic );
    CPPUNIT_TEST( number );
    CPPUNIT_TEST_SUITE_END();

public:
    void basic();
    void number();
};

#endif
