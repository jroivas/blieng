#ifndef __TEST_MAPS_H
#define __TEST_MAPS_H

#include <cppunit/extensions/HelperMacros.h>
#include <boost/shared_ptr.hpp>

#include <blieng/blieng.h>

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

    boost::shared_ptr<blieng::BliengState> _state;
};

#endif
