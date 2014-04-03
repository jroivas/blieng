#include "eventlog_test.h"
#include "eventlog.h"
#include <bliobject.h>
#include <item.h>
#include <memory>

using std::unique_ptr;

CPPUNIT_TEST_SUITE_REGISTRATION( EventLogTest );

using blieng::EventLog;
using blieng::ObjectLog;

void EventLogTest::basic()
{
    EventLog *obj = EventLog::getInstance();

    CPPUNIT_ASSERT( obj != NULL );

    obj->log("dummy", 42);
    obj->log("brains", 1);
    obj->log("dummy", 59);
    obj->log("brains", 9);
    obj->log("brains", 999);

    const ObjectLog *log = obj->get("dummy");
    const ObjectLog *blog = obj->get("brains");

    CPPUNIT_ASSERT( log != NULL );
    CPPUNIT_ASSERT( log->getName() == "dummy" );
    CPPUNIT_ASSERT( log->events.size() == 2 );

    CPPUNIT_ASSERT( blog != NULL );
    CPPUNIT_ASSERT( blog->getName() == "brains" );
    CPPUNIT_ASSERT( blog->events.size() == 3 );
}

void EventLogTest::events()
{
    EventLog *obj = EventLog::getInstance();

    obj->logString("events", "Something happened");
    obj->logString("events", "Got 10 gold");
    obj->log("cash", 10);
    obj->logString("events", "Killed a zombie");
    obj->log("kills", 1);
    obj->logString("events", "Going to a shop");
    obj->logString("events", "Buy a bicycle");
    obj->log("cash", std::string("shop"));
    obj->log("cash", -3);

    ObjectLog *log = obj->get("events");
    CPPUNIT_ASSERT( log->getName() == "events" );
    CPPUNIT_ASSERT( log->events.size() == 5 );

    CPPUNIT_ASSERT( log->toString().find("Got 10 gold") != std::string::npos );
    CPPUNIT_ASSERT( log->toString().find("Buy a") != std::string::npos );
    CPPUNIT_ASSERT( log->toString().find("[events] Killed a zombie") != std::string::npos );

    log = obj->get("cash");

    CPPUNIT_ASSERT( log->toString().find("[cash]") != std::string::npos );
    CPPUNIT_ASSERT( log->toString().find("[cash] 10") != std::string::npos );
    CPPUNIT_ASSERT( log->toString().find("[cash] -3") != std::string::npos );
}

void EventLogTest::counters()
{
    EventLog *obj = EventLog::getInstance();
    CPPUNIT_ASSERT_EQUAL( (unsigned int)0, obj->getCounter("test1") );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)0, obj->getCounter("test2") );

    obj->incrementCounter("test1");
    CPPUNIT_ASSERT_EQUAL( (unsigned int)1, obj->getCounter("test1") );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)0, obj->getCounter("test2") );

    obj->incrementCounter("test1");
    CPPUNIT_ASSERT_EQUAL( (unsigned int)2, obj->getCounter("test1") );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)0, obj->getCounter("test2") );

    obj->incrementCounter("test1");
    obj->incrementCounter("test2");
    CPPUNIT_ASSERT_EQUAL( (unsigned int)3, obj->getCounter("test1") );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)1, obj->getCounter("test2") );

    obj->incrementCounter("test2", 5);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)3, obj->getCounter("test1") );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)6, obj->getCounter("test2") );

    obj->incrementCounter("test1", 1024);
    CPPUNIT_ASSERT_EQUAL( (unsigned int)1027, obj->getCounter("test1") );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)6, obj->getCounter("test2") );
}
