#include "point_test.h"
#include <point.h>

CPPUNIT_TEST_SUITE_REGISTRATION( PointTest );

void PointTest::basic()
{
    blieng::Point p(1,1);
    blieng::Point d(0,0);
    blieng::Point p2(1,0);
    blieng::Point p3(1,1);

    CPPUNIT_ASSERT( p != d );
    CPPUNIT_ASSERT( p != p2 );
    CPPUNIT_ASSERT( p == p3 );
}

void PointTest::set()
{
    blieng::Point a(0,0);
    blieng::Point b(0,0);
    blieng::Point c(1,0);
    blieng::Point d(1,1);

    CPPUNIT_ASSERT( a == b );
    CPPUNIT_ASSERT( a != c );
    CPPUNIT_ASSERT( b != c );

    b.setX(1);
    CPPUNIT_ASSERT( a != b );
    CPPUNIT_ASSERT( a != c );
    CPPUNIT_ASSERT( b == c );

    b.setY(1);
    CPPUNIT_ASSERT( a != b );
    CPPUNIT_ASSERT( a != c );
    CPPUNIT_ASSERT( b != c );
    CPPUNIT_ASSERT( b == d );

    b.setY(2);
    CPPUNIT_ASSERT( a != b );
    CPPUNIT_ASSERT( a != c );
    CPPUNIT_ASSERT( b != c );
    CPPUNIT_ASSERT( b != d );

    a.setY(2);
    CPPUNIT_ASSERT( a != b );
    CPPUNIT_ASSERT( a != c );
    CPPUNIT_ASSERT( b != c );
    CPPUNIT_ASSERT( b != d );

    a.setX(1);
    CPPUNIT_ASSERT( a == b );
    CPPUNIT_ASSERT( a != c );
    CPPUNIT_ASSERT( b != c );
    CPPUNIT_ASSERT( b != d );
}

void PointTest::length()
{
    blieng::Point a(0,0);
    blieng::Point b(1,0);
    blieng::Point c(1,1);
    blieng::Point d(2,1);
    blieng::Point e(2,2);
    blieng::Point e_neg(-2,-2);

    CPPUNIT_ASSERT (a.length(a) == 0);
    CPPUNIT_ASSERT (a.length(b) == 1);
    CPPUNIT_ASSERT (a.length(c) == 2);
    CPPUNIT_ASSERT (a.length(d) == 3);
    CPPUNIT_ASSERT (a.length(e) == 4);
    CPPUNIT_ASSERT (a.length(e) == a.length(e_neg));
    CPPUNIT_ASSERT (a.length(e) == e.length(a));
}

void PointTest::traverse()
{
    blieng::Point a(0,0);
    blieng::Point b(100,200);
    blieng::Point c(300,2000);

    
    CPPUNIT_ASSERT (a.traverse(b, 0, 10) == a);
    CPPUNIT_ASSERT (a.traverse(b, -10, 10) == a);
    CPPUNIT_ASSERT (a.traverse(b, 10, 10) == b);
    CPPUNIT_ASSERT (a.traverse(b, 100, 10) == b);

    CPPUNIT_ASSERT (a.traverse(b, 5, 10).x == 50);
    CPPUNIT_ASSERT (a.traverse(b, 5, 10).y == 100);

    CPPUNIT_ASSERT (c.traverse(b, 5, 10) == c.traverse(b, 500, 1000));
}

void PointTest::assign()
{
    blieng::Point a(5,42);
    blieng::Point b(100,200);

    CPPUNIT_ASSERT( a.x == 5 );
    CPPUNIT_ASSERT( a.y == 42 );

    CPPUNIT_ASSERT( a != b );

    a = b;

    CPPUNIT_ASSERT( a == b );
    CPPUNIT_ASSERT( a.x == b.x );
    CPPUNIT_ASSERT( a.y == b.y );
    CPPUNIT_ASSERT( a.x == 100 );
    CPPUNIT_ASSERT( a.y == 200 );

    b.x = 99;
    CPPUNIT_ASSERT( a != b );
}

void PointTest::valid()
{
    blieng::Point a(5,42);
    blieng::Point b(100,200);
    blieng::Point c(false);

    CPPUNIT_ASSERT(a.isValid() == true);
    CPPUNIT_ASSERT(b.isValid() == true);
    CPPUNIT_ASSERT(c.isValid() != true);

    a.valid = false;
    c.valid = true;
    CPPUNIT_ASSERT(a.isValid() != true);
    CPPUNIT_ASSERT(c.isValid() == true);
}
