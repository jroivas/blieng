#ifndef __TEST_PATH_H
#define __TEST_PATH_H

#include <cppunit/extensions/HelperMacros.h>

class PathTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( PathTest );
    CPPUNIT_TEST( basic );
    CPPUNIT_TEST( append );
    CPPUNIT_TEST( reverse );
    CPPUNIT_TEST( copy );
    CPPUNIT_TEST( take );
    CPPUNIT_TEST( length );
    CPPUNIT_TEST( update );
    CPPUNIT_TEST( combine );
    CPPUNIT_TEST_SUITE_END();
public:
    void basic();
    void append();
    void reverse();
    void copy();
    void take();
    void length();
    void update();
    void combine();
};

#endif
