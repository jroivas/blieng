#ifndef __BLIENG_EVENTLOG_H
#define __BLIENG_EVENTLOG_H

#include "bliobject.h"
#include "auto_vector.h"
#include <vector>
#include <memory>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>

using std::unique_ptr;

namespace blieng
{

/**
 * Logging events for specific object.
 * Automatically records the time of the event.
 */
class ObjectLog : public BliObject
{
public:
    /**
     * Logging for nullptr
     */
    ObjectLog();
    /**
     * Logging for any object
     *
     * \param obj Object reference
     */
    ObjectLog(void *obj);
    /**
     * Logging for specific name
     *
     * \param name Name reference
     */
    ObjectLog(const std::string &name);
    virtual ~ObjectLog() {}

    /**
     * Assign object for this log
     *
     * \param obj Object reference
     */
    void assign(void *obj);
    /**
     * Add a event for log.
     * Event can be any type, however only some certain types are natively supported.
     *
     * \param event The event to be recorded
     */
    void addEvent(BliAny event);
    /**
     * Get the object reference
     *
     * \returns Pointer to the assigned object
     */
    void *getObject();
    /**
     * Get the name assigned to this object.
     *
     * \returns The assigned name
     */
    std::string getName() const;
    /**
     * Assign a name for this object.
     *
     * \param name The name to be assigned
     */
    void setName(const std::string &name);

    /**
     * Get string presentation of all events.
     * Will print time of the event, name if defined
     * and finally the data possibly in human readable format.
     *
     * \returns String representation of events
     */
    std::string toString() const;

    /**
     * Will try to convert anything to string.
     * Tries to show it in human readable format.
     *
     * \param data Any data
     * \returns String presentation of the data.
     */
    static std::string anyToString(BliAny data);

    /**
     * Contains all recorded events.
     */
    std::vector<std::pair<boost::posix_time::ptime, BliAny> > events;

private:
    void *object;
    std::string name;
};

//FIXME: implement counters and achivements

/**
 * General event logging
 */
class EventLog
{
public:
    /**
     * Get the event log singleton.
     * TODO FIXME: Get rid of singleton?
     * \return Instance of event log
     */
    static EventLog *getInstance();
    virtual ~EventLog() {}

    /**
     * Log an event with object.
     * Adds event to the log, maps it to a object.
     *
     * \param object Object reference
     * \param event The event to be recorded
     */
    void log(void *object, BliAny event);
    /**
     * Log an event with name.
     * Adds event to the log, maps it to a name.
     *
     * \param name Name reference
     * \param event The event to be recorded
     */
    void log(const std::string &name, BliAny event);
    /**
     * Log a string
     * Adds string to the log, maps it to a name.
     *
     * \param name Name reference
     * \param event The string to be added
     */
    void logString(const std::string &name, const std::string &event);
    /**
     * Get object log mapped to a object.
     * Object log will contain all events mapped to the specific object.
     *
     * \param object Object reference
     * \returns An \ref ObjectLog instance containing all events mapped to the object
     */
    ObjectLog *get(void *object);
    /**
     * Get object log mapped to a name.
     * Object log will contain all events mapped to the specific name.
     *
     * \param name Name reference
     * \returns An \ref ObjectLog instance containing all events mapped to the name
     */
    ObjectLog *get(const std::string &name);

private:
    EventLog();
    auto_vector<ObjectLog> events;
};

}

#endif
