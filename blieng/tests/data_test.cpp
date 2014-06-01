#include "data_test.h"

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

class FakeDataFile : public blieng::DataFile
{
public:
    const DataFileObject *getObject(const std::string &name) {
        auto item = _fake_data.find(name);
        if (item != _fake_data.end()) {
            return new DataFileObject(
                item->second.c_str(),
                item->second.length());
        }
        return new DataFileObject();
    }

    void setFakeData(const std::string &_name, const std::string &_data) {
        _fake_data[_name] = _data;
    }

private:
    std::map<std::string, std::string> _fake_data;
};

class DataMock : public blieng::Data
{
public:
    MOCK_CONST_METHOD1(readString, std::string(const std::string));
    MOCK_CONST_METHOD1(fileExists, bool(const std::string));

    void setFakeData(const std::string &_name, const std::string &_data) {
        FakeDataFile *tmp = nullptr;
        if (m_datafile == nullptr) {
            tmp = new FakeDataFile();
        } else {
            tmp = dynamic_cast<FakeDataFile*>(m_datafile);
        }
        tmp->setFakeData(_name, _data);

        m_datafile = tmp;
    }
private:
    FakeDataFile _fake_data_file;
};

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
