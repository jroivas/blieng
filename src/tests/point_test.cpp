#include "point_test.h"
#include <point.h>

void PointTest::basic()
{
	blieng::Point p(1,1);
	blieng::Point d(0,0);
	blieng::Point p2(1,0);
	blieng::Point p3(1,1);

	TEST_ASSERT( p != d );
	TEST_ASSERT( p != p2 );
	TEST_ASSERT( p == p3 );
}

void PointTest::set()
{
	blieng::Point a(0,0);
	blieng::Point b(0,0);
	blieng::Point c(1,0);
	blieng::Point d(1,1);

	TEST_ASSERT( a == b );
	TEST_ASSERT( a != c );
	TEST_ASSERT( b != c );

	b.setX(1);
	TEST_ASSERT( a != b );
	TEST_ASSERT( a != c );
	TEST_ASSERT( b == c );

	b.setY(1);
	TEST_ASSERT( a != b );
	TEST_ASSERT( a != c );
	TEST_ASSERT( b != c );
	TEST_ASSERT( b == d );

	b.setY(2);
	TEST_ASSERT( a != b );
	TEST_ASSERT( a != c );
	TEST_ASSERT( b != c );
	TEST_ASSERT( b != d );

	a.setY(2);
	TEST_ASSERT( a != b );
	TEST_ASSERT( a != c );
	TEST_ASSERT( b != c );
	TEST_ASSERT( b != d );

	a.setX(1);
	TEST_ASSERT( a == b );
	TEST_ASSERT( a != c );
	TEST_ASSERT( b != c );
	TEST_ASSERT( b != d );
}

void PointTest::length()
{
	blieng::Point a(0,0);
	blieng::Point b(1,0);
	blieng::Point c(1,1);
	blieng::Point d(2,1);
	blieng::Point e(2,2);
	blieng::Point e_neg(-2,-2);

	TEST_ASSERT (a.length(a) == 0)
	TEST_ASSERT (a.length(b) == 1)
	TEST_ASSERT (a.length(c) == 2)
	TEST_ASSERT (a.length(d) == 3)
	TEST_ASSERT (a.length(e) == 4)
	TEST_ASSERT (a.length(e) == a.length(e_neg))
	TEST_ASSERT (a.length(e) == e.length(a))
}

void PointTest::traverse()
{
	blieng::Point a(0,0);
	blieng::Point b(100,200);
	blieng::Point c(300,2000);

	
	TEST_ASSERT (a.traverse(b, 0, 10) == a)
	TEST_ASSERT (a.traverse(b, -10, 10) == a)
	TEST_ASSERT (a.traverse(b, 10, 10) == b)
	TEST_ASSERT (a.traverse(b, 100, 10) == b)

	TEST_ASSERT (a.traverse(b, 5, 10).x == 50)
	TEST_ASSERT (a.traverse(b, 5, 10).y == 100)

	TEST_ASSERT (c.traverse(b, 5, 10) == c.traverse(b, 500, 1000))
}
