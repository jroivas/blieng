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
