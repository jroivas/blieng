#ifndef __TEST_DATAFILE_H
#define __TEST_DATAFILE_H

#include <cppunit/extensions/HelperMacros.h>

class DataFileTest : public CppUnit::TestFixture
{
public:
    CPPUNIT_TEST_SUITE(DataFileTest);
    CPPUNIT_TEST(read);
    CPPUNIT_TEST(write);
    CPPUNIT_TEST_SUITE_END();
private:
    void read();
    void write();
};

#endif
