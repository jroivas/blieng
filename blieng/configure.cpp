#include "configure.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#ifdef DATA_MUTEX_LOCK
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
#endif

using blieng::Configure;

typedef std::pair<std::string, Configure::key_type_t> key_values_t;

Configure::Configure(shared_ptr<blieng::Data> _data) : BliObject(), data(_data)
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

bool Configure::validate()
{
#ifdef DATA_MUTEX_LOCK
    boost::lock_guard<boost::mutex> keylock(key_mutex);
#endif
    BOOST_FOREACH(key_values_t key, keys) {
        if (!isValue(key.first)) {
            std::cout << key.first << ": NOT FOUND\n";
            return false;
        }
    }
    return true;
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
                if (realval->isString()) setValue(data_key, realval->asString());
            }
            else if (val->second == Configure::KeyDouble) {
                if (realval->isNumeric()) setValue(data_key, realval->asDouble());
            }
            else if (val->second == Configure::KeyUInt) {
                if (realval->isNumeric()) setValue(data_key, realval->asUInt());
            }
            else if (val->second == Configure::KeyInt) {
                if (realval->isNumeric()) setValue(data_key, realval->asInt());
            }
            else if (val->second == Configure::KeyBool) {
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
                setValue(data_key, res);
            }
            else if (val->second == Configure::KeyStringList) {
                if (realval->isArray()) {
                    std::vector<std::string> list_data;
#if 1
                    auto it = realval->u.array.begin();
                    while (it != realval->u.array.end()) {
                        list_data.push_back((*it)->asString());
                        ++it;
                    }
#else
                    for (unsigned int i=0; i<realval->u.array.length; ++i) {
                        list_data.push_back(realval->u.array.values[i]->asString());
                    }
#endif
                    setValue(data_key, list_data);
                }
            }
            else if (val->second == Configure::KeyIntList) {
                if (realval->isArray()) {
                    std::vector<int> list_data;

                    auto it = realval->u.array.begin();
                    while (it != realval->u.array.end()) {
                        list_data.push_back((*it)->asInt());
                        ++it;
                    }

                    setValue(data_key, list_data);
                }
            }
        }
    }
}
