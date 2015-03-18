#ifndef __TEST_TOWN_H
#define __TEST_TOWN_H

#include <cppunit/extensions/HelperMacros.h>

class TownTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TownTest);
    CPPUNIT_TEST(basic);
    CPPUNIT_TEST(population);
    CPPUNIT_TEST(characterClass);
    CPPUNIT_TEST_SUITE_END();
public:
    void basic();
    void population();
    void characterClass();
};

#endif
