#ifndef __TEST_EVENTLOG_H
#define __TEST_EVENTLOG_H

#include <cppunit/extensions/HelperMacros.h>

class EventLogTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(EventLogTest);
    CPPUNIT_TEST(basic);
    CPPUNIT_TEST(events);
    CPPUNIT_TEST_SUITE_END();
public:
    void basic();
    void events();
};

#endif
