#ifndef __TEST_POINT_H
#define __TEST_POINT_H

#include <cppunit/extensions/HelperMacros.h>

class PointTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( PointTest );
    CPPUNIT_TEST( basic );
    CPPUNIT_TEST( set );
    CPPUNIT_TEST( length );
    CPPUNIT_TEST( traverse );
    CPPUNIT_TEST( assign );
    CPPUNIT_TEST( valid );
    CPPUNIT_TEST_SUITE_END();

public:
    void basic();
    void set();
    void length();
    void traverse();
    void assign();
    void valid();
};

#endif
