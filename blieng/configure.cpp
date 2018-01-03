/*
 * Copyright 2014-2017 Jouni Roivas
 */

#include "blieng/configure.h"

#include <boost/algorithm/string.hpp>

#ifdef DATA_MUTEX_LOCK
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
#endif
#include <boost/assert.hpp>

#include <string>
#include <utility>
#include <vector>

using blieng::Configure;

typedef std::pair<std::string, Configure::key_type_t> key_values_t;

Configure::Configure(
    boost::shared_ptr<blieng::BliengState> _state)
    : BliObject(),
    m_state(_state)
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

bool Configure::load(
    const std::string &_config_file)
{
    std::string fname = m_state->m_data->findFile(_config_file);
    if (fname != "") {
        data_json = m_state->m_data->readJson(fname);
        parse();
        return true;
    }
    return false;
}

void Configure::addKey(
    const std::string &val,
    key_type_t key_type)
{
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(key_mutex);
#endif

    keys[val] = key_type;
}

void Configure::addOptionalKey(
    const std::string &val,
    key_type_t key_type)
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

bool Configure::validateValues() const
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

void Configure::parseStringList(std::string key, const BliengJson realval)
{
    if (realval.is_array()) {
        std::vector<std::string> list_data;

        for (std::string v : realval) {
            list_data.push_back(v);
        }

        setValue(key, list_data);
    }
}

void Configure::parseIntList(std::string key, const BliengJson realval)
{
    if (realval.is_array()) {
        std::vector<int> list_data;

        for (int v : realval) {
            list_data.push_back(v);
        }

        setValue(key, list_data);
    }
}

void Configure::parseBool(std::string key, const BliengJson realval)
{
    bool res = false;

    if (realval.is_boolean()) {
        res = realval;
    } else if (realval.is_number()) {
        int v = realval;
        res = (v == 1);
    } else if (realval.is_string()) {
        std::string sval = realval;
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
    if (!data_json.is_object()) return;

#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(key_mutex);
#endif

    // FIXME This is ugly
    for (auto data_key : JsonKeys(data_json)) {
        auto val = keys.find(data_key);
        auto val2 = opt_keys.find(data_key);
        if (val == keys.end()) val = opt_keys.find(data_key);

        if (val != keys.end() || val2 != opt_keys.end()) {
            const BliengJson realval = data_json[data_key];
            if (realval == nullptr) {}
            else if (val->second == Configure::KeyString) {
                if (realval.is_string()) {
                    std::string v = realval;
                    setValue(data_key, v);
                }
            }
            else if (val->second == Configure::KeyDouble) {
                if (realval.is_number_float()) {
                    double v = realval;
                    setValue(data_key, v);
                }
            }
            else if (val->second == Configure::KeyUInt) {
                if (realval.is_number()) {
                    unsigned int v = realval;
                    setValue(data_key, v);
                }
            }
            else if (val->second == Configure::KeyInt) {
                if (realval.is_number()) {
                    int v = realval;
                    setValue(data_key, v);
                }
            }
            else if (val->second == Configure::KeyBool) {
                if (realval.is_boolean()) {
                    bool v = realval;
                    setValue(data_key, v);
                }
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
