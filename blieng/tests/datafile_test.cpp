#include "datafile_test.h"
#include <datafile.h>
#include "test_tools.h"

CPPUNIT_TEST_SUITE_REGISTRATION( DataFileTest );

using blieng::DataFile;

void DataFileTest::read()
{
    mock_io_start();

    blieng::DataFile *obj = new blieng::DataFile("dummy.dat");
    //CPPUNIT_ASSERT( obj->read(NULL, 0) == false );
    //delete obj;

    mock_io_stop();
}

void DataFileTest::write()
{
    mock_io_start();

    blieng::DataFile *obj = new blieng::DataFile("dummy.dat");
    CPPUNIT_ASSERT( obj != NULL );

    obj->addData("meaning", "42");
    obj->write(NULL, 0);

    //delete obj;
    mock_io_stop();

/*
    std::vector<std::string> dat = mock_list_files();
    CPPUNIT_ASSERT( dat.size() != 0 );
    std::cout << dat[0] << "\n";
*/
    
    std::string dat = mock_get_data("dummy.dat");
    std::cout << dat.size() << "\n";
    unsigned int a = dat.size();
    while (a > 0) {
        std::cout << (int)dat[dat.size() - a] << " ";
        --a;
    }
    std::cout << "\n";
/*
*/
}
