#ifndef __BLIENG_EVENTLOG_H
#define __BLIENG_EVENTLOG_H

#include "bliobject.h"
#include "auto_vector.h"
#include <vector>
#include <memory>
#include <map>
//#include <boost/any.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using std::unique_ptr;

namespace blieng
{

class ObjectLog : public BliObject
{
public:
    ObjectLog();
    ObjectLog(void *obj);
    ObjectLog(std::string name);
    virtual ~ObjectLog() {}

    void assign(void *obj);
    void addEvent(BliAny event);
    void *getObject();
    std::string getName() const;
    void setName(std::string name);

    std::vector<std::pair<boost::posix_time::ptime, BliAny> > events;

    std::string toString() const;

private:
    void *object;
    std::string name;
    std::string anyToString(BliAny data) const;
};

class EventLog
{
public:
    static EventLog *getInstance();
    virtual ~EventLog() {}

    void log(void *object, BliAny event);
    void log(std::string name, BliAny event);
    void logString(std::string name, std::string event);
    ObjectLog *get(void *object);
    ObjectLog *get(std::string name);

private:
    EventLog();
    auto_vector<ObjectLog> events;
};

}

#endif
