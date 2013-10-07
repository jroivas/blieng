#ifndef __TEST_ITEM_H
#define __TEST_ITEM_H

#include <cppunit/extensions/HelperMacros.h>

class ItemTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ItemTest);
    CPPUNIT_TEST(basic);
    CPPUNIT_TEST_SUITE_END();
public:
    void basic();
};

#endif
