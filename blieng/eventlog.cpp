#include "eventlog.h"
#include <boost/foreach.hpp>
#include <sstream>

using blieng::ObjectLog;
using blieng::EventLog;
using blieng::BliAny;

ObjectLog::ObjectLog() : BliObject()
{
    object = NULL;
}

ObjectLog::ObjectLog(void *obj) : BliObject(), object(obj), name("")
{
}

ObjectLog::ObjectLog(std::string _name) : BliObject(), object(NULL), name(_name)
{
}

void *ObjectLog::getObject()
{
    return object;
}

std::string ObjectLog::getName() const
{
    return name;
}

void ObjectLog::setName(std::string _name)
{
    name = _name;
}

void ObjectLog::assign(void *obj)
{
    object = obj;
}

void ObjectLog::addEvent(BliAny event)
{
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
    std::pair<boost::posix_time::ptime, BliAny> data;
    data.first = now;
    data.second = event;
    events.push_back(data);
}

std::string ObjectLog::anyToString(BliAny data) const
{
    std::ostringstream res;
    res << data;

    return res.str();
}

std::string ObjectLog::toString() const
{
    std::string res = "";

    auto it = events.cbegin();
    while (it != events.cend()) {
        res += to_simple_string((*it).first);
        res += " ";
        if (name != "") {
            res += "[";
            res += name;
            res += "] ";
        }
        res += anyToString((*it).second);
        res += "\n";
        ++it;
    }

    return res;
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

void EventLog::log(void *object, BliAny event)
{
    BOOST_FOREACH(ObjectLog *_log, events) {
        if (_log->getObject() == object) {
            _log->addEvent(event);
            return;
        }
    }

    unique_ptr<ObjectLog> newobject(new ObjectLog(object));
    newobject->addEvent(event);
    events.push_back(std::move(newobject));
}

void EventLog::log(std::string name, BliAny event)
{
    BOOST_FOREACH(ObjectLog *_log, events) {
        if (_log->getName() == name) {
            _log->addEvent(event);
            return;
        }
    }

    unique_ptr<ObjectLog> newobject(new ObjectLog(name));
    newobject->addEvent(event);
    events.push_back(std::move(newobject));
}

void EventLog::logString(std::string name, std::string event)
{
    BliAny val = event;
    log(name, val);
}

ObjectLog *EventLog::get(std::string name)
{
    BOOST_FOREACH(ObjectLog *_log, events) {
        if (_log->getName() == name) {
            return _log;
        }
    }

    return NULL;
}

ObjectLog *EventLog::get(void *object)
{
    BOOST_FOREACH(ObjectLog *_log, events) {
        if (_log->getObject() == object) {
            return _log;
        }
    }

    return NULL;
}

#if 0
std::vector<ObjectLog *> EventLog::getAll()
{
    return events;
}
#endif
