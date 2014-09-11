/*
 * Copyright 2014 Blistud:io
 */

#include "blieng/bliobject.h"

#include <boost/format.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

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
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <limits>
#include <typeinfo>

#include "blieng/data.h"
#include "blieng/logging.h"

using blieng::BliObject;
using blieng::BliAny;

BliObject::BliObject()
{
}

BliObject::~BliObject()
{
    // Enforce deletion
    while (!values.empty()) {
        auto data = values.begin();
        values.erase(data);
    }
}

void BliObject::assignObject(const BliObject *another)
{
    if (another == nullptr)
        return;

#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(value_mutex);
#endif
    for (auto val : another->values) {
        values[val.first] = val.second;
    }
}

bool BliObject::isValue(const std::string &key) const
{
    auto value_iter = values.find(key);

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
    auto value_iter = values.find(key);

    if (value_iter == values.end()) {
        LOG_ERROR("Error, key not found: " + key);
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
        LOG_ERROR("Error, " #X " key not found: " + key);\
        throw "Error, " #X " key not found: " + key;\
    }\
    if (val.type() == typeid(Y)) {\
        return boost::any_cast<Y>(val);\
    }\
    try {\
        return boost::any_cast<Y>(val);\
    } catch (boost::bad_any_cast &c) {\
        LOG_ERROR("Error, not a " #X " value at: " + key);\
        return default_value;\
    }\
}

template<typename A, typename B>
bool BliObject::fitsLimits(BliAny val, A &res) const
{
    if (val.type() != typeid(B))
        return false;

    int a_digits = std::numeric_limits<A>::digits;
    int b_digits = std::numeric_limits<B>::digits;

    int a_is_signed = std::numeric_limits<A>::is_signed;
    int b_is_signed = std::numeric_limits<B>::is_signed;

    // These shouls always fit...
    uint64_t _max = std::numeric_limits<A>::max();
    int64_t _min = std::numeric_limits<A>::min();

    B _val = boost::any_cast<B>(val);
    bool conv = false;
    if (a_digits >= b_digits
        && a_is_signed == b_is_signed) {
        conv = true;
    }
    else if (a_is_signed != b_is_signed) {
        if (b_is_signed && _val >= 0 && a_digits >= b_digits) {
            conv = true;
        }
        else if (a_is_signed && a_digits >= b_digits) {
            conv = true;
        }
        else if (_val >= 0 && _val < _max) {
            conv = true;
        }
    }
    else {
        if (_val >= _min && _val < _max) {
            conv = true;
        }
    }

    if (conv) {
        res = static_cast<A>(boost::any_cast<B>(val));
        return true;
    }
    return false;
}

template<typename T>
T BliObject::getValue(const std::string &key) const
{
    BliAny val = getValue(key);
    if (val.empty()) {
        LOG_ERROR("Error, key not found: " + key);
        throw "Error, key not found: " + key;
    }

    if (val.type() == typeid(T))
        return boost::any_cast<T>(val);

    T res = 0;
    if (fitsLimits<T, int>(val, res))
        return res;
    if (fitsLimits<T, unsigned int>(val, res))
        return res;
    if (fitsLimits<T, long>(val, res))
        return res;
    if (fitsLimits<T, unsigned long>(val, res))
        return res;
    if (fitsLimits<T, long long>(val, res))
        return res;
    if (fitsLimits<T, unsigned long long>(val, res))
        return res;
    if (fitsLimits<T, char>(val, res))
        return res;
    if (fitsLimits<T, unsigned char>(val, res))
        return res;
    if (fitsLimits<T, short>(val, res))
        return res;
    if (fitsLimits<T, unsigned short>(val, res))
        return res;
    if (fitsLimits<T, double>(val, res))
        return res;
    if (fitsLimits<T, float>(val, res))
        return res;

    std::ostringstream msg;
    msg << "Unsafe conversion of "
        << key
        << " from typeid("
        << val.type().name()
        << ")"
        << " to typeid("
        << typeid(T).name()
        << ")";
    LOG_ERROR(msg.str());
    throw msg.str();
}

int BliObject::getIntValue(const std::string &key) const
{
    return getValue<int>(key);
}

char BliObject::getCharValue(const std::string &key) const
{
    return getValue<char>(key);
}

unsigned int BliObject::getUIntValue(const std::string &key) const
{
    return getValue<unsigned int>(key);
}

double BliObject::getDoubleValue(const std::string &key) const
{
    return getValue<double>(key);
}

std::string BliObject::getStringValue(const std::string &key) const
{
    BliAny val = getValue(key);
    try {
        return boost::any_cast<std::string>(val);
    }
    catch (boost::bad_any_cast &c) {}
    try {
        return boost::any_cast<char *>(val);
    }
    catch (boost::bad_any_cast &c) {}

    std::ostringstream msg;
    msg << "Unsafe conversion of "
        << key
        << " from typeid("
        << val.type().name()
        << ")"
        << " to string";
    LOG_ERROR(msg.str());
    throw msg.str();
}

bool BliObject::getBoolValue(const std::string &key) const
{
    BliAny val = getValue(key);
    try {
        return boost::any_cast<bool>(val);
    }
    catch (boost::bad_any_cast &c) {}

    std::ostringstream msg;
    msg << "Unsafe conversion of "
        << key
        << " from typeid("
        << val.type().name()
        << ")"
        << " to bool";
    LOG_ERROR(msg.str());
    throw msg.str();
}

std::vector<std::string> BliObject::getListValue(const std::string &key) const
{
    BliAny val = getValue(key);
    try {
        return boost::any_cast<std::vector<std::string> >(val);
    }
    catch (boost::bad_any_cast &c) {
        throw std::string("Not string list at " + key);
    }
}

std::vector<int> BliObject::getIntValues(const std::string &key) const
{
    BliAny val = getValue(key);
    try {
        return boost::any_cast<std::vector<int> >(val);
    }
    catch (boost::bad_any_cast &c) {
        std::string errmsg = "Not int list at " + key + " " + c.what();
        std::cerr << errmsg << std::endl;
        throw errmsg;
    }
}

const std::type_info *BliObject::getValueType(const std::string &key) const
{
    BliAny val = getValue(key);
    if (val.empty()) {
        LOG_ERROR("Error, key not found: " + key);
        throw "Error, key not found: " + key;
    }
    return &val.type();
}

std::string BliObject::toString() const
{
    std::ostringstream res;

    for (auto item : values) {
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

    for (auto val : values) {
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
    if (!isValue(key))
        return false;

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

std::string blieng::toString(int value)
{
    return boost::lexical_cast<std::string>(value);
}

std::string blieng::toString(unsigned int value)
{
    return boost::lexical_cast<std::string>(value);
}

std::string blieng::toString(double value)
{
    return boost::lexical_cast<std::string>(value);
}

std::string blieng::percentageString(
    double value,
    unsigned int digits)
{
    int percentage = static_cast<int>(value * 100);
    std::string val = blieng::toString(percentage);
    if (digits > 0) {
        double norm = fabs(value * 100) - fabs(percentage);
        std::string rest = blieng::toString(norm);
        val += rest.substr(1, digits + 1);
    }
    val += "%";
    return val;
}


template<typename T>
void
serializeObject(boost::archive::binary_oarchive &arch, T obj)
{
    arch << obj;
}

template<typename T>
T deserializeObject(boost::archive::binary_iarchive &arch)
{
    T obj;
    arch >> obj;
    return obj;
}

std::string BliObject::serialize(std::string type) const
{
    std::ostringstream ss;

    boost::archive::binary_oarchive arch(ss);
    if (type == "")
        type = "BliObject";
    arch << type;
    serializeObject<unsigned int>(arch, values.size());

    for (auto val : values) {
        arch << val.first;

        BliAny any = val.second;
        std::string types = any.typeString();
        arch << types;

        if (types == "int")
            serializeObject<int>(arch, any.asInt());
        else if (types == "unsigned int")
            serializeObject<unsigned int>(arch, any.asUInt());
        else if (types == "long")
            serializeObject<long>(arch, any.asLong());
        else if (types == "unsigned long")
            serializeObject<unsigned long>(arch, any.asULong());
        else if (types == "char")
            serializeObject<char>(arch, any.asChar());
        else if (types == "unsigned char")
            serializeObject<unsigned char>(arch, any.asUChar());
        else if (types == "short")
            serializeObject<short>(arch, any.asShort());
        else if (types == "unsigned short")
            serializeObject<unsigned short>(arch, any.asUShort());
        else if (types == "long long")
            serializeObject<long long>(arch, any.asLongLong());
        else if (types == "unsigned long long")
            serializeObject<unsigned long long>(arch, any.asULongLong());
        else if (types == "string")
            serializeObject<std::string>(arch, any.asString());
        else if (types == "float")
            serializeObject<float>(arch, any.asFloat());
        else if (types == "double")
            serializeObject<double>(arch, any.asDouble());
        /*else
            throw std::string("Invalid value for key '" + types + "'");
        */
    }

    return ss.str();
}

bool BliObject::deserialize(
    std::string data,
    std::string type)
{
    std::istringstream ss(data);

    boost::archive::binary_iarchive arch(ss);
    if (type == "")
        type = "BliObject";
    std::string data_type = deserializeObject<std::string>(arch);
    if (data_type != type)
        return false;

    unsigned int num_values = deserializeObject<unsigned int>(arch);
    unsigned int index = 0;
    while (index < num_values) {
        std::string key_name = deserializeObject<std::string>(arch);
        std::string types = deserializeObject<std::string>(arch);
        if (types == "int")
            setValue(key_name, deserializeObject<int>(arch));
        else if (types == "unsigned int")
            setValue(key_name, deserializeObject<unsigned int>(arch));
        else if (types == "long")
            setValue(key_name, deserializeObject<long>(arch));
        else if (types == "unsigned long")
            setValue(key_name, deserializeObject<unsigned long>(arch));
        else if (types == "char")
            setValue(key_name, deserializeObject<char>(arch));
        else if (types == "unsigned char")
            setValue(key_name, deserializeObject<unsigned char>(arch));
        else if (types == "short")
            setValue(key_name, deserializeObject<short>(arch));
        else if (types == "unsigned short")
            setValue(key_name, deserializeObject<unsigned short>(arch));
        else if (types == "long long")
            setValue(key_name, deserializeObject<long long>(arch));
        else if (types == "unsigned long long")
            setValue(key_name, deserializeObject<unsigned long long>(arch));
        else if (types == "string")
            setValue(key_name, deserializeObject<std::string>(arch));
        else if (types == "float")
            setValue(key_name, deserializeObject<float>(arch));
        else if (types == "double")
            setValue(key_name, deserializeObject<double>(arch));
        ++index;
    }

    return true;
}
