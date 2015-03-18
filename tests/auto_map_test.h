#ifndef __TEST_AUTO_MAP_H
#define __TEST_AUTO_MAP_H

#include <cppunit/extensions/HelperMacros.h>

class AutoMapTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( AutoMapTest );
    CPPUNIT_TEST(access);
    CPPUNIT_TEST(iter);
    CPPUNIT_TEST_SUITE_END();
public:
    void access();
    void iter();
};

#endif
