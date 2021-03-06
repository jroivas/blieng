#ifndef __TEST_BLIOBJECT_H
#define __TEST_BLIOBJECT_H

#include <cppunit/extensions/HelperMacros.h>

class BliObjectTest : public CppUnit::TestFixture
{
public:
    void setUp();
    void tearDown();
    CPPUNIT_TEST_SUITE( BliObjectTest );
    CPPUNIT_TEST(values);
    CPPUNIT_TEST(getValues);
    CPPUNIT_TEST(getValuesLimits);
    CPPUNIT_TEST(toString);
    CPPUNIT_TEST(assign);
    CPPUNIT_TEST(random);
    CPPUNIT_TEST(incdec);
    CPPUNIT_TEST(bliany);
    CPPUNIT_TEST(percentageString);
    CPPUNIT_TEST(serialize);
    CPPUNIT_TEST(compress);
    CPPUNIT_TEST(uuid);
    CPPUNIT_TEST_SUITE_END();
private:
    void values();
    void getValues();
    void toString();
    void percentageString();
    void assign();
    void random();
    void incdec();
    void bliany();
    void getValuesLimits();
    void serialize();
    void compress();
    void uuid();
};

#endif
