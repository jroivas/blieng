#ifndef __TEST_DATAFILE_H
#define __TEST_DATAFILE_H

#include <cppunit/extensions/HelperMacros.h>

class DataFileTest : public CppUnit::TestFixture
{
public:
    void tearDown();
    CPPUNIT_TEST_SUITE(DataFileTest);
    CPPUNIT_TEST(read);
    CPPUNIT_TEST(write);
    CPPUNIT_TEST(write_read);
    CPPUNIT_TEST(write_read2);
    CPPUNIT_TEST(write_read_obfuscate);
    CPPUNIT_TEST(write_read_obfuscate2);
    CPPUNIT_TEST(compress);
    CPPUNIT_TEST_SUITE_END();
private:
    void read();
    void write();
    void write_read();
    void write_read2();
    void write_read_obfuscate();
    void write_read_obfuscate2();
    void compress();
};

#endif
