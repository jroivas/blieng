/*
 * Copyright 2014 Blistud:io
 */

#include "blieng/configure.h"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#ifdef DATA_MUTEX_LOCK
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
#endif

#include <string>
#include <utility>
#include <vector>

using blieng::Configure;

typedef std::pair<std::string, Configure::key_type_t> key_values_t;

Configure::Configure(boost::shared_ptr<blieng::Data> _data) :
    BliObject(), data(_data)
{
}

Configure::~Configure()
{
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(key_mutex);
#endif

    keys.erase(keys.begin(), keys.end());
    opt_keys.erase(opt_keys.begin(), opt_keys.end());
}

bool Configure::load(const std::string &_config_file)
{
    std::string fname = data->findFile(_config_file);
    if (fname != "") {
        data_json = data->readJson(fname);
        parse();
        return true;
    }
    return false;
}

void Configure::addKey(const std::string &val, key_type_t key_type)
{
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(key_mutex);
#endif

    keys[val] = key_type;
}

void Configure::addOptionalKey(const std::string &val, key_type_t key_type)
{
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(key_mutex);
#endif

    opt_keys[val] = key_type;
}

bool Configure::validate() const
{
    for (key_values_t key : keys) {
        if (!isValue(key.first)) {
            std::cout << key.first << ": NOT FOUND\n";
            return false;
        }
    }
    return true;
}

bool Configure::validateValues()
{
    for (key_values_t key : keys) {
        if (!isValue(key.first)) return false;

        if (key.second == Configure::KeyString)
            getStringValue(key.first);
        else if (key.second == Configure::KeyDouble)
            getDoubleValue(key.first);
        else if (key.second == Configure::KeyInt)
            getIntValue(key.first);
        else if (key.second == Configure::KeyUInt)
            getUIntValue(key.first);
        else if (key.second == Configure::KeyBool)
            getBoolValue(key.first);
        else if (key.second == Configure::KeyStringList)
            getListValue(key.first);
        else if (key.second == Configure::KeyIntList)
            getIntValues(key.first);
    }
    return true;
}

void Configure::parseStringList(std::string key, const json_value* realval)
{
    if (realval->isArray()) {
        std::vector<std::string> list_data;

        auto it = realval->u.array.begin();
        while (it != realval->u.array.end()) {
            list_data.push_back((*it)->asString());
            ++it;
        }

        setValue(key, list_data);
    }
}

void Configure::parseIntList(std::string key, const json_value* realval)
{
    if (realval->isArray()) {
        std::vector<int> list_data;

        auto it = realval->u.array.begin();
        while (it != realval->u.array.end()) {
            list_data.push_back((*it)->asInt());
            ++it;
        }

        setValue(key, list_data);
    }
}

void Configure::parseBool(std::string key, const json_value* realval)
{
    bool res = false;

    if (realval->isNumeric()) {
        if (realval->asInt() == 1) res = true;
    } else if (realval->isString()) {
        std::string sval = realval->asString();
        boost::algorithm::to_lower(sval);
        if (sval == "yes" || sval == "on" ||
            sval == "true" || sval == "y") {
                res = true;
        }
    }
    setValue(key, res);
}

void Configure::parse()
{
    if (!data_json->isObject()) return;

#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(key_mutex);
#endif

    BOOST_FOREACH(const std::string data_key, data_json->getMemberNames()) {
        auto val = keys.find(data_key);
        auto val2 = opt_keys.find(data_key);
        if (val == keys.end()) val = opt_keys.find(data_key);

        if (val != keys.end() || val2 != opt_keys.end()) {
            const json_value* realval = data->getJsonValue(data_json, data_key);
            if (val->second == Configure::KeyString) {
                if (realval->isString())
                    setValue(data_key, realval->asString());
            }
            else if (val->second == Configure::KeyDouble) {
                if (realval->isNumeric())
                    setValue(data_key, realval->asDouble());
            }
            else if (val->second == Configure::KeyUInt) {
                if (realval->isNumeric())
                    setValue(data_key, realval->asUInt());
            }
            else if (val->second == Configure::KeyInt) {
                if (realval->isNumeric())
                    setValue(data_key, realval->asInt());
            }
            else if (val->second == Configure::KeyBool) {
                parseBool(data_key, realval);
            }
            else if (val->second == Configure::KeyStringList) {
                parseStringList(data_key, realval);
            }
            else if (val->second == Configure::KeyIntList) {
                parseIntList(data_key, realval);
            }
        }
    }
}
