#include "character_test.h"
#include "test_tools.h"
#include <character.h>
#include <bliobject.h>
#include <item.h>
#include <memory>

using std::unique_ptr;

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
    mock_io_start();
    shared_ptr<blieng::Data> _data(new blieng::Data());
    shared_ptr<blieng::Configure> _config(new blieng::Configure(_data));

    unique_ptr<blieng::Item> tmp(new blieng::Item(_config, _data));
    unique_ptr<blieng::Item> tmp2(new blieng::Item(_config, _data));

    unique_ptr<Character> obj(new Character());

    CPPUNIT_ASSERT( obj->size() == 0);

    obj->addItem(std::move(tmp));
    CPPUNIT_ASSERT( obj->size() == 1);

    obj->addItem(std::move(tmp2));
    CPPUNIT_ASSERT( obj->size() == 2);


    CPPUNIT_ASSERT( obj->removeItem(1) );
    CPPUNIT_ASSERT( obj->size() == 1);

    CPPUNIT_ASSERT( !obj->removeItem(1) );
    CPPUNIT_ASSERT( obj->size() == 1);

    CPPUNIT_ASSERT( obj->removeItem(0) );
    CPPUNIT_ASSERT( obj->size() == 0);

    CPPUNIT_ASSERT( !obj->removeItem(0) );
    CPPUNIT_ASSERT( obj->size() == 0);
#if 0
    CPPUNIT_ASSERT( obj->removeItem(tmp2.get()) );
    CPPUNIT_ASSERT( obj->getItems()->size() == 1);

    CPPUNIT_ASSERT( !obj->removeItem(tmp2.get()) );
    CPPUNIT_ASSERT( obj->getItems()->size() == 1);

    CPPUNIT_ASSERT( obj->removeItem(tmp.get()) );
    CPPUNIT_ASSERT( obj->getItems()->size() == 0);

    CPPUNIT_ASSERT( !obj->removeItem(tmp.get()) );
    CPPUNIT_ASSERT( obj->getItems()->size() == 0);
#endif

    obj->addItem(std::move(tmp));
    obj->addItem(std::move(tmp2));
    CPPUNIT_ASSERT( obj->size() == 2);

#if 0
    bool found = false;
    const auto_vector<blieng::Item> *items = obj->getItems();

    auto_vector<blieng::Item>::const_iterator ii = items->begin();
    while (ii != items->end()) {
        if (*ii == tmp2.get()) found = true;
        ++ii;
    }
    CPPUNIT_ASSERT( found );

    found = false;
    ii = items->begin();
    while (ii != items->end()) {
        if (*ii == tmp.get()) found = true;
        ++ii;
    }
    CPPUNIT_ASSERT( found );

    //CPPUNIT_ASSERT( obj->removeItem(tmp.get()) );
    CPPUNIT_ASSERT( obj->removeItem(0) );

    found = false;
    items = obj->getItems();
    ii = items->begin();
    while (ii != items->end()) {
        if (*ii == tmp.get()) found = true;
        ++ii;
    }
    CPPUNIT_ASSERT( !found );
#endif
    mock_io_stop();
}

void CharacterTest::assign()
{
    mock_io_start();
    shared_ptr<blieng::Data> _data(new blieng::Data());
    shared_ptr<blieng::Configure> _config(new blieng::Configure(_data));

    unique_ptr<blieng::Item> tmp(new blieng::Item(_config, _data));
    unique_ptr<blieng::Item> tmp2(new blieng::Item(_config, _data));

    tmp->setValue("name", std::string("item1"));
    tmp2->setValue("name", std::string("item2"));

    Character *obj = new Character();
    obj->addItem(std::move(tmp));
    obj->addItem(std::move(tmp2));

    CPPUNIT_ASSERT( obj->size() == 2);

    Character *obj2 = new Character();
    CPPUNIT_ASSERT( obj2->size() == 0);

    obj2->assignObject(obj);

    CPPUNIT_ASSERT( obj->size() == 2);

#if 0
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
#endif
    mock_io_stop();
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
