#ifndef __TEST_LOGGING_H
#define __TEST_LOGGING_H

#include <cppunit/extensions/HelperMacros.h>

class LoggingTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(LoggingTest);
    CPPUNIT_TEST(basic);
    CPPUNIT_TEST(levels);
    CPPUNIT_TEST_SUITE_END();
public:
    void basic();
    void levels();
};

#endif
