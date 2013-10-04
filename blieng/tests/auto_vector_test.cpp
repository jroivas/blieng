#include "auto_vector_test.h"
#include "auto_vector.h"
#include <memory>

using std::unique_ptr;

CPPUNIT_TEST_SUITE_REGISTRATION( AutoVectorTest );

void AutoVectorTest::push_back()
{
    auto_vector<int> vect;

    unique_ptr<int> tmp1(new int);
    unique_ptr<int> tmp2(new int);
    unique_ptr<int> tmp3(new int);
    unique_ptr<int> tmp4(new int);

    *tmp1 = 10;
    *tmp2 = 42;
    *tmp3 = 99;
    *tmp4 = 999999;

    CPPUNIT_ASSERT( *tmp1 == 10 );
    CPPUNIT_ASSERT( *tmp2 == 42 );
    CPPUNIT_ASSERT( *tmp3 == 99 );
    CPPUNIT_ASSERT( *tmp4 == 999999 );

    vect.push_back(std::move(tmp1));

    CPPUNIT_ASSERT( vect.size() == 1 );
    CPPUNIT_ASSERT( tmp1 == nullptr );
    CPPUNIT_ASSERT( tmp2 != nullptr );

    vect.push_back(std::move(tmp2));
    CPPUNIT_ASSERT( vect.size() == 2 );
    CPPUNIT_ASSERT( tmp1 == nullptr );
    CPPUNIT_ASSERT( tmp2 == nullptr );
    CPPUNIT_ASSERT( tmp3 != nullptr );

    vect.push_back(std::move(tmp3));
    CPPUNIT_ASSERT( vect.size() == 3 );
    vect.push_back(std::move(tmp4));
    CPPUNIT_ASSERT( vect.size() == 4 );

    CPPUNIT_ASSERT( tmp1 == nullptr );
    CPPUNIT_ASSERT( tmp2 == nullptr );
    CPPUNIT_ASSERT( tmp3 == nullptr );
    CPPUNIT_ASSERT( tmp4 == nullptr );
}

void AutoVectorTest::pop_back()
{
    auto_vector<int> vect;

    unique_ptr<int> tmp1(new int);
    unique_ptr<int> tmp2(new int);
    unique_ptr<int> tmp3(new int);
    unique_ptr<int> tmp4(new int);

    *tmp1 = 10;
    *tmp2 = 42;
    *tmp3 = 99;
    *tmp4 = 999999;

    vect.push_back(std::move(tmp1));
    vect.push_back(std::move(tmp2));
    vect.push_back(std::move(tmp3));

    CPPUNIT_ASSERT( vect.size() == 3 );
    CPPUNIT_ASSERT( *vect[0] == 10 );
    CPPUNIT_ASSERT( *vect[1] == 42 );
    CPPUNIT_ASSERT( *vect[2] == 99 );
    CPPUNIT_ASSERT( vect.size() == 3 );

    CPPUNIT_ASSERT( tmp3 == nullptr );
    tmp3 = std::move(vect.pop_back());
    CPPUNIT_ASSERT( vect.size() == 2 );

    CPPUNIT_ASSERT( *tmp3 == 99 );
}
