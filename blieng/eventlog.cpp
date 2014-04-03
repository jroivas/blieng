#include "eventlog.h"
#include <boost/foreach.hpp>
#include <sstream>

using blieng::ObjectLog;
using blieng::EventLog;
using blieng::BliAny;

static std::string __counter_object_name = "__global_counters";

ObjectLog::ObjectLog() : BliObject()
{
    object = nullptr;
}

ObjectLog::ObjectLog(void *obj) : BliObject(), object(obj), name("")
{
}

ObjectLog::ObjectLog(const std::string &_name) : BliObject(), object(nullptr), name(_name)
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

void ObjectLog::setName(const std::string &_name)
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

std::string ObjectLog::anyToString(BliAny data)
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

static EventLog *__static_event_log = nullptr;

EventLog *EventLog::getInstance()
{
    if (__static_event_log == nullptr) {
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

void EventLog::log(const std::string &name, BliAny event)
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

void EventLog::logString(const std::string &name, const std::string &event)
{
    BliAny val = event;
    log(name, val);
}

ObjectLog *EventLog::get(const std::string &name)
{
    BOOST_FOREACH(ObjectLog *_log, events) {
        if (_log->getName() == name) {
            return _log;
        }
    }

    return nullptr;
}

ObjectLog *EventLog::get(void *object)
{
    BOOST_FOREACH(ObjectLog *_log, events) {
        if (_log->getObject() == object) {
            return _log;
        }
    }

    return nullptr;
}

void EventLog::incrementCounter(const std::string &name, unsigned int cnt)
{
    ObjectLog *__counters = nullptr;
    BOOST_FOREACH(ObjectLog *_log, events) {
        if (_log->getName() == __counter_object_name) {
            __counters = _log;
            break;
        }
    }

    if (__counters == nullptr) {
        __counters = new ObjectLog(__counter_object_name);
        unique_ptr<ObjectLog> newobject(__counters);
        events.push_back(std::move(newobject));
    }

    if (!__counters->isValue(name)) __counters->setValue(name, static_cast<unsigned int>(0));
    __counters->changeNumberValue(name, cnt);
}

unsigned int EventLog::getCounter(const std::string &name) const
{
    BOOST_FOREACH(ObjectLog *_log, events) {
        if (_log->getName() == __counter_object_name) {
            if (_log->isValue(name)) {
                return _log->getUIntValue(name);
            }
            return 0;
        }
    }

    return 0;
}
