#ifndef __TEST_MAPS_H
#define __TEST_MAPS_H

#include <cppunit/extensions/HelperMacros.h>

class MapsTest : public CppUnit::TestFixture
{
public:
    void setUp();
    void tearDown();
    CPPUNIT_TEST_SUITE(MapsTest);
    CPPUNIT_TEST(basic);
    CPPUNIT_TEST(paths);
    CPPUNIT_TEST_SUITE_END();
private:
    void basic();
    void paths();
};

#endif
