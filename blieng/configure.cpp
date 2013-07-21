#include "configure.h"
#include "data.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

using blieng::Configure;

static Configure *__static_configure = NULL;
typedef std::pair<std::string, Configure::key_type_t> key_values_t;

Configure::Configure() : BliObject()
{
}

Configure *Configure::getInstance()
{
    if (__static_configure == NULL) {
        __static_configure = new Configure(); 
    }
    return __static_configure;
}

Configure *Configure::getInstance(std::string config)
{
    if (__static_configure == NULL) {
        __static_configure = new Configure(); 
        __static_configure->load(config);
    }
    return __static_configure;
}

void Configure::load(std::string config_file)
{
    std::string fname = Data::getInstance()->findFile(config_file);
    if (fname != "") {
        data_json = Data::getInstance()->readJson(fname);
        parse();
    }
}

void Configure::addKey(std::string val, key_type_t key_type)
{
    keys[val] = key_type;
}

void Configure::addOptionalKey(std::string val, key_type_t key_type)
{
    opt_keys[val] = key_type;
}

bool Configure::validate()
{
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
    if (!data_json.isObject()) return;

    BOOST_FOREACH(std::string data_key, data_json.getMemberNames()) {
        std::map<std::string, key_type_t>::iterator val = keys.find(data_key);
        if (val == keys.end()) val = opt_keys.find(data_key);

        if (val != keys.end() || val != opt_keys.end()) {
            Json::Value realval = Data::getInstance()->getJsonValue(data_json, data_key);
            if (val->second == Configure::KeyString) {
                if (realval.isString()) setValue(data_key, realval.asString());
            }
            else if (val->second == Configure::KeyDouble) {
                if (realval.isNumeric()) setValue(data_key, realval.asDouble());
            }
            else if (val->second == Configure::KeyUInt) {
                if (realval.isNumeric()) setValue(data_key, realval.asUInt());
            }
            else if (val->second == Configure::KeyInt) {
                if (realval.isNumeric()) setValue(data_key, realval.asInt());
            }
            else if (val->second == Configure::KeyBool) {
                bool res = false;

                if (realval.isNumeric()) {
                    if (realval.asInt() == 1) res = true;
                } else if (realval.isString()) {
                    std::string sval = realval.asString();
                    boost::algorithm::to_lower(sval);
                    bool res = false;
                    if (sval == "yes" || sval == "on" ||
                        sval == "true" || sval == "y") {
                            res = true;
                    }
                }
                setValue(data_key, res);
            }
        }
    }
}
