#ifndef __TEST_CONFIGURE_H
#define __TEST_CONFIGURE_H

#include <cppunit/extensions/HelperMacros.h>

class ConfigureTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ConfigureTest);
    CPPUNIT_TEST(basic);
    CPPUNIT_TEST_SUITE_END();
public:
    void basic();
};

#endif
