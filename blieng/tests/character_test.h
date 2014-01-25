#ifndef __TEST_CHARACTER_H
#define __TEST_CHARACTER_H

#include <cppunit/extensions/HelperMacros.h>

class CharacterTest : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(CharacterTest);
    CPPUNIT_TEST(alive_kill);
    CPPUNIT_TEST(items);
    CPPUNIT_TEST(assign);
    CPPUNIT_TEST(assign_object);
    CPPUNIT_TEST_SUITE_END();
private:
    void alive_kill();
    void items();
    void assign();
    void assign_object();
};

#endif
