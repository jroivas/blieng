#include "auto_map_test.h"
#include "auto_map.h"
#include <memory>

using std::unique_ptr;

CPPUNIT_TEST_SUITE_REGISTRATION( AutoMapTest );

void AutoMapTest::access()
{
    auto_map<std::string, int> data;

    unique_ptr<int> tmp1(new int);
    unique_ptr<int> tmp2(new int);
    unique_ptr<int> tmp3(new int);
    unique_ptr<int> tmp4(new int);

    *tmp1 = 10;
    *tmp2 = 42;
    *tmp3 = 99;
    *tmp4 = 999999;

    CPPUNIT_ASSERT( data.empty() );
    CPPUNIT_ASSERT( data.size() == 0 );

    data["ten"] = std::move(tmp1);

    CPPUNIT_ASSERT( !data.empty() );
    CPPUNIT_ASSERT( data.size() == 1 );
    CPPUNIT_ASSERT( tmp1 == nullptr );

    data["max"] = std::move(tmp4);

    CPPUNIT_ASSERT( data.size() == 2 );
    CPPUNIT_ASSERT( tmp4 == nullptr );

    data["meaning"] = std::move(tmp2);
    CPPUNIT_ASSERT( data.size() == 3 );

    data["mid"] = std::move(tmp3);
    CPPUNIT_ASSERT( data.size() == 4 );

    CPPUNIT_ASSERT( *data["mid"] == 99 );

    CPPUNIT_ASSERT( data.size() == 4 );

    *data["mid"] = 101;

    CPPUNIT_ASSERT( data.size() == 4 );
    CPPUNIT_ASSERT( *data["mid"] != 99 );
    CPPUNIT_ASSERT( *data["mid"] == 101 );
}

void AutoMapTest::iter()
{
    auto_map<std::string, int> data;

    unique_ptr<int> tmp1(new int);
    unique_ptr<int> tmp2(new int);
    unique_ptr<int> tmp3(new int);

    *tmp1 = 10;
    *tmp2 = 42;
    *tmp3 = 99;

    data["ten"] = std::move(tmp1);
    data["mid"] = std::move(tmp3);
    data["meaning"] = std::move(tmp2);

    auto_map<std::string, int>::iterator it = data.begin();
    CPPUNIT_ASSERT( it == data.begin() );
    while (it != data.end()) {
        ++it;
    }
    CPPUNIT_ASSERT( it == data.end() );

    it = data.begin();

    CPPUNIT_ASSERT( (*it)->key == "ten" );
    ++it;
    CPPUNIT_ASSERT( (*it)->key == "mid" );
    ++it;
    CPPUNIT_ASSERT( (*it)->key == "meaning" );
    tmp1 = (*it)->getValue();

    CPPUNIT_ASSERT( *tmp1 == 42 );
    CPPUNIT_ASSERT( data.size() == 3 );

    CPPUNIT_ASSERT( tmp2 == nullptr );
    tmp2 = data.get("meaning");
    CPPUNIT_ASSERT( !tmp2.get() );
    CPPUNIT_ASSERT( tmp2 == nullptr );
    CPPUNIT_ASSERT( data.size() == 2 );

    tmp2 = data.get("mid");
    CPPUNIT_ASSERT( tmp2 != nullptr );
    CPPUNIT_ASSERT( *tmp2 == 99 );
    CPPUNIT_ASSERT( data.size() == 1 );
}
