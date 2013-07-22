#include "character_test.h"
#include <character.h>
#include <item.h>

using blieng::Character;

void CharacterTest::alive_kill()
{
    Character *obj = new Character();
    TEST_ASSERT( obj->isAlive() );

    TEST_ASSERT( !obj->getBoolValue("dead") );

    obj->kill();
    TEST_ASSERT( !obj->isAlive() );

    TEST_ASSERT( obj->getBoolValue("dead") );
}

void CharacterTest::items()
{
    blieng::Item *tmp = new blieng::Item();
    blieng::Item *tmp2 = new blieng::Item();

    Character *obj = new Character();

    TEST_ASSERT( obj->getItems().size() == 0);

    obj->addItem(tmp);
    TEST_ASSERT( obj->getItems().size() == 1);

    obj->addItem(tmp2);
    TEST_ASSERT( obj->getItems().size() == 2);


    TEST_ASSERT( obj->removeItem(tmp2) );
    TEST_ASSERT( obj->getItems().size() == 1);

    TEST_ASSERT( !obj->removeItem(tmp2) );
    TEST_ASSERT( obj->getItems().size() == 1);

    TEST_ASSERT( obj->removeItem(tmp) );
    TEST_ASSERT( obj->getItems().size() == 0);

    TEST_ASSERT( !obj->removeItem(tmp) );
    TEST_ASSERT( obj->getItems().size() == 0);

    obj->addItem(tmp);
    obj->addItem(tmp2);
    TEST_ASSERT( obj->getItems().size() == 2);

    bool found = false;
    std::vector<blieng::Item*> items = obj->getItems();
    std::vector<blieng::Item*>::iterator ii = items.begin();
    while (ii != items.end()) {
        if (*ii == tmp2) found = true;
        ++ii;
    }
    TEST_ASSERT( found );

    found = false;
    ii = items.begin();
    while (ii != items.end()) {
        if (*ii == tmp) found = true;
        ++ii;
    }
    TEST_ASSERT( found );

    TEST_ASSERT( obj->removeItem(tmp) );

    found = false;
    items = obj->getItems();
    ii = items.begin();
    while (ii != items.end()) {
        if (*ii == tmp) found = true;
        ++ii;
    }
    TEST_ASSERT( !found );
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

    TEST_ASSERT( obj->getItems().size() == 2);

    Character *obj2 = new Character();
    TEST_ASSERT( obj2->getItems().size() == 0);

    obj2->assignObject(obj);

    TEST_ASSERT( obj2->getItems().size() == 2);

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
    TEST_ASSERT( found );

    ii = items.begin();
    found = false;
    while (ii != items.end()) {
        if ((*ii)->getStringValue("name") == "item2") found = true;
        ++ii;
    }
    TEST_ASSERT( found );

    TEST_ASSERT( tmp3 != tmp );
    TEST_ASSERT( tmp->getStringValue("name") == "item1" );
    TEST_ASSERT( tmp3->getStringValue("name") == "item1" );

    tmp3->setValue("name", std::string("item3") );

    TEST_ASSERT( tmp->getStringValue("name") == "item1" );
    TEST_ASSERT( tmp3->getStringValue("name") == "item3" );

    ii = items.begin();
    found = false;
    while (ii != items.end()) {
        if ((*ii)->getStringValue("name") == "item1") found = true;
        ++ii;
    }
    TEST_ASSERT( !found );

    ii = items.begin();
    found = false;
    while (ii != items.end()) {
        if ((*ii)->getStringValue("name") == "item3") found = true;
        ++ii;
    }
    TEST_ASSERT( found );
}
