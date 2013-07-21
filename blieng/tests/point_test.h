#ifndef __TEST_POINT_H
#define __TEST_POINT_H

#include <cpptest.h>

class PointTest : public Test::Suite
{
public:
    PointTest() {
        TEST_ADD(PointTest::basic);
        TEST_ADD(PointTest::set);
        TEST_ADD(PointTest::length);
        TEST_ADD(PointTest::traverse);
        TEST_ADD(PointTest::assign);
        TEST_ADD(PointTest::valid);
    }
private:
    void basic();
    void set();
    void length();
    void traverse();
    void assign();
    void valid();
};

#endif
