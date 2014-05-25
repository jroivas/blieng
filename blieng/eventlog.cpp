/*
 * Copyright 2014 Blistud:io
 */

#include "blieng/eventlog.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using blieng::ObjectLog;
using blieng::EventLog;
using blieng::BliAny;

static std::string __counter_object_name = "__global_counters";

ObjectLog::ObjectLog() : BliObject()
{
    object = nullptr;
}

ObjectLog::ObjectLog(void *obj) :
    BliObject(), object(obj), name("")
{
}

ObjectLog::ObjectLog(const std::string &_name) :
    BliObject(), object(nullptr), name(_name)
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
    boost::posix_time::ptime now =
        boost::posix_time::microsec_clock::universal_time();
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

std::string ObjectLog::timeToString(boost::posix_time::ptime timestamp) const
{
    return to_simple_string(timestamp);
}

std::string ObjectLog::nameDataToString(BliAny anydata) const
{
    std::string res = "";
    if (name != "") {
        res += "[";
        res += name;
        res += "] ";
    }
    res += anyToString(anydata);
    return res;
}

std::string ObjectLog::eventToString(
    std::vector<
        std::pair<boost::posix_time::ptime, BliAny>
    >::const_iterator iter) const
{
    std::string res = timeToString((*iter).first);
    res += " " + nameDataToString((*iter).second);
    res += "\n";

    return res;
}

std::string ObjectLog::toString() const
{
    std::string res = "";

    auto it = events.cbegin();
    while (it != events.cend()) {
        res += eventToString(it);
        ++it;
    }

    return res + BliObject::toString();
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
    for (ObjectLog *_log : events) {
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
    for (ObjectLog *_log : events) {
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
    for (ObjectLog *_log : events) {
        if (_log->getName() == name) {
            return _log;
        }
    }

    return nullptr;
}

ObjectLog *EventLog::get(void *object)
{
    for (ObjectLog *_log : events) {
        if (_log->getObject() == object) {
            return _log;
        }
    }

    return nullptr;
}

void EventLog::incrementCounter(const std::string &name, unsigned int cnt)
{
    ObjectLog *__counters = nullptr;
    for (ObjectLog *_log : events) {
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

    if (!__counters->isValue(name))
        __counters->setValue(name, static_cast<unsigned int>(0));

    __counters->changeNumberValue(name, cnt);
}

unsigned int EventLog::getCounter(const std::string &name) const
{
    for (ObjectLog *_log : events) {
        if (_log->getName() == __counter_object_name) {
            if (_log->isValue(name)) {
                return _log->getUIntValue(name);
            }
            return 0;
        }
    }

    return 0;
}

std::string EventLog::toString() const
{
    std::string res = "";
    std::string counters = "";

    for (ObjectLog *_log : events) {
        if (_log->getName() == __counter_object_name) {
            counters += _log->toString();
        } else {
            res += _log->toString();
        }
    }

    return res + counters;
}

bool compareTime(
    std::pair<boost::posix_time::ptime, BliAny> i,
    std::pair<boost::posix_time::ptime, BliAny> j);
bool compareTime(
    std::pair<boost::posix_time::ptime, BliAny> i,
    std::pair<boost::posix_time::ptime, BliAny> j)
{
    return i.first < j.first;
}

std::string EventLog::toChronologicalString() const
{
    std::string res = "";
    std::string counters = "";

    ObjectLog tmplog;
    for (ObjectLog *_log : events) {
        if (_log->getName() == __counter_object_name) {
            counters += _log->toString();
        } else {
            /* tmplog.events.insert(
                    tmplog.events.end(),
                    _log->events.begin(),
                    _log->events.end());
             */
            auto it = _log->events.cbegin();
            while (it != _log->events.cend()) {
                std::pair<boost::posix_time::ptime, BliAny> data;
                data.first = (*it).first;
                data.second = _log->nameDataToString((*it).second);
                tmplog.events.push_back(data);
                ++it;
            }
        }
    }
    std::sort(tmplog.events.begin(), tmplog.events.end(), compareTime);

    return tmplog.toString() + counters;
}
