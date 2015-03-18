#ifndef __TEST_AUTO_VECTOR_H
#define __TEST_AUTO_VECTOR_H

#include <cppunit/extensions/HelperMacros.h>

class AutoVectorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( AutoVectorTest );
    CPPUNIT_TEST(push_back);
    CPPUNIT_TEST(pop_back);
    CPPUNIT_TEST(empty_clear_pop);
    CPPUNIT_TEST(access);
    CPPUNIT_TEST_SUITE_END();
public:
    void push_back();
    void pop_back();
    void empty_clear_pop();
    void access();
};

#endif
