#pragma once

#include <cppunit/extensions/HelperMacros.h>

class BliPhysicsTest : public CppUnit::TestFixture
{
public:
    void setUp();
    void tearDown();
    CPPUNIT_TEST_SUITE( BliPhysicsTest );
    CPPUNIT_TEST(world);
    CPPUNIT_TEST(dynamicbody);
    CPPUNIT_TEST(staticbody);
    CPPUNIT_TEST(simulation);
    CPPUNIT_TEST(simulation2);
    CPPUNIT_TEST_SUITE_END();
private:
    void world();
    void dynamicbody();
    void staticbody();
    void simulation();
    void simulation2();
};
