#ifndef __TEST_BLIOBJECT_H
#define __TEST_BLIOBJECT_H

#include <cpptest.h>

class BliObjectTest : public Test::Suite
{
public:
    BliObjectTest() {
        TEST_ADD(BliObjectTest::values);
        TEST_ADD(BliObjectTest::getValues);
        TEST_ADD(BliObjectTest::toString);
        TEST_ADD(BliObjectTest::assign);
        TEST_ADD(BliObjectTest::random);
    }
private:
    void values();
    void getValues();
    void toString();
    void assign();
    void random();
};

#endif
