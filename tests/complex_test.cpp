#include "complex_test.h"
#include <complex.h>
#include <utility>
#include <algorithm>

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

    CPPUNIT_ASSERT(c2 + 2 == Complex(7, 4));
}

void ComplexTest::minus()
{
    Complex c1(1, 1);
    Complex c2(5, 4);
    Complex c3(10, 7);

    CPPUNIT_ASSERT(c2.minus(c1) == Complex(4, 3));
    CPPUNIT_ASSERT(c1.minus(c2) == Complex(-4, -3));

    CPPUNIT_ASSERT(c3.minus(c2) == Complex(5, 3));
    CPPUNIT_ASSERT(c3 - c2 == Complex(5, 3));
    CPPUNIT_ASSERT(c2 - 2 == Complex(3, 4));

}

void ComplexTest::mul()
{
    Complex c1(2.5, 2);
    Complex c2(5, 4);
    Complex c3(10, 7);

    CPPUNIT_ASSERT(c2.mul(c1) == Complex(4.5, 20));
    CPPUNIT_ASSERT(c1.mul(c2) == Complex(4.5, 20));

    CPPUNIT_ASSERT(c3.mul(c2) == Complex(22, 75));
    CPPUNIT_ASSERT(c3 * c2 == Complex(22, 75));
    CPPUNIT_ASSERT(c2 * 2 == Complex(10, 8));
}

void ComplexTest::conjugate()
{
    Complex c1(2.5, 2);
    Complex c2(5, 4);
    Complex c3(10, 7);
    Complex c4(1, -2);

    CPPUNIT_ASSERT(c1.conjugate() == Complex(2.5, -2));
    CPPUNIT_ASSERT(c2.conjugate() == Complex(5, -4));
    CPPUNIT_ASSERT(c3.conjugate() == Complex(10, -7));
    CPPUNIT_ASSERT(c4.conjugate() == Complex(1, 2));
}

void ComplexTest::assign()
{
    Complex c1(1, 1);
    Complex c2(5, 4);
    Complex c3(10, 7);

    Complex b;
    b = c1;
    CPPUNIT_ASSERT(b == c1);
    CPPUNIT_ASSERT(b != c2);
    b = c2;
    CPPUNIT_ASSERT(b != c1);
    CPPUNIT_ASSERT(b == c2);
}
