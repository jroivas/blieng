#include "card_test.h"
#include <card.h>
#include <string>
#include <memory>

CPPUNIT_TEST_SUITE_REGISTRATION( CardTest );

void CardTest::basic()
{
    blieng::Card tmp1;

    for (unsigned int i = 0; i < 100; ++i) {
        blieng::Card tmp2;
        CPPUNIT_ASSERT( tmp1.uuid() != tmp2.uuid() );
        CPPUNIT_ASSERT( tmp1.size() == tmp2.size() );
    }

}

void CardTest::combine()
{
    blieng::Card tmp1;
    std::unique_ptr<blieng::Card> tmp2(new blieng::Card());
    std::unique_ptr<blieng::Card> tmp3(new blieng::Card());
    std::unique_ptr<blieng::Card> tmp4(new blieng::Card());
    blieng::Card tmp5;

    CPPUNIT_ASSERT( tmp1.size() == tmp5.size() );
    CPPUNIT_ASSERT( tmp1.size() == 0 );

    tmp1.combine(std::move(tmp2));
    CPPUNIT_ASSERT( tmp1.size() == 1 );
    CPPUNIT_ASSERT( tmp1.size() != tmp5.size() );

    tmp5.combine(std::move(tmp4));
    CPPUNIT_ASSERT( tmp1.size() == tmp5.size() );

    tmp1.combine(std::move(tmp3));
    CPPUNIT_ASSERT( tmp1.size() == 2 );
    CPPUNIT_ASSERT( tmp1.size() != tmp5.size() );

    CPPUNIT_ASSERT( tmp1.remove(0) );
    CPPUNIT_ASSERT( tmp1.size() == 1 );
    CPPUNIT_ASSERT( tmp1.remove(0) );
    CPPUNIT_ASSERT( tmp1.size() == 0 );
    CPPUNIT_ASSERT( !tmp1.remove(0) );
    CPPUNIT_ASSERT( tmp1.size() == 0 );

    CPPUNIT_ASSERT( tmp1.size() != tmp5.size() );
    CPPUNIT_ASSERT( tmp5.remove(0) );
    CPPUNIT_ASSERT( tmp1.size() == tmp5.size() );
}

void CardTest::iter()
{

    blieng::Card tmp1;
    std::unique_ptr<blieng::Card> tmp2(new blieng::Card());
    std::unique_ptr<blieng::Card> tmp3(new blieng::Card());

    CPPUNIT_ASSERT( tmp1.size() == 0 );

    tmp1.combine(std::move(tmp2));
    CPPUNIT_ASSERT( tmp1.size() == 1 );

    tmp1.combine(std::move(tmp3));
    CPPUNIT_ASSERT( tmp1.size() == 2 );


    auto it = tmp1.begin();
    CPPUNIT_ASSERT( it != tmp1.end() );

    ++it;
    CPPUNIT_ASSERT( it != tmp1.end() );

    ++it;
    CPPUNIT_ASSERT( it == tmp1.end() );

    // Changing to non-const iterator
    it = tmp1.begin();

    ++it;
    CPPUNIT_ASSERT( it != tmp1.end() );
    CPPUNIT_ASSERT( tmp1.size() == 2 );

    tmp1.remove(it);
    CPPUNIT_ASSERT( tmp1.size() == 1 );

    it = tmp1.begin();
    tmp1.remove(it);
    CPPUNIT_ASSERT( tmp1.size() == 0 );
}
