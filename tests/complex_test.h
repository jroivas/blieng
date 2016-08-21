#ifndef __TEST_COMPLEX_H
#define __TEST_COMPLEX_H

#include <cppunit/extensions/HelperMacros.h>

class ComplexTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( ComplexTest );
    CPPUNIT_TEST( basic );
    CPPUNIT_TEST( plus );
    CPPUNIT_TEST( minus );
    CPPUNIT_TEST( mul );
    CPPUNIT_TEST( conjugate );
    CPPUNIT_TEST( assign );
    CPPUNIT_TEST( calc );
    CPPUNIT_TEST_SUITE_END();
public:
    void basic();
    void plus();
    void minus();
    void mul();
    void conjugate();
    void assign();
    void calc();
};

#endif
