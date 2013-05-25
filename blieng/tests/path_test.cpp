#include "path_test.h"
#include <path.h>
#include <point.h>

void PathTest::basic()
{
	blieng::Path a;
	blieng::Path b;

	TEST_ASSERT( a.isValid() == false );
	TEST_ASSERT( b.isValid() == false );
	TEST_ASSERT( a == b);

	a.addPoint(blieng::Point(1,2));
	TEST_ASSERT( a.isValid() == true );
	TEST_ASSERT( b.isValid() == false );

	TEST_ASSERT( a != b);
}

void PathTest::append()
{
	blieng::Path a;
	blieng::Path b;

	a.addPoint(blieng::Point(1,2));

	TEST_ASSERT(a.size() == 1);

	b.addPoint(blieng::Point(5,6));
	b.addPoint(blieng::Point(9,10));

	TEST_ASSERT(a.size() == 1);
	TEST_ASSERT(b.size() == 2);

	a.append(b);

	TEST_ASSERT(a.size() == 3);

	TEST_ASSERT(a.takeFirst().x == 1);
	TEST_ASSERT(a.size() == 2);
	TEST_ASSERT(a.takeFirst().x == 5);
	TEST_ASSERT(a.size() == 1);
	TEST_ASSERT(a.takeFirst().x == 9);
	TEST_ASSERT(a.size() == 0);
	TEST_ASSERT(b.size() == 2);
}

void PathTest::reverse()
{
	blieng::Path a;
	blieng::Path b;

	a.addPoint(blieng::Point(1,2));
	a.addPoint(blieng::Point(3,4));
	a.addPoint(blieng::Point(5,6));

	TEST_ASSERT(a.getStart().x == 1);
	TEST_ASSERT(a.getStart().y == 2);
	TEST_ASSERT(a.getEnd().x == 5);
	TEST_ASSERT(a.getEnd().y == 6);
	TEST_ASSERT(a != b);

	b = a.reversed();

	TEST_ASSERT(a.getStart().x == 1);
	TEST_ASSERT(a.getStart().y == 2);
	TEST_ASSERT(a.getEnd().x == 5);
	TEST_ASSERT(a.getEnd().y == 6);

	TEST_ASSERT(b.getStart().x == 5);
	TEST_ASSERT(b.getStart().y == 6);
	TEST_ASSERT(b.getEnd().x == 1);
	TEST_ASSERT(b.getEnd().y == 2);

	TEST_ASSERT(a != b);

	a.reverse();
	TEST_ASSERT(a == b);
}

void PathTest::copy()
{
	blieng::Path a;
	blieng::Path b;

	a.addPoint(blieng::Point(1,2));
	a.addPoint(blieng::Point(3,4));
	a.addPoint(blieng::Point(5,6));
	TEST_ASSERT(a != b);
	

	b = a.copy();
	TEST_ASSERT(a == b);

	a.addPoint(blieng::Point(7,8));
	TEST_ASSERT(a != b);

	b = a;
	TEST_ASSERT(a == b);

	a.addPoint(blieng::Point(9,10));
	TEST_ASSERT(a != b);
}

void PathTest::take()
{
	blieng::Path a;
	blieng::Path b;

	a.addPoint(blieng::Point(1,2));
	a.addPoint(blieng::Point(3,4));
	a.addPoint(blieng::Point(5,6));

	TEST_ASSERT( a.getStart().x == 1);
	TEST_ASSERT( a.getStart().y == 2);
	TEST_ASSERT( a.getStart().x == 1);
	TEST_ASSERT( a.getStart().y == 2);
	TEST_ASSERT( a.getStart().isValid() == true);

	blieng::Point tmp = a.takeFirst();
	TEST_ASSERT( a.getStart().x == 3);
	TEST_ASSERT( a.getStart().y == 4);
	TEST_ASSERT( tmp.x == 1);
	TEST_ASSERT( tmp.y == 2);

	TEST_ASSERT( a.getEnd().x == 5);
	TEST_ASSERT( a.getEnd().y == 6);

	tmp = a.takeLast();
	TEST_ASSERT( tmp.x == 5);
	TEST_ASSERT( tmp.y == 6);
	TEST_ASSERT( a.getEnd().x == 3);
	TEST_ASSERT( a.getEnd().y == 4);
	TEST_ASSERT( a.getStart().x == 3);
	TEST_ASSERT( a.getStart().y == 4);

	TEST_ASSERT( a.getStart().isValid() == true);

	tmp = a.takeFirst();
	TEST_ASSERT( tmp.x == 3);
	TEST_ASSERT( tmp.y == 4);

	TEST_ASSERT( a.getStart().isValid() == false);
}

void PathTest::length()
{
	blieng::Path a;

	a.addPoint(blieng::Point(1,2));
	TEST_ASSERT(a.length() == 0);

	a.addPoint(blieng::Point(3,4));
	TEST_ASSERT(a.length() == 4);

	a.addPoint(blieng::Point(5,6));
	TEST_ASSERT(a.length() == 8);
}
