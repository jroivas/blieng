#include "bliobject.h"
#include "data.h"
#include <iostream>
#include <sstream>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/lexical_cast.hpp>

#ifdef DATA_MUTEX_LOCK
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
#endif

#ifdef PSEUDO_RANDOM
#include <boost/random/mersenne_twister.hpp>
#else
#include <boost/random/random_device.hpp>
#endif

using blieng::BliObject;
using blieng::BliAny;

typedef std::pair<std::string, BliAny> values_t;
typedef std::map<std::string, BliAny>::iterator values_iter_t;
typedef std::map<std::string, BliAny>::const_iterator values_const_iter_t;

#ifdef ANDROID
#include <QDebug>
void doDebug(std::string s)
{
    qDebug() << s.c_str();
}
#else
void doDebug(std::string s)
{
    std::cerr << s << "\n";
}
#endif

BliObject::BliObject()
{
}

BliObject::~BliObject()
{
    // Enforce deletion
    while (!values.empty()) {
        values_iter_t data = values.begin();
        values.erase(data);
    }
}

void BliObject::assignObject(const BliObject *another)
{
    if (another == nullptr) return;

#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(value_mutex);
#endif
    BOOST_FOREACH(values_t val, another->values) {
        values[val.first] = val.second;
    }
}

bool BliObject::isValue(std::string key)
{
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(value_mutex);
#endif
    values_iter_t value_iter = values.find(key);

    if (value_iter == values.end()) return false;
    return true;
}

int BliObject::getRandomInt(int limit_low, int limit_max)
{
#ifdef PSEUDO_RANDOM
    boost::random::mt19937 gen;
#else
    boost::random::random_device gen;
#endif
    boost::random::uniform_int_distribution<> dist(limit_low, limit_max);
    int res = dist(gen);
    return res;
}

double BliObject::getRandomDouble(double limit_low, double limit_max)
{
#ifdef PSEUDO_RANDOM
    boost::random::mt19937 gen;
#else
    boost::random::random_device gen;
#endif
    boost::random::uniform_real_distribution<> dist(limit_low, limit_max);
    double res = dist(gen);
    return res;
}

BliAny BliObject::getValue(std::string key) const
{
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(value_mutex);
#endif
    values_const_iter_t value_iter = values.find(key);

    if (value_iter == values.end()) {
        doDebug("Error, key not found: " + key);
        throw std::string("Error, key not found: " + key);
    }

    return value_iter->second;
}

void BliObject::setValue(std::string key, BliAny value)
{
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(value_mutex);
#endif
    values[key] = value;
}

#define getConvertValue(X, Y) \
Y BliObject::get ## X ## Value(std::string key, Y default_value) const\
{\
    BliAny val = getValue(key);\
    if (val.empty()) {\
        doDebug("Error, key not found: " + key);\
        throw "Error, key not found: " + key;\
    }\
    if (val.type() == typeid(Y)) {\
        return boost::any_cast<Y>(val);\
    }\
    try {\
        return boost::any_cast<Y>(val);\
    } catch (boost::bad_any_cast &c) {\
        doDebug("Error, not a " #X " value at: " + key);\
        /*throw "Error, not a " #X " value at: " + key;*/\
        return default_value;\
    }\
}

#define getConvertNumberValue(X, Y, A, B, C, D, E) \
Y BliObject::get ## X ## Value(std::string key, Y default_value) const\
{\
    BliAny val = getValue(key);\
    if (val.empty()) {\
        doDebug("Error, key not found: " + key);\
        throw "Error, key not found: " + key;\
    }\
    if (val.type() == typeid(Y)) {\
        return boost::any_cast<Y>(val);\
    } else {\
        if (val.type() == typeid(A)) { return static_cast<Y>(boost::any_cast<A>(val)); }\
        if (val.type() == typeid(B)) { return static_cast<Y>(boost::any_cast<B>(val)); }\
        if (val.type() == typeid(C)) { return static_cast<Y>(boost::any_cast<C>(val)); }\
        if (val.type() == typeid(D)) { return static_cast<Y>(boost::any_cast<D>(val)); }\
        if (val.type() == typeid(E)) { return static_cast<Y>(boost::any_cast<E>(val)); }\
    }\
    std::ostringstream m;\
    m << val;\
    doDebug("Error, not a " #X " value at: " + key + ", val: " + m.str());\
    /*throw "Error, not a " #X " value at: " + key;*/\
    return default_value;\
}

getConvertValue(String, std::string)
getConvertNumberValue(Int, int, unsigned int, long, unsigned long, double, float)
getConvertNumberValue(UInt, unsigned int, unsigned long, long, int, double, float)
getConvertNumberValue(Double, double, float, unsigned long, long, unsigned int, int)
getConvertValue(Bool, bool)

std::vector<std::string> BliObject::getListValue(std::string key)
{
    BliAny val = getValue(key);
    return boost::any_cast<std::vector<std::string> >(val);
}

std::vector<int> BliObject::getIntValues(std::string key)
{
    BliAny val = getValue(key);
    return boost::any_cast<std::vector<int> >(val);
#if 0
    std::vector<std::string> vals = getListValue(key);
    std::vector<int> res;

    BOOST_FOREACH(std::string val, vals) {
        res.push_back(boost::lexical_cast<int>(val));
    }
    return res;
#endif
}

const std::type_info *BliObject::getValueType(std::string key)
{
    BliAny val = getValue(key);
    if (val.empty()) {
        doDebug("Error, key not found: " + key);
        throw "Error, key not found: " + key;
    }
    return &val.type();
}

std::string BliObject::toString() const
{
    std::string res = "";
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(value_mutex);
#endif
    BOOST_FOREACH(values_t item, values) {
        std::string key = item.first;
        BliAny val = item.second;

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
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(value_mutex);
#endif
    BOOST_FOREACH(values_t val, values) {
        res.push_back(val.first);
    }
    return res;
}

bool BliObject::changeNumberValue(std::string key, int diff)
{
    if (!isValue(key)) return false;

    BliAny val = getValue(key);
    if (val.type() == typeid(int)) {
        int num = boost::any_cast<int>(val);
        num += diff;
        if (isValue(key + "-max")) {
            int max = getIntValue(key + "-max");
            if (num > max) return false;
        }
        setValue(key, num);
    }
    else if (val.type() == typeid(unsigned int)) {
        unsigned int num = boost::any_cast<unsigned int>(val);
        if (diff < 0) {
            num -= static_cast<unsigned int>(-1 * diff);
        } else {
            num += static_cast<unsigned int>(diff);
        }
        if (isValue(key + "-max")) {
            unsigned int max = getUIntValue(key + "-max");
            if (num > max) return false;
        }
        setValue(key, num);
    }
    else if (val.type() == typeid(double)) {
        double num = boost::any_cast<double>(val);
        num += static_cast<double>(diff);
        if (isValue(key + "-max")) {
            double max = getDoubleValue(key + "-max");
            if (num > max) return false;
        }
        setValue(key, num);
    } else {
        return false;
    }

    return true;
}

bool BliObject::increase(std::string key)
{
    return changeNumberValue(key, 1);
}

bool BliObject::decrease(std::string key)
{
    return changeNumberValue(key, -1);
}

bool BliObject::getRandomBoolean()
{
    int tmp1 = getRandomInt(0, 420000);
    int tmp2 = getRandomInt(12345, 999999);

    tmp1 %= 2;
    tmp2 %= 2;
    return (tmp1 == tmp2);
}
