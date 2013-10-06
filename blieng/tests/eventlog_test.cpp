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

    ObjectLog *log = obj->get("dummy");

    //CPPUNIT_ASSERT( obj-> );
}
