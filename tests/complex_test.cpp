#include "complex_test.h"
#include <complex.h>

CPPUNIT_TEST_SUITE_REGISTRATION( ComplexTest );

using blieng::Complex;

void ComplexTest::basic()
{
    Complex c1(1, 1);
    Complex c2(5, 4);
    Complex c3(5, 4);

    CPPUNIT_ASSERT(c1 != c2);
    CPPUNIT_ASSERT(c3 == c2);

    CPPUNIT_ASSERT(c1 == Complex(1,1));
    CPPUNIT_ASSERT(c1 != Complex(1,2));
}

void ComplexTest::plus()
{
    Complex c1(1, 1);
    Complex c2(5, 4);
    Complex c3(10, 7);

    Complex res = c1.plus(c2);
    CPPUNIT_ASSERT(res == Complex(6, 5));

    CPPUNIT_ASSERT(c3.plus(c2) == Complex(15, 11));
    CPPUNIT_ASSERT(c3+c2 == Complex(15, 11));
}

void ComplexTest::minus()
{
    Complex c1(1, 1);
    Complex c2(5, 4);
    Complex c3(10, 7);

    CPPUNIT_ASSERT(c2.minus(c1) == Complex(4, 3));
    CPPUNIT_ASSERT(c1.minus(c2) == Complex(-4, -3));

    CPPUNIT_ASSERT(c3.minus(c2) == Complex(5, 3));
    CPPUNIT_ASSERT(c3-c2 == Complex(5, 3));
}

void ComplexTest::mul()
{
    Complex c1(2.5, 2);
    Complex c2(5, 4);
    Complex c3(10, 7);

    CPPUNIT_ASSERT(c2.mul(c1) == Complex(4, 3));
    CPPUNIT_ASSERT(c1.mul(c2) == Complex(-4, -3));

    CPPUNIT_ASSERT(c3.mul(c2) == Complex(5, 3));
}