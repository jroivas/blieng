#ifndef __TEST_BLIOBJECT_H
#define __TEST_BLIOBJECT_H

#include <cppunit/extensions/HelperMacros.h>

class BliObjectTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( BliObjectTest );
    CPPUNIT_TEST(values);
    CPPUNIT_TEST(getValues);
    CPPUNIT_TEST(toString);
    CPPUNIT_TEST(assign);
    CPPUNIT_TEST(random);
    CPPUNIT_TEST(incdec);
    CPPUNIT_TEST_SUITE_END();
public:
    void values();
    void getValues();
    void toString();
    void assign();
    void random();
    void incdec();
};

#endif
