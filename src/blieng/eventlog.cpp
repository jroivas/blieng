#include "eventlog.h"
#include <boost/foreach.hpp>

using blieng::ObjectLog;
using blieng::EventLog;

ObjectLog::ObjectLog() : BliObject()
{
	object = NULL;
}

ObjectLog::ObjectLog(void *obj) : BliObject(), object(obj)
{
}

void *ObjectLog::getObject()
{
	return object;
}

void ObjectLog::assign(void *obj)
{
	object = obj;
}

void ObjectLog::addEvent(boost::any event)
{
	boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
	std::pair<boost::posix_time::ptime, boost::any> data;
	data.first = now;
	data.second = event;
	events.push_back(data);
}

static EventLog *__static_event_log = NULL;

EventLog *EventLog::getInstance()
{
	if (__static_event_log == NULL) {
		__static_event_log = new EventLog();
	}
	return __static_event_log;
}

EventLog::EventLog()
{
}

void EventLog::log(void *object, boost::any event)
{
	BOOST_FOREACH(ObjectLog *log, events) {
		if (log->getObject() == object) {
			log->addEvent(event);
			return;
		}
	}

	ObjectLog *newobject = new ObjectLog(object);
	events.push_back(newobject);
	newobject->addEvent(event);
}

ObjectLog *EventLog::get(void *object)
{
	BOOST_FOREACH(ObjectLog *log, events) {
		if (log->getObject() == object) {
			return log;
		}
	}

	return NULL;
}

std::vector<ObjectLog *> EventLog::getAll()
{
	return events;
}

