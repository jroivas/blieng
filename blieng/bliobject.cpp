#include "bliobject.h"
#include "data.h"
#include <iostream>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

typedef std::pair<std::string, boost::any> values_t;
typedef std::map<std::string, boost::any>::iterator values_iter_t;

using blieng::BliObject;

BliObject::BliObject()
{
}

void BliObject::assignObject(BliObject *another)
{
    if (another == NULL) return;

    BOOST_FOREACH(values_t val, another->values) {
        values[val.first] = val.second;
    }
}

bool BliObject::isValue(std::string key)
{
    values_iter_t value_iter = values.find(key);

    if (value_iter == values.end()) return false;
    return true;
}

int BliObject::getRandomInt(int limit_low, int limit_max)
{
    boost::random::uniform_int_distribution<> dist(limit_low, limit_max);
    return dist(*Data::getInstance()->getGen());
}

double BliObject::getRandomDouble(double limit_low, double limit_max)
{
    boost::random::uniform_real_distribution<> dist(limit_low, limit_max);
    return dist(*Data::getInstance()->getGen());
}

boost::any BliObject::getValue(std::string key)
{
    values_iter_t value_iter = values.find(key);
    if (value_iter == values.end()) {
        std::cout << "Error, key not found: " + key + "\n";
        throw "Error, key not found: " + key;
    }
    return value_iter->second;
}

void BliObject::setValue(std::string key, boost::any value)
{
    values[key] = value;
}

#define getConvertValue(X, Y) \
Y BliObject::get ## X ## Value(std::string key)\
{\
    boost::any val = getValue(key);\
    if (val.empty()) {\
        std::cout << "Error, key not found: " + key + "\n";\
        throw "Error, key not found: " + key;\
    }\
    if (val.type() == typeid(Y)) {\
        return boost::any_cast<Y>(val);\
    }\
    try {\
        return boost::any_cast<Y>(val);\
    } catch (boost::bad_any_cast c) {\
        std::cout << "Error, not a " #X " value at: " + key + "\n";\
        throw "Error, not a " #X " value at: " + key;\
    }\
}

#define getConvertNumberValue(X, Y, A, B, C) \
Y BliObject::get ## X ## Value(std::string key)\
{\
    boost::any val = getValue(key);\
    if (val.empty()) {\
        std::cout << "Error, key not found: " + key + "\n";\
        throw "Error, key not found: " + key;\
    }\
    if (val.type() == typeid(Y)) {\
        return boost::any_cast<Y>(val);\
    } else {\
        if (val.type() == typeid(A)) { return boost::any_cast<A>(val); }\
        if (val.type() == typeid(B)) { return boost::any_cast<B>(val); }\
        if (val.type() == typeid(C)) { return boost::any_cast<C>(val); }\
    }\
    throw "Error, not a " #X " value at: " + key;\
}

getConvertValue(String, std::string)
getConvertNumberValue(Int, int, unsigned int, double, float)
getConvertNumberValue(UInt, unsigned int, int, double, float)
getConvertNumberValue(Double, double, float, unsigned int, int)
getConvertValue(Bool, bool)

const std::type_info *BliObject::getValueType(std::string key)
{
    boost::any val = getValue(key);
    if (val.empty()) {
        std::cout << "Error, key not found: " + key + "\n";
        throw "Error, key not found: " + key;
    }
    return &val.type();
}

std::string BliObject::toString()
{
    std::string res = "";
    BOOST_FOREACH(values_t item, values) {
        std::string key = item.first;
        boost::any val = item.second;

        res += (boost::format("%20s: ") % key).str();

        if (val.type() == typeid(int)) {
            res += (boost::format("%d") % boost::any_cast<int>(val)).str();
        }
        else if (val.type() == typeid(unsigned int)) {
            res += (boost::format("%u") % boost::any_cast<unsigned int>(val)).str();
        }
        else if (val.type() == typeid(bool)) {
            res += boost::any_cast<bool>(val) ? "true" : "false";
        }
        else if (val.type() == typeid(double)) {
            res += (boost::format("%f") % boost::any_cast<double>(val)).str();
        }
        else if (val.type() == typeid(std::string)) {
            res += boost::any_cast<std::string>(val);
        }
        else {
            res += "<unknown type>";
        }
        res += "\n";
    }

    return res;
}

std::list<std::string> BliObject::getKeys()
{
    std::list<std::string> res;
    BOOST_FOREACH(values_t val, values) {
        res.push_back(val.first);
    }
    return res;
}

bool BliObject::changeIntValue(std::string key, int diff)
{
    int maxval = -1;
    if (isValue(key + "-max")) {
        boost::any max_item = getValue(key + "-max");
        if (max_item.type() == typeid(int)) {
            maxval = boost::any_cast<int>(max_item);
        }
    }

    int val = -1;
    try {
        val = getIntValue(key);
    }
    catch (std::string e) {
        return false;
    }
    if ((val + diff >= 0) && (maxval == -1 || (val + diff) < maxval)) {
        setValue(key, val + diff);
        return true;
    }
    return false;
}

bool BliObject::increase(std::string key)
{
    return changeIntValue(key, 1);
}

bool BliObject::decrease(std::string key)
{
    return changeIntValue(key, -1);
}
