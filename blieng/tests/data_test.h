#ifndef __TEST_DATA_H
#define __TEST_DATA_H

#include <cpptest.h>

class DataTest : public Test::Suite
{
public:
    DataTest() {
        TEST_ADD(DataTest::object);
    }
private:
    void object();
};

#endif
