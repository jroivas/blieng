/*
 * Copyright 2014-2018 Jouni Roivas
 */

#include "blieng/bliobject.h"

#include <boost/format.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

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

#ifdef PSEUDO_RANDOM
static std::time_t now = std::time(0);
boost::random::mt19937 BliObject::m_rand_gen{static_cast<std::uint32_t>(now)};
#else
boost::random::random_device BliObject::m_rand_gen;
#endif

BliObject::BliObject()
{
    m_uuid = genUUID();
}

BliObject::~BliObject()
{
    // Enforce deletion
    while (!m_values.empty()) {
        auto data = m_values.begin();
        m_values.erase(data);
    }
    m_uuid = "";
}

void BliObject::assignObject(const BliObject *another)
{
    if (another == nullptr) return;

#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(m_value_mutex);
#endif
    for (auto val : another->m_values) {
        m_values[val.first] = val.second;
    }
}

bool BliObject::isValue(const std::string &key) const
{
    auto value_iter = m_values.find(key);
    return (value_iter != m_values.end());
}

int BliObject::getRandomInt(int limit_low, int limit_max)
{
    boost::random::uniform_int_distribution<> dist(limit_low, limit_max);
    return dist(m_rand_gen);
}

double BliObject::getRandomDouble(double limit_low, double limit_max)
{
    boost::random::uniform_real_distribution<> dist(limit_low, limit_max);
    return dist(m_rand_gen);
}

bool BliObject::getRandomBoolean()
{
    return getRandomInt(0, 420000) % 2;
}

BliAny BliObject::getValue(const std::string &key) const
{
    auto value_iter = m_values.find(key);

    if (value_iter == m_values.end()) {
        std::string err = "Error, key not found: " + key;
        LOG_ERROR(err);
        throw std::string(err);
    }

    return value_iter->second;
}

void BliObject::setValue(const std::string &key, BliAny value)
{
    {
#ifdef DATA_MUTEX_LOCK
        boost::lock_guard<boost::mutex> keylock(m_value_mutex);
#endif

        m_values[key] = value;
    }
    valuesUpdated(key);
}

void BliObject::valuesUpdated(const std::string &)
{
}

template<typename A, typename B>
bool BliObject::fitsLimits(BliAny val, A &res) const
{
    if (val.type() != typeid(B)) {
        return false;
    }

    int a_digits = std::numeric_limits<A>::digits;
    int b_digits = std::numeric_limits<B>::digits;

    int a_is_signed = std::numeric_limits<A>::is_signed;
    int b_is_signed = std::numeric_limits<B>::is_signed;

    // These should always fit...
    uint64_t _max = static_cast<uint64_t>(std::numeric_limits<A>::max());
    int64_t _min = std::numeric_limits<A>::min();

    B _val = val.cast<B>();
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
        else if (_val >= 0 && static_cast<uint64_t>(_val) < _max) {
            conv = true;
        }
    }
    else {
        if (_min >=0
            && static_cast<uint64_t>(_val) >= static_cast<uint64_t>(_min)
            && static_cast<uint64_t>(_val) < _max) {
            conv = true;
        }
        if (_min <0
            && static_cast<int64_t>(_val) >= static_cast<int64_t>(_min)
            && static_cast<int64_t>(_val) < static_cast<int64_t>(_max)) {
            conv = true;
        }
    }

    if (conv) {
        res = static_cast<A>(val.cast<B>());
        return true;
    }
    return false;
}

template<typename T>
T BliObject::getValue(const std::string &key) const
{
    BliAny val = getValue(key);
    if (val.empty()) {
        std::string err = "Error, key not found: " + key;
        LOG_ERROR(err);
        throw err;
    }

    if (val.type() == typeid(T)) return val.cast<T>();

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

long long BliObject::getLongLongValue(const std::string &key) const
{
    return getValue<long long>(key);
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
        return val.cast<std::string>();
    }
    catch (cdiggins::anyimpl::bad_any_cast &c) {}
    try {
        return val.cast<char*>();
    }
    catch (cdiggins::anyimpl::bad_any_cast &c) {}

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
        return val.cast<bool>();
    }
    catch (cdiggins::anyimpl::bad_any_cast &c) {}

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
        return val.cast<std::vector<std::string>>();
    }
    catch (cdiggins::anyimpl::bad_any_cast &c) {
        throw std::string("Not string list at " + key);
    }
}

std::vector<int> BliObject::getIntValues(const std::string &key) const
{
    BliAny val = getValue(key);
    try {
        return val.cast<std::vector<int>>();
    }
    catch (cdiggins::anyimpl::bad_any_cast &c) {
        throw std::string("Not int list at " + key);
    }
}

const std::type_info *BliObject::getValueType(const std::string &key) const
{
    BliAny val = getValue(key);
    if (val.empty()) {
        std::string err = "Error, key not found: " + key;
        LOG_ERROR(err);
        throw err;
    }
    return &val.type();
}

std::string BliObject::toString() const
{
    std::ostringstream res;

    for (auto item : m_values) {
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
    boost::lock_guard<boost::mutex> keylock(m_value_mutex);
#endif

    for (auto val : m_values) {
        res.push_back(val.first);
    }
    return res;
}

template<typename T>
bool BliObject::changeNumValue(std::string key, BliAny val, int diff)
{
    T __num = val.cast<T>();
    __num += diff;
    if (isValue(key + "-max")) {
        T __max = getIntValue(key + "-max");
        if (__num > __max)
            return false;
    }
    setValue(key, __num);
    return true;
}

bool BliObject::changeNumberValue(const std::string &key, int diff)
{
    if (!isValue(key))
        return false;

    BliAny val = getValue(key);

    // FIXME possible int overflows on multiple cases!
    if (val.type() == typeid(int)) {
        return changeNumValue<int>(key, val, diff);
    }
    else if (val.type() == typeid(unsigned int)) {
        return changeNumValue<unsigned int>(key, val, diff);
    }
    else if (val.type() == typeid(long)) {
        return changeNumValue<long>(key, val, diff);
    }
    else if (val.type() == typeid(unsigned long)) {
        return changeNumValue<unsigned long>(key, val, diff);
    }
    else if (val.type() == typeid(long long)) {
        return changeNumValue<long long>(key, val, diff);
    }
    else if (val.type() == typeid(unsigned long long)) {
        return changeNumValue<unsigned long long>(key, val, diff);
    }
    else if (val.type() == typeid(short)) {
        return changeNumValue<short>(key, val, diff);
    }
    else if (val.type() == typeid(unsigned short)) {
        return changeNumValue<unsigned short>(key, val, diff);
    }
    else if (val.type() == typeid(char)) {
        return changeNumValue<char>(key, val, diff);
    }
    else if (val.type() == typeid(unsigned char)) {
        return changeNumValue<unsigned char>(key, val, diff);
    }
    else if (val.type() == typeid(double)) {
        return changeNumValue<double>(key, val, diff);
    }
    else if (val.type() == typeid(float)) {
        return changeNumValue<float>(key, val, diff);
    }

    throw std::string("Not any known numerical value at: " + key);
}

bool BliObject::increase(const std::string &key)
{
    return changeNumberValue(key, 1);
}

bool BliObject::decrease(const std::string &key)
{
    return changeNumberValue(key, -1);
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

std::string blieng::percentageString(double value, unsigned int digits)
{
    int percentage = static_cast<int>(value * 100);
    std::string val = blieng::toString(percentage);
    if (digits > 0) {
        double norm = std::fabs(value * 100) - std::fabs(percentage);
        std::string rest = blieng::toString(norm);
        val += rest.substr(1, digits + 1);
    }
    val += "%";
    return val;
}


template<typename T>
void serializeObject(boost::archive::text_oarchive &arch, T obj)
{
    arch << obj;
}

template<typename T>
T deserializeObject(boost::archive::text_iarchive &arch)
{
    T obj;
    arch >> obj;
    return obj;
}

std::string BliObject::serialize(std::string type) const
{
    std::ostringstream ss;

    boost::archive::text_oarchive arch(ss);
    if (type == "") type = "BliObject";
    arch << type;

    serializeObject<unsigned int>(arch, m_values.size());

    for (auto val : m_values) {
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
        else if (types == "bool")
            serializeObject<bool>(arch, any.asBool());
        else
            throw std::string("Invalid value for key '" + val.first + "': " + types);
    }

    return ss.str();
}

std::string BliObject::serializedType(const std::string &data)
{
    std::istringstream ss(data);
    boost::archive::text_iarchive arch(ss);

    return deserializeObject<std::string>(arch);
}

bool BliObject::deserialize(const std::string &data, std::string type)
{
    std::istringstream ss(data);
    LOG_DEBUG("Deserializing data");

    boost::archive::text_iarchive arch(ss);
    LOG_DEBUG("Deserializing archive");
    if (type == "") {
        type = "BliObject";
    }
    LOG_DEBUG("Deserializing type");
    std::string data_type = deserializeObject<std::string>(arch);
    if (data_type != type) {
        return false;
    }

    LOG_DEBUG("Deserializing item count");
    unsigned int num_values = deserializeObject<unsigned int>(arch);
    unsigned int index = 0;
    while (index < num_values) {
        LOG_DEBUG("Deserializing name");
        std::string key_name = deserializeObject<std::string>(arch);
        LOG_DEBUG("Deserializing " + key_name);
        std::string types = deserializeObject<std::string>(arch);
        LOG_DEBUG("Type of " + key_name + ": " + types);
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
        else if (types == "bool")
            setValue(key_name, deserializeObject<bool>(arch));
        else
            throw std::string("Invalid value for key '" + key_name + "': " + types);
        ++index;
    }

    return true;
}

std::string BliObject::genUUID() const
{
#ifdef PSEUDO_RANDOM
    boost::uuids::basic_random_generator<boost::mt19937> gen;
#else
    boost::uuids::basic_random_generator<boost::random_device> gen(&m_rand_gen);
#endif
    boost::uuids::uuid gen_uuid = gen();
    std::ostringstream ss;

    ss << gen_uuid;
    return ss.str();
}

BliObject &BliObject::operator=(const BliObject *other)
{
    assignObject(other);
    return *this;
}
