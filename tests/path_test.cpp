#include "path_test.h"
#include <path.h>
#include <point.h>

CPPUNIT_TEST_SUITE_REGISTRATION( PathTest );

void PathTest::basic()
{
    blieng::Path a;
    blieng::Path b;

    CPPUNIT_ASSERT( a.isValid() == false );
    CPPUNIT_ASSERT( b.isValid() == false );
    CPPUNIT_ASSERT( a == b);

    a.addPoint(blieng::Point(1,2));
    CPPUNIT_ASSERT( a.isValid() == true );
    CPPUNIT_ASSERT( b.isValid() == false );

    CPPUNIT_ASSERT( a != b);
}

void PathTest::append()
{
    blieng::Path a;
    blieng::Path b;

    a.addPoint(blieng::Point(1,2));

    CPPUNIT_ASSERT(a.size() == 1);

    b.addPoint(blieng::Point(5,6));
    b.addPoint(blieng::Point(9,10));

    CPPUNIT_ASSERT(a.size() == 1);
    CPPUNIT_ASSERT(b.size() == 2);

    a.append(b);

    CPPUNIT_ASSERT(a.size() == 3);

    CPPUNIT_ASSERT(a.takeFirst().x == 1);
    CPPUNIT_ASSERT(a.size() == 2);
    CPPUNIT_ASSERT(a.takeFirst().x == 5);
    CPPUNIT_ASSERT(a.size() == 1);
    CPPUNIT_ASSERT(a.takeFirst().x == 9);
    CPPUNIT_ASSERT(a.size() == 0);
    CPPUNIT_ASSERT(b.size() == 2);
}

void PathTest::combine()
{
    blieng::Path a;
    blieng::Path b;

    a.addPoint(blieng::Point(1,2));

    CPPUNIT_ASSERT(a.size() == 1);

    b.addPoint(blieng::Point(5,6));
    b.addPoint(blieng::Point(9,10));

    CPPUNIT_ASSERT(a.size() == 1);
    CPPUNIT_ASSERT(b.size() == 2);

    blieng::Path c = a.combine(b);

    CPPUNIT_ASSERT(a.size() == 1);
    CPPUNIT_ASSERT(c.size() == 3);

    CPPUNIT_ASSERT(c.takeFirst().x == 1);
    CPPUNIT_ASSERT(c.size() == 2);
    CPPUNIT_ASSERT(c.takeFirst().x == 5);
    CPPUNIT_ASSERT(c.size() == 1);
    CPPUNIT_ASSERT(c.takeFirst().x == 9);
    CPPUNIT_ASSERT(a.size() == 1);
    CPPUNIT_ASSERT(b.size() == 2);
    CPPUNIT_ASSERT(c.size() == 0);
}

void PathTest::reverse()
{
    blieng::Path a;
    blieng::Path b;

    a.addPoint(blieng::Point(1,2));
    a.addPoint(blieng::Point(3,4));
    a.addPoint(blieng::Point(5,6));

    CPPUNIT_ASSERT(a.getStart().x == 1);
    CPPUNIT_ASSERT(a.getStart().y == 2);
    CPPUNIT_ASSERT(a.getEnd().x == 5);
    CPPUNIT_ASSERT(a.getEnd().y == 6);
    CPPUNIT_ASSERT(a != b);

    b = a.reversed();

    CPPUNIT_ASSERT(a.getStart().x == 1);
    CPPUNIT_ASSERT(a.getStart().y == 2);
    CPPUNIT_ASSERT(a.getEnd().x == 5);
    CPPUNIT_ASSERT(a.getEnd().y == 6);

    CPPUNIT_ASSERT(b.getStart().x == 5);
    CPPUNIT_ASSERT(b.getStart().y == 6);
    CPPUNIT_ASSERT(b.getEnd().x == 1);
    CPPUNIT_ASSERT(b.getEnd().y == 2);

    CPPUNIT_ASSERT(a != b);

    a.reverse();
    CPPUNIT_ASSERT(a == b);
}

void PathTest::copy()
{
    blieng::Path a;
    blieng::Path b;

    a.addPoint(blieng::Point(1,2));
    a.addPoint(blieng::Point(3,4));
    a.addPoint(blieng::Point(5,6));
    CPPUNIT_ASSERT(a != b);

    b = a.copy();
    CPPUNIT_ASSERT(a == b);

    a.addPoint(blieng::Point(7,8));
    CPPUNIT_ASSERT(a != b);

    b = a;
    CPPUNIT_ASSERT(a == b);

    a.addPoint(blieng::Point(9,10));
    CPPUNIT_ASSERT(a != b);
}

void PathTest::take()
{
    blieng::Path a;
    blieng::Path b;

    a.addPoint(blieng::Point(1,2));
    a.addPoint(blieng::Point(3,4));
    a.addPoint(blieng::Point(5,6));

    CPPUNIT_ASSERT( a.getStart().x == 1);
    CPPUNIT_ASSERT( a.getStart().y == 2);
    CPPUNIT_ASSERT( a.getStart().x == 1);
    CPPUNIT_ASSERT( a.getStart().y == 2);
    CPPUNIT_ASSERT( a.getStart().isValid() == true);

    blieng::Point tmp = a.takeFirst();
    CPPUNIT_ASSERT( a.getStart().x == 3);
    CPPUNIT_ASSERT( a.getStart().y == 4);
    CPPUNIT_ASSERT( tmp.x == 1);
    CPPUNIT_ASSERT( tmp.y == 2);

    CPPUNIT_ASSERT( a.getEnd().x == 5);
    CPPUNIT_ASSERT( a.getEnd().y == 6);

    tmp = a.takeLast();
    CPPUNIT_ASSERT( tmp.x == 5);
    CPPUNIT_ASSERT( tmp.y == 6);
    CPPUNIT_ASSERT( a.getEnd().x == 3);
    CPPUNIT_ASSERT( a.getEnd().y == 4);
    CPPUNIT_ASSERT( a.getStart().x == 3);
    CPPUNIT_ASSERT( a.getStart().y == 4);

    CPPUNIT_ASSERT( a.getStart().isValid() == true);

    tmp = a.takeFirst();
    CPPUNIT_ASSERT( tmp.x == 3);
    CPPUNIT_ASSERT( tmp.y == 4);

    CPPUNIT_ASSERT( a.getStart().isValid() == false);
}

void PathTest::length()
{
    blieng::Path a;

    a.addPoint(blieng::Point(1,2));
    CPPUNIT_ASSERT(a.length() == 0);

    a.addPoint(blieng::Point(3,4));
    CPPUNIT_ASSERT(a.length() == 4);

    a.addPoint(blieng::Point(5,6));
    CPPUNIT_ASSERT(a.length() == 8);
}

void PathTest::update()
{
    blieng::Path a;

    a.addPoint(blieng::Point(1,2));
    a.addPoint(blieng::Point(3,4));
    a.addPoint(blieng::Point(5,6));

    CPPUNIT_ASSERT(a.size() == 3);

    CPPUNIT_ASSERT( a.getPoints()[0].x == 1 );
    CPPUNIT_ASSERT( a.getPoints()[0].y == 2 );

    CPPUNIT_ASSERT( a.getPoints()[1].x == 3 );
    CPPUNIT_ASSERT( a.getPoints()[1].y == 4 );

    CPPUNIT_ASSERT( a.getPoints()[2].x == 5 );
    CPPUNIT_ASSERT( a.getPoints()[2].y == 6 );

    a.updatePointAt(1, blieng::Point(8,0));

    CPPUNIT_ASSERT(a.size() == 3);

    CPPUNIT_ASSERT( a.getPoints()[0].x == 1 );
    CPPUNIT_ASSERT( a.getPoints()[0].y == 2 );

    CPPUNIT_ASSERT( a.getPoints()[1].x == 8 );
    CPPUNIT_ASSERT( a.getPoints()[1].y == 0 );

    CPPUNIT_ASSERT( a.getPoints()[2].x == 5 );
    CPPUNIT_ASSERT( a.getPoints()[2].y == 6 );

}

void PathTest::area()
{
    blieng::Path a;

    a.addPoint(blieng::Point(1,2));
    a.addPoint(blieng::Point(3,4));
    a.addPoint(blieng::Point(5,6));
    a.addPoint(blieng::Point(8,4));
    a.addPoint(blieng::Point(8,2));
    a.addPoint(blieng::Point(1,2));

    CPPUNIT_ASSERT(a.area() == 17);

    a = blieng::Path();

    a.addPoint(blieng::Point(1,2));
    a.addPoint(blieng::Point(2,6));
    a.addPoint(blieng::Point(5,6));
    a.addPoint(blieng::Point(8,4));
    a.addPoint(blieng::Point(5,1));
    a.addPoint(blieng::Point(1,2));

    CPPUNIT_ASSERT(a.area() == 23.5);
}

void PathTest::areaGeo()
{
    blieng::Path a;

    a.addPoint(blieng::Point(1,2));
    a.addPoint(blieng::Point(3,4));
    a.addPoint(blieng::Point(5,6));
    a.addPoint(blieng::Point(8,4));
    a.addPoint(blieng::Point(8,2));
    a.addPoint(blieng::Point(1,2));

    CPPUNIT_ASSERT(int(a.areaGeo() / 1000.0 / 1000.0) == 210664);

    a = blieng::Path();

    a.addPoint(blieng::Point(60.0, 60.0));
    a.addPoint(blieng::Point(60.1, 60.0));
    a.addPoint(blieng::Point(60.1, 60.1));
    a.addPoint(blieng::Point(60.0, 60.1));
    a.addPoint(blieng::Point(60.0, 60.0));

    CPPUNIT_ASSERT(int(a.areaGeo() / 1000.0) == 123920);
}
