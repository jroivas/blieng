#ifndef __BLIENG_EVENTLOG_H
#define __BLIENG_EVENTLOG_H

#include "bliobject.h"
#include <vector>
#include <map>
#include <boost/any.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace blieng
{

class ObjectLog : public BliObject
{
public:
	ObjectLog();
	ObjectLog(void *obj);
	
	void assign(void *obj);
	void addEvent(boost::any event);
	void *getObject();

	std::vector<std::pair<boost::posix_time::ptime, boost::any> > events;

private:
	void *object;
};

class EventLog
{
public:
	EventLog *getInstance();
	void log(void *object, boost::any event);
	ObjectLog *get(void *object);
	std::vector<ObjectLog *> getAll();
	

private:
	EventLog();
	std::vector<ObjectLog *> events;
};

}

#endif
