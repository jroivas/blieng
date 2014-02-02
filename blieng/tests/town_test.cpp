#include "town_test.h"
#include "test_tools.h"
#include <bliobject.h>
#include <character.h>
#include <town.h>
#include <memory>

CPPUNIT_TEST_SUITE_REGISTRATION( TownTest );

void TownTest::basic()
{
    std::unique_ptr<blieng::Town> town(new blieng::Town());

    CPPUNIT_ASSERT_EQUAL( (unsigned int)0, town->getPopulation() );

    std::string town_name = "Testing Town 42";
    CPPUNIT_ASSERT_ASSERTION_FAIL( CPPUNIT_ASSERT_EQUAL(town_name , town->getName() ) );

    town->setName(town_name);
    CPPUNIT_ASSERT_EQUAL(town_name , town->getName() );


    CPPUNIT_ASSERT_EQUAL( (unsigned int)0, town->getSize() );

    town->setSize(42);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)42, town->getSize() );


    CPPUNIT_ASSERT_EQUAL( (double)0, town->getPositionX() );
    CPPUNIT_ASSERT_EQUAL( (double)0, town->getPositionY() );

    blieng::Point point = town->getPosition();
    CPPUNIT_ASSERT_EQUAL( (double)0, point.x );
    CPPUNIT_ASSERT_EQUAL( (double)0, point.y );
}

void TownTest::population()
{
    std::unique_ptr<blieng::Town> town(new blieng::Town());

    CPPUNIT_ASSERT_EQUAL( (unsigned int)0, town->getPopulation() );

    blieng::Character *obj1 = new blieng::Character();
    town->addCharacter(obj1);

    CPPUNIT_ASSERT_EQUAL( (unsigned int)1, town->getPopulation() );

    blieng::Character *obj2 = new blieng::Character();
    town->addCharacter(obj2);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)2, town->getPopulation() );


    // Killing object does not affect on town population directly
    obj1->kill();
    CPPUNIT_ASSERT_EQUAL( (unsigned int)2, town->getPopulation() );
    town->updatePopulation();
    CPPUNIT_ASSERT_EQUAL( (unsigned int)1, town->getPopulation() );

    town->removeCharacter(obj1);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)1, town->getPopulation() );

    {
        const std::vector<blieng::Character *> chrs = town->getCharacters();
        CPPUNIT_ASSERT_EQUAL( (size_t)1, chrs.size() );
        CPPUNIT_ASSERT_EQUAL( *(chrs.begin()), obj2 );
    }

    town->removeCharacter(obj2);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)0, town->getPopulation() );

    {
        const std::vector<blieng::Character *> chrs = town->getCharacters();
        CPPUNIT_ASSERT_EQUAL( (size_t)0, chrs.size() );
    }
}

void TownTest::characterClass()
{
    std::unique_ptr<blieng::Town> town(new blieng::Town());

    CPPUNIT_ASSERT_EQUAL( (unsigned int)0,  town->getCharacterClassCnt("human") );

    blieng::Character *obj = new blieng::Character();
    obj->setValue("class", std::string("human"));
    town->addCharacter(obj);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)1,  town->getCharacterClassCnt("human") );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)0,  town->getCharacterClassCnt("robot") );

    obj = new blieng::Character();
    obj->setValue("class", std::string("human"));
    town->addCharacter(obj);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)2,  town->getCharacterClassCnt("human") );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)0,  town->getCharacterClassCnt("robot") );

    obj = new blieng::Character();
    obj->setValue("class", std::string("robot"));
    town->addCharacter(obj);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)2,  town->getCharacterClassCnt("human") );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)1,  town->getCharacterClassCnt("robot") );

    obj = new blieng::Character();
    obj->setValue("class", std::string("human"));
    town->addCharacter(obj);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)3,  town->getCharacterClassCnt("human") );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)1,  town->getCharacterClassCnt("robot") );

    blieng::Character *obj_robot = new blieng::Character();
    obj_robot->setValue("class", std::string("robot"));
    town->addCharacter(obj_robot);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)3,  town->getCharacterClassCnt("human") );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)2,  town->getCharacterClassCnt("robot") );


    for (unsigned int i = 0; i < 5; i++) {
        obj = new blieng::Character();
        obj->setValue("class", std::string("human"));
        town->addCharacter(obj);
        CPPUNIT_ASSERT_EQUAL( (unsigned int)3 + i + 1,  town->getCharacterClassCnt("human") );
        CPPUNIT_ASSERT_EQUAL( (unsigned int)2,  town->getCharacterClassCnt("robot") );
    }

    std::vector<blieng::Character *> robots = town->getCharacterClass("robot");
    CPPUNIT_ASSERT_EQUAL( (size_t)2, robots.size() );

    CPPUNIT_ASSERT_EQUAL( obj_robot, *(robots.begin() + 1) );
}
