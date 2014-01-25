#ifndef __TEST_CARD_H
#define __TEST_CARD_H

#include <cppunit/extensions/HelperMacros.h>

class CardTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( CardTest );
    CPPUNIT_TEST( basic );
    CPPUNIT_TEST( combine );
    CPPUNIT_TEST( iter );
    CPPUNIT_TEST_SUITE_END();

public:
    void basic();
    void combine();
    void iter();
};

#endif
