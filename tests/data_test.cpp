#include "data_test.h"
#include "test_tools.h"

#include <gmock/gmock.h>
#include <data.h>
#include <boost/random/random_device.hpp>

#include <map>

#include "blieng/datafile.h"

CPPUNIT_TEST_SUITE_REGISTRATION( DataTest );

using blieng::Data;
using ::testing::_;
using ::testing::Return;
using ::testing::Invoke;

void DataTest::setUp()
{
}

void DataTest::object()
{
    boost::shared_ptr<DataMock> obj(new DataMock());

    CPPUNIT_ASSERT( obj != NULL );
    CPPUNIT_ASSERT( obj.get() != NULL );
}

void DataTest::readString()
{
    std::string origdata = "This file\nContains\nSome random strings\n\nEnd.";

    boost::shared_ptr<DataMock> obj(new DataMock());

    EXPECT_CALL(*(obj.get()), readString(_))
        .WillOnce(Return(origdata));

    std::string res = obj->readString("string_file");

    CPPUNIT_ASSERT( res != "" );
    CPPUNIT_ASSERT( res == origdata);
}

void DataTest::readLines()
{
    std::string origdata = "This file\nContains\nSome random strings\nEnd.";

    boost::shared_ptr<DataMock> obj(new DataMock());
    obj->setFakeData("data/string_file", origdata);
    obj->setFakeData("dummy/string_file", "dummy" + origdata);

    std::vector<std::string> res = obj->readLinesFromFile("string_file");

    CPPUNIT_ASSERT( !res.empty() );
    CPPUNIT_ASSERT( res.size() == 4 );
    CPPUNIT_ASSERT( res[0] == "This file");
    CPPUNIT_ASSERT( res[1] == "Contains");
    CPPUNIT_ASSERT( res[2] == "Some random strings");
    CPPUNIT_ASSERT( res[3] == "End.");
}

void DataTest::readLinesEmpty()
{
    std::string origdata = "";

    boost::shared_ptr<DataMock> obj(new DataMock());
    obj->setFakeData("data/string_file", origdata);

    std::vector<std::string> res = obj->readLinesFromFile("string_file");

    CPPUNIT_ASSERT( res.empty() );
}

void DataTest::readLinesNoFile()
{
    boost::shared_ptr<DataMock> obj(new DataMock());
    std::vector<std::string> res = obj->readLinesFromFile("no_string_file");

    CPPUNIT_ASSERT( res.empty() );
}

void DataTest::readData()
{
    std::string origdata;
    origdata.append("Hi", 2);
    origdata.append("\0", 1);
    origdata.append("Zero\42Data.");

    boost::shared_ptr<DataMock> obj(new DataMock());
    obj->setFakeData("data/datas", origdata);

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
}

void DataTest::readJson()
{
    boost::shared_ptr<DataMock> obj(new DataMock());
    obj->setFakeData("data/json1", "{\"aa\": \"b42\", \"second\": 12, \"third\": [1,2,3]}");

    blieng::BliengJson res = obj->readJson("json1");

    CPPUNIT_ASSERT( res.is_object() );

    CPPUNIT_ASSERT( res["aa"] != nullptr );
    CPPUNIT_ASSERT( res["aa"].is_string() );

    CPPUNIT_ASSERT( res["second"] != nullptr );
    CPPUNIT_ASSERT( res["second"].is_number() );

    CPPUNIT_ASSERT( res["third"] != nullptr );
    CPPUNIT_ASSERT( res["third"].is_array() );

    CPPUNIT_ASSERT( obj->isJsonKey(res, "second") );
    CPPUNIT_ASSERT( obj->getJsonValue(res, "second").is_number() );
}

void DataTest::fileExists()
{
    boost::shared_ptr<DataMock> obj(new DataMock());

    EXPECT_CALL(*(obj.get()), fileExists(_))
        .WillRepeatedly(Return(false));
    CPPUNIT_ASSERT( !obj->fileExists("dummy") );
    CPPUNIT_ASSERT( !obj->fileExists("dummy") );

    obj->setFakeData("data/dummy", "dum");

    EXPECT_CALL(*(obj.get()), fileExists(_))
        .WillOnce(Return(true));

    CPPUNIT_ASSERT( obj->fileExists("dummy") );
}
