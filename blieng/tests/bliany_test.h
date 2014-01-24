#ifndef __TEST_POINT_H
#define __TEST_POINT_H

#include <cppunit/extensions/HelperMacros.h>

class BliAnyTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( BliAnyTest );
    CPPUNIT_TEST( basic );
    CPPUNIT_TEST_SUITE_END();

public:
    void basic();
};

#endif
