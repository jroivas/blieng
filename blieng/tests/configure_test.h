#ifndef __TEST_CONFIGURE_H
#define __TEST_CONFIGURE_H

#include <cppunit/extensions/HelperMacros.h>

class ConfigureTest : public CppUnit::TestFixture
{
public:
    void setUp();
    void tearDown();
    CPPUNIT_TEST_SUITE(ConfigureTest);
    CPPUNIT_TEST(basic);
    CPPUNIT_TEST(json);
    CPPUNIT_TEST(optional);
    CPPUNIT_TEST_SUITE_END();
private:
    void basic();
    void json();
    void optional();
};

#endif
