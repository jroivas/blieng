#include "character_test.h"
#include <character.h>
#include <bliobject.h>
#include <item.h>

CPPUNIT_TEST_SUITE_REGISTRATION( CharacterTest );

using blieng::Character;

void CharacterTest::alive_kill()
{
    Character *obj = new Character();
    CPPUNIT_ASSERT( obj->isAlive() );

    CPPUNIT_ASSERT( !obj->getBoolValue("dead") );

    obj->kill();
    CPPUNIT_ASSERT( !obj->isAlive() );

    CPPUNIT_ASSERT( obj->getBoolValue("dead") );
}

void CharacterTest::items()
{
    blieng::Item *tmp = new blieng::Item();
    blieng::Item *tmp2 = new blieng::Item();

    Character *obj = new Character();

    CPPUNIT_ASSERT( obj->getItems().size() == 0);

    obj->addItem(tmp);
    CPPUNIT_ASSERT( obj->getItems().size() == 1);

    obj->addItem(tmp2);
    CPPUNIT_ASSERT( obj->getItems().size() == 2);


    CPPUNIT_ASSERT( obj->removeItem(tmp2) );
    CPPUNIT_ASSERT( obj->getItems().size() == 1);

    CPPUNIT_ASSERT( !obj->removeItem(tmp2) );
    CPPUNIT_ASSERT( obj->getItems().size() == 1);

    CPPUNIT_ASSERT( obj->removeItem(tmp) );
    CPPUNIT_ASSERT( obj->getItems().size() == 0);

    CPPUNIT_ASSERT( !obj->removeItem(tmp) );
    CPPUNIT_ASSERT( obj->getItems().size() == 0);

    obj->addItem(tmp);
    obj->addItem(tmp2);
    CPPUNIT_ASSERT( obj->getItems().size() == 2);

    bool found = false;
    std::vector<blieng::Item*> items = obj->getItems();
    std::vector<blieng::Item*>::iterator ii = items.begin();
    while (ii != items.end()) {
        if (*ii == tmp2) found = true;
        ++ii;
    }
    CPPUNIT_ASSERT( found );

    found = false;
    ii = items.begin();
    while (ii != items.end()) {
        if (*ii == tmp) found = true;
        ++ii;
    }
    CPPUNIT_ASSERT( found );

    CPPUNIT_ASSERT( obj->removeItem(tmp) );

    found = false;
    items = obj->getItems();
    ii = items.begin();
    while (ii != items.end()) {
        if (*ii == tmp) found = true;
        ++ii;
    }
    CPPUNIT_ASSERT( !found );
}

void CharacterTest::assign()
{
    blieng::Item *tmp = new blieng::Item();
    blieng::Item *tmp2 = new blieng::Item();

    tmp->setValue("name", std::string("item1"));
    tmp2->setValue("name", std::string("item2"));

    Character *obj = new Character();
    obj->addItem(tmp);
    obj->addItem(tmp2);

    CPPUNIT_ASSERT( obj->getItems().size() == 2);

    Character *obj2 = new Character();
    CPPUNIT_ASSERT( obj2->getItems().size() == 0);

    obj2->assignObject(obj);

    CPPUNIT_ASSERT( obj2->getItems().size() == 2);

    std::vector<blieng::Item*> items = obj2->getItems();
    std::vector<blieng::Item*>::iterator ii = items.begin();
    bool found = false;
    blieng::Item *tmp3 = NULL;
    while (ii != items.end()) {
        if ((*ii)->getStringValue("name") == "item1") {
            found = true;
            tmp3 = (*ii);
        }
        ++ii;
    }
    CPPUNIT_ASSERT( found );

    ii = items.begin();
    found = false;
    while (ii != items.end()) {
        if ((*ii)->getStringValue("name") == "item2") found = true;
        ++ii;
    }
    CPPUNIT_ASSERT( found );

    CPPUNIT_ASSERT( tmp3 != tmp );
    CPPUNIT_ASSERT( tmp->getStringValue("name") == "item1" );
    CPPUNIT_ASSERT( tmp3->getStringValue("name") == "item1" );

    tmp3->setValue("name", std::string("item3") );

    CPPUNIT_ASSERT( tmp->getStringValue("name") == "item1" );
    CPPUNIT_ASSERT( tmp3->getStringValue("name") == "item3" );

    ii = items.begin();
    found = false;
    while (ii != items.end()) {
        if ((*ii)->getStringValue("name") == "item1") found = true;
        ++ii;
    }
    CPPUNIT_ASSERT( !found );

    ii = items.begin();
    found = false;
    while (ii != items.end()) {
        if ((*ii)->getStringValue("name") == "item3") found = true;
        ++ii;
    }
    CPPUNIT_ASSERT( found );
}

void CharacterTest::assign_object()
{
    Character *chr = new Character();
    blieng::BliObject *obj = new blieng::BliObject();

    obj->setValue("name", std::string("obj"));
    obj->setValue("test1", 42);

    chr->setValue("name", std::string("chr"));
    chr->setValue("cnt", 1);

    CPPUNIT_ASSERT( obj->getStringValue("name") == "obj" );
    CPPUNIT_ASSERT( obj->getIntValue("test1") == 42 );

    CPPUNIT_ASSERT( chr->getStringValue("name") == "chr" );
    CPPUNIT_ASSERT( chr->getIntValue("cnt") == 1 );

    CPPUNIT_ASSERT( !obj->isValue("cnt") );
    CPPUNIT_ASSERT( !chr->isValue("test1") );

    chr->assignObject(obj);

    CPPUNIT_ASSERT( obj->getStringValue("name") == "obj" );
    CPPUNIT_ASSERT( obj->getIntValue("test1") == 42 );

    CPPUNIT_ASSERT( chr->getStringValue("name") == "obj" );
    CPPUNIT_ASSERT( chr->getIntValue("cnt") == 1 );
    CPPUNIT_ASSERT( chr->getIntValue("test1") == 42 );

    CPPUNIT_ASSERT( !obj->isValue("cnt") );
    CPPUNIT_ASSERT( chr->isValue("test1") );
}
