#ifndef __BLIENG_BLIOBJECT_H
#define __BLIENG_BLIOBJECT_H

#include "config.h"
#include <string>
#include <map>
#include <list>
#include "bliany.h"
#ifdef DATA_MUTEX_LOCK
#include <boost/thread/mutex.hpp>
#endif

namespace blieng
{

class BliObject
{
public:
    BliObject();
    virtual ~BliObject();

    void setValue(const std::string &key, BliAny value);
    virtual bool isValue(const std::string &key) const;

    virtual bool increase(const std::string &key);
    virtual bool decrease(const std::string &key);
    bool changeNumberValue(const std::string &key, int diff);

    virtual BliAny getValue(const std::string &key) const;
    virtual std::string getStringValue(const std::string &key, const std::string &default_value="") const;
    virtual bool getBoolValue(const std::string &key, const bool &default_value=false) const;
    virtual int getIntValue(const std::string &key, int default_value=0) const;
    virtual unsigned int getUIntValue(const std::string &key, unsigned int default_value=0) const;
    virtual double getDoubleValue(const std::string &key, double default_value=0.0) const;
    virtual std::vector<std::string> getListValue(const std::string &key);
    virtual std::vector<int> getIntValues(const std::string &key);

    virtual std::string toString() const;
    const std::type_info *getValueType(const std::string &key) const;

    virtual std::list<std::string> getKeys();
    inline unsigned int size() { return values.size(); }

    void assignObject(const BliObject *another);

    static int getRandomInt(int limit_low, int limit_max);
    static double getRandomDouble(double limit_low, double limit_max);
    static bool getRandomBoolean();

protected:
    std::map<std::string, BliAny> values;

#ifdef DATA_MUTEX_LOCK
private:
    boost::mutex value_mutex;
#endif
};

}

#endif
