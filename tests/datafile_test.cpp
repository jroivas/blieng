#include "datafile_test.h"
#include <datafile.h>
#include "test_tools.h"

CPPUNIT_TEST_SUITE_REGISTRATION( DataFileTest );

using blieng::DataFile;

void DataFileTest::tearDown()
{
    //mock_io_stop();
}

void DataFileTest::read()
{
    blieng::DataFile *obj = new blieng::DataFile("dummy.dat");
    //CPPUNIT_ASSERT( obj->read(NULL, 0) == false );
    CPPUNIT_ASSERT( obj->isValid() );
    delete obj;
}

void DataFileTest::write()
{
    mock_io_start();

    blieng::DataFile *obj = new blieng::DataFile("dummy.dat");
    CPPUNIT_ASSERT( obj != NULL );

    obj->addData("meaning", "42");
    obj->write(NULL, 0);

    delete obj;
    mock_io_stop();

    std::string dat = mock_get_data("dummy.dat");
    CPPUNIT_ASSERT( !dat.empty() );
}

void DataFileTest::write_read()
{
    mock_io_start();

    blieng::DataFile *obj = new blieng::DataFile("dummy.dat");
    CPPUNIT_ASSERT( obj != NULL );

    obj->addData("dum", "di_dam");
    obj->write(NULL, 0);

    delete obj;

    blieng::DataFile *obj2 = new blieng::DataFile("dummy.dat");
    obj2->read(NULL, 0);

    const char *data;
    unsigned int dlen = obj2->getData("dum", &data);
    CPPUNIT_ASSERT( dlen == 7);
    CPPUNIT_ASSERT( data[0] == 'd' );
    CPPUNIT_ASSERT( data[1] == 'i' );
    CPPUNIT_ASSERT( data[2] == '_' );
    CPPUNIT_ASSERT( data[3] == 'd' );
    CPPUNIT_ASSERT( data[4] == 'a' );
    CPPUNIT_ASSERT( data[5] == 'm' );
    CPPUNIT_ASSERT( data[6] == 0 );

    delete obj2;
    mock_io_stop();
}

void DataFileTest::write_read2()
{
    mock_io_start();

    blieng::DataFile *obj = new blieng::DataFile("dummy.dat");
    CPPUNIT_ASSERT( obj != NULL );

    char tmp[4];
    tmp[0] = 0x12;
    tmp[1] = 0x99;
    tmp[2] = 0x77;
    tmp[3] = 0x42;

    obj->addData("rand", tmp, 4);
    obj->write(NULL, 0);

    delete obj;

    blieng::DataFile *obj2 = new blieng::DataFile("dummy.dat");
    obj2->read(NULL, 0);

    const char *data;
    unsigned int dlen = obj2->getData("rand", &data);
    CPPUNIT_ASSERT( dlen == 4);
    CPPUNIT_ASSERT( data[0] == 0x12 );
    CPPUNIT_ASSERT( data[1] == (char)0x99 );
    CPPUNIT_ASSERT( data[2] == 0x77 );
    CPPUNIT_ASSERT( data[3] == 0x42 );

    delete obj2;
    mock_io_stop();
}

void DataFileTest::write_read_obfuscate()
{
    mock_io_start();

    blieng::DataFile *obj = new blieng::DataFile("dummy.dat");
    CPPUNIT_ASSERT( obj != NULL );

    char tmp[4];
    tmp[0] = 0x12;
    tmp[1] = 0x99;
    tmp[2] = 0x77;
    tmp[3] = 0x42;


    obj->addData("rand", tmp, 4);
    obj->write("42", 2);

    delete obj;

    blieng::DataFile *obj2 = new blieng::DataFile("dummy.dat");
    obj2->read("42", 2);

    const char *data;
    unsigned int dlen = obj2->getData("rand", &data);
    CPPUNIT_ASSERT( dlen == 4);
    CPPUNIT_ASSERT( data[0] == 0x12 );
    CPPUNIT_ASSERT( data[1] == (char)0x99 );
    CPPUNIT_ASSERT( data[2] == 0x77 );
    CPPUNIT_ASSERT( data[3] == 0x42 );

    delete obj2;
    mock_io_stop();
}

void DataFileTest::write_read_obfuscate2()
{
    mock_io_start();

    blieng::DataFile *obj = new blieng::DataFile("dummy.dat");
    CPPUNIT_ASSERT( obj != NULL );

    char tmp[4];
    tmp[0] = 0x12;
    tmp[1] = 0x99;
    tmp[2] = 0x77;
    tmp[3] = 0x42;

    obj->addData("rand", tmp, 4);
    obj->write("42", 2);

    delete obj;

    blieng::DataFile *obj2 = new blieng::DataFile("dummy.dat");
    // Decrypt with different key
    obj2->read("420", 3);

    const char *data;
    unsigned int dlen = obj2->getData("rand", &data);
    CPPUNIT_ASSERT( dlen == 4);
    CPPUNIT_ASSERT( data[0] != 0x12 );
    CPPUNIT_ASSERT( data[1] != (char)0x99 );
    CPPUNIT_ASSERT( data[2] != 0x77 );
    CPPUNIT_ASSERT( data[3] != 0x42 );

    delete obj2;
    mock_io_stop();
}

void DataFileTest::compress()
{
    mock_io_start();

    blieng::DataFile *obj = new blieng::DataFile("dummy.dat");
    CPPUNIT_ASSERT( obj != NULL );

    obj->enableCompression();

    char tmp[4];
    tmp[0] = 0x12;
    tmp[1] = 0x99;
    tmp[2] = 0x77;
    tmp[3] = 0x42;
    std::string tmp2("aaaaaaaaabbbbbbbbbbbbbbcccccccccc");

    obj->addData("rand", tmp, 4);
    obj->addData("aaa", tmp2);

    obj->write("42", 2);
    delete obj;

    blieng::DataFile *obj2 = new blieng::DataFile("dummy.dat");
    obj2->read("42", 2);

    const char *data;
    unsigned int dlen = obj2->getData("rand", &data);
    CPPUNIT_ASSERT( dlen == 4);
    CPPUNIT_ASSERT( data[0] == 0x12 );
    CPPUNIT_ASSERT( data[1] == (char)0x99 );
    CPPUNIT_ASSERT( data[2] == 0x77 );
    CPPUNIT_ASSERT( data[3] == 0x42 );

    dlen = obj2->getData("aaa", &data);
    CPPUNIT_ASSERT( dlen == tmp2.length() + 1);
    std::string tmp3(data, dlen - 1);
    CPPUNIT_ASSERT_EQUAL(tmp2, tmp3);


    delete obj2;
    mock_io_stop();
}
