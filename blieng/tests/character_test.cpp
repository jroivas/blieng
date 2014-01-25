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

    obj->addItem(std::move(tmp));
    obj->addItem(std::move(tmp2));
    CPPUNIT_ASSERT( obj->size() == 2);

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
