#ifndef __TEST_DATA_H
#define __TEST_DATA_H

#include <cppunit/extensions/HelperMacros.h>

class DataTest : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(DataTest);
    CPPUNIT_TEST(object);
    CPPUNIT_TEST_SUITE_END();
private:
    void object();
};

#endif