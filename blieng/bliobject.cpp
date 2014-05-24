/*
 * Copyright 2014 Blistud:io
 */

#include "blieng/bliobject.h"

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

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "blieng/data.h"
#include "blieng/logging.h"

using blieng::BliObject;
using blieng::BliAny;

typedef std::pair<std::string, BliAny> values_t;
typedef std::map<std::string, BliAny>::iterator values_iter_t;
typedef std::map<std::string, BliAny>::const_iterator values_const_iter_t;

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

bool BliObject::isValue(const std::string &key) const
{
    values_const_iter_t value_iter = values.find(key);

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
    return dist(gen);
}

double BliObject::getRandomDouble(double limit_low, double limit_max)
{
#ifdef PSEUDO_RANDOM
    boost::random::mt19937 gen;
#else
    boost::random::random_device gen;
#endif

    boost::random::uniform_real_distribution<> dist(limit_low, limit_max);
    return dist(gen);
}

BliAny BliObject::getValue(const std::string &key) const
{
    values_const_iter_t value_iter = values.find(key);

    if (value_iter == values.end()) {
        LOG_DEBUG("Error, key not found: " + key);
        throw std::string("Error, key not found: " + key);
    }

    return value_iter->second;
}

void BliObject::setValue(const std::string &key, BliAny value)
{
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(value_mutex);
#endif

    values[key] = value;
}

#define getConvertValue(X, Y) \
Y BliObject::get ## X ## Value( \
    const std::string &key, \
    const Y &default_value) const\
{\
    BliAny val = getValue(key);\
    if (val.empty()) {\
        LOG_DEBUG("Error, key not found: " + key);\
        throw "Error, key not found: " + key;\
    }\
    if (val.type() == typeid(Y)) {\
        return boost::any_cast<Y>(val);\
    }\
    try {\
        return boost::any_cast<Y>(val);\
    } catch (boost::bad_any_cast &c) {\
        LOG_DEBUG("Error, not a " #X " value at: " + key);\
        return default_value;\
    }\
}

#define getConvertNumberValue(X, Y, A, B, C, D, E) \
Y BliObject::get ## X ## Value(const std::string &key, Y default_value) const\
{\
    BliAny val = getValue(key);\
    if (val.empty()) {\
        LOG_DEBUG("Error, key not found: " + key);\
        throw "Error, key not found: " + key;\
    }\
    if (val.type() == typeid(Y)) {\
        return boost::any_cast<Y>(val);\
    } else {\
        if (val.type() == typeid(A)) {\
            return static_cast<Y>(boost::any_cast<A>(val));\
        }\
        if (val.type() == typeid(B)) {\
            return static_cast<Y>(boost::any_cast<B>(val));\
        }\
        if (val.type() == typeid(C)) {\
            return static_cast<Y>(boost::any_cast<C>(val));\
        }\
        if (val.type() == typeid(D)) {\
            return static_cast<Y>(boost::any_cast<D>(val));\
        }\
        if (val.type() == typeid(E)) {\
            return static_cast<Y>(boost::any_cast<E>(val));\
        }\
    }\
    std::ostringstream m;\
    m << val;\
    LOG_DEBUG("Error, not a " #X " value at: " + key + ", val: " + m.str());\
    return default_value;\
}

// FIXME
getConvertValue(String, std::string)
getConvertNumberValue(
    Int, int, unsigned int, long, unsigned long, double, float)
getConvertNumberValue(
    UInt, unsigned int, unsigned long, long, int, double, float)
getConvertNumberValue(
    Double, double, float, unsigned long, long, unsigned int, int)
getConvertValue(Bool, bool)

std::vector<std::string> BliObject::getListValue(const std::string &key)
{
    BliAny val = getValue(key);
    try {
        return boost::any_cast<std::vector<std::string> >(val);
    }
    catch (boost::bad_any_cast &c) {
        throw std::string("Not string list at " + key);
    }
}

std::vector<int> BliObject::getIntValues(const std::string &key)
{
    BliAny val = getValue(key);
    try {
        return boost::any_cast<std::vector<int> >(val);
    }
    catch (boost::bad_any_cast &c) {
        throw std::string("Not int list at " + key);
    }
}

const std::type_info *BliObject::getValueType(const std::string &key) const
{
    BliAny val = getValue(key);
    if (val.empty()) {
        LOG_DEBUG("Error, key not found: " + key);
        throw "Error, key not found: " + key;
    }
    return &val.type();
}

std::string BliObject::toString() const
{
    std::ostringstream res;

    BOOST_FOREACH(values_t item, values) {
        std::string key = item.first;
        BliAny val = item.second;

        res << key + ": ";
        res << val;
        res << "\n";
    }

    return res.str();
}

std::vector<std::string> BliObject::getKeys()
{
    std::vector<std::string> res;
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(value_mutex);
#endif

    for(values_t val : values) {
        res.push_back(val.first);
    }
    return res;
}

#define CHANGE_NUM_VALUE(KEY, VAL, DIFF, T) do {\
    T __num = boost::any_cast<T>(VAL);\
    __num += DIFF;\
    if (isValue(KEY + "-max")) {\
        T __max = getIntValue(key + "-max");\
        if (__num > __max) return false;\
    }\
    setValue(KEY, __num);\
}  while (0);

bool BliObject::changeNumberValue(const std::string &key, int diff)
{
    if (!isValue(key)) return false;

    BliAny val = getValue(key);

    if (val.type() == typeid(int)) {
        CHANGE_NUM_VALUE(key, val, diff, int);
    }
    else if (val.type() == typeid(unsigned int)) {
        // FIXME possible int overflow!
        CHANGE_NUM_VALUE(key, val, diff, unsigned int);
    }
    else if (val.type() == typeid(double)) {
        CHANGE_NUM_VALUE(key, val, diff, double);
    }
    else {
        return false;
    }

    return true;
}
#undef CHANGE_NUM_VALUE

bool BliObject::increase(const std::string &key)
{
    return changeNumberValue(key, 1);
}

bool BliObject::decrease(const std::string &key)
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

std::string blieng::toString(unsigned int value)
{
    return boost::lexical_cast<std::string>(value);
}

std::string blieng::toString(double value)
{
    return boost::lexical_cast<std::string>(value);
}
