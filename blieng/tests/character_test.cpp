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
