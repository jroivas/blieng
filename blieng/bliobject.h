#ifndef __BLIENG_BLIOBJECT_H
#define __BLIENG_BLIOBJECT_H

#include <string>
#include <map>
#include <list>
#include "bliany.h"

namespace blieng
{

class BliObject
{
public:
    BliObject();
    virtual ~BliObject();

    void setValue(std::string key, BliAny value);
    virtual bool isValue(std::string key);

    virtual bool increase(std::string key);
    virtual bool decrease(std::string key);
    bool changeNumberValue(std::string key, int diff);

    virtual BliAny getValue(std::string key) const;
    virtual std::string getStringValue(std::string key, std::string default_value="") const;
    virtual bool getBoolValue(std::string key, bool default_value=false) const;
    virtual int getIntValue(std::string key, int default_value=0) const;
    virtual unsigned int getUIntValue(std::string key, unsigned int default_value=0) const;
    virtual double getDoubleValue(std::string key, double default_value=0.0) const;
    virtual std::vector<std::string> getListValue(std::string key);
    virtual std::vector<int> getIntValues(std::string key);

    virtual std::string toString() const;
    const std::type_info *getValueType(std::string key);

    virtual std::list<std::string> getKeys();
    inline unsigned int size() { return values.size(); }

    void assignObject(const BliObject *another);

    static int getRandomInt(int limit_low, int limit_max);
    static double getRandomDouble(double limit_low, double limit_max);
    static bool getRandomBoolean();

protected:
    std::map<std::string, BliAny> values;
};

}

#endif
