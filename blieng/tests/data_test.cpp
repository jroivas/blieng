#include "data_test.h"
#include "test_tools.h"
#include <data.h>
#include <boost/random/random_device.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( DataTest );

using blieng::Data;

void DataTest::setUp()
{
    mock_add_folder("data");
}

void DataTest::object()
{
    mock_io_start();

    shared_ptr<blieng::Data> obj(new blieng::Data());

    CPPUNIT_ASSERT( obj != NULL );
    CPPUNIT_ASSERT( obj.get() != NULL );

    mock_io_stop();
}

void DataTest::readString()
{
    std::string origdata = "This file\nContains\nSome random strings\n\nEnd.";
    mock_set_file("data/string_file", origdata);

    mock_io_start();

    shared_ptr<blieng::Data> obj(new blieng::Data());
    std::string res = obj->readString("string_file");

    CPPUNIT_ASSERT( res != "" );
    CPPUNIT_ASSERT( res == origdata);

    mock_io_stop();
}

void DataTest::readLines()
{
    std::string origdata = "This file\nContains\nSome random strings\n\nEnd.";
    mock_set_file("data/string_file", origdata);

    mock_io_start();

    shared_ptr<blieng::Data> obj(new blieng::Data());
    std::vector<std::string> res = obj->readLinesFromFile("string_file");

    CPPUNIT_ASSERT( !res.empty() );
    CPPUNIT_ASSERT( res.size() == 4 );
    CPPUNIT_ASSERT( res[0] == "This file");
    CPPUNIT_ASSERT( res[1] == "Contains");
    CPPUNIT_ASSERT( res[2] == "Some random strings");
    CPPUNIT_ASSERT( res[3] == "End.");

    mock_io_stop();
}

void DataTest::readLinesEmpty()
{
    std::string origdata = "";
    mock_set_file("data/string_file", origdata);

    mock_io_start();

    shared_ptr<blieng::Data> obj(new blieng::Data());
    std::vector<std::string> res = obj->readLinesFromFile("string_file");

    CPPUNIT_ASSERT( res.empty() );

    mock_io_stop();
}

void DataTest::readLinesNoFile()
{
    mock_io_start();

    shared_ptr<blieng::Data> obj(new blieng::Data());
    std::vector<std::string> res = obj->readLinesFromFile("no_string_file");

    CPPUNIT_ASSERT( res.empty() );

    mock_io_stop();
}

void DataTest::readData()
{
    std::string origdata;
    origdata.append("Hi", 2);
    origdata.append("\0", 1);
    origdata.append("Zero\42Data.");
    mock_set_file("data/datas", origdata);

    mock_io_start();

    shared_ptr<blieng::Data> obj(new blieng::Data());
    const char *res = nullptr;
    unsigned int cnt = obj->readData("datas", &res);

    CPPUNIT_ASSERT( cnt > 0 );
    CPPUNIT_ASSERT( res != nullptr );
    CPPUNIT_ASSERT( cnt >= 9 );

    CPPUNIT_ASSERT( res[0] == 'H' );
    CPPUNIT_ASSERT( res[1] == 'i' );
    CPPUNIT_ASSERT( res[2] == 0 );
    CPPUNIT_ASSERT( res[3] == 'Z' );
    CPPUNIT_ASSERT( res[7] == '\42' );
    CPPUNIT_ASSERT( res[8] == 'D' );

    mock_io_stop();
}

void DataTest::readJson()
{
    mock_set_file("json1", "{\"aa\": \"b42\", \"second\": 12, \"third\": [1,2,3]}");

    mock_io_start();

    shared_ptr<blieng::Data> obj(new blieng::Data());

    json_value *res = obj->readJson("json1");

    CPPUNIT_ASSERT( res != nullptr );

    CPPUNIT_ASSERT( res->isObject() );
    //CPPUNIT_ASSERT( !res->empty() );

    CPPUNIT_ASSERT( res->isMember("aa") );
    CPPUNIT_ASSERT( (*res)["aa"].isString() );

    CPPUNIT_ASSERT( res->isMember("second") );
    CPPUNIT_ASSERT( (*res)["second"].isNumeric() );

    CPPUNIT_ASSERT( res->isMember("third") );
    CPPUNIT_ASSERT( (*res)["third"].isArray() );

    CPPUNIT_ASSERT( obj->isJsonKey(res, "second") );
    CPPUNIT_ASSERT( obj->getJsonValue(res, "second")->isNumeric() );

    mock_io_stop();
}

void DataTest::fileExists()
{
    mock_io_start();

    shared_ptr<blieng::Data> obj(new blieng::Data());

    CPPUNIT_ASSERT( !obj->fileExists("dummy") );
    CPPUNIT_ASSERT( !obj->fileExists("dummy") );

    mock_set_file("dummy", "dum");

    CPPUNIT_ASSERT( obj->fileExists("dummy") );

    //CPPUNIT_ASSERT( obj->findFile("dummy") == "" );

    mock_io_stop();
}
