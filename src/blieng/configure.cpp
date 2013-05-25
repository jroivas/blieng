#include "configure.h"
#include "data.h"
#include <boost/foreach.hpp>

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

void Configure::load(std::string config_file)
{
	std::string fname = Data::getInstance()->findFile(config_file);
	std::cout << fname << "\n";
	if (fname != "") {
		data_json = Data::getInstance()->readJson(fname);
		parse();
	}
}

void Configure::addKey(std::string val, key_type_t key_type)
{
	keys[val] = key_type;
}

bool Configure::validate()
{
	BOOST_FOREACH(key_values_t key, keys) {
		std::cout << key.first << " ";
		if (!isValue(key.first)) {
			std::cout << "fail\n";
			return false;
		}
		std::cout << "ok\n";
	}
	return true;
}

void Configure::parse()
{
	if (!data_json.isObject()) return;

	BOOST_FOREACH(std::string data_key, data_json.getMemberNames()) {
		std::cout <<  data_key << "\n";
		std::map<std::string, key_type_t>::iterator val = keys.find(data_key);
		if (val != keys.end()) {
			Json::Value realval = Data::getInstance()->getJsonValue(data_json, data_key);
			std::cout << "found" << "\n";
			if (val->second == Configure::KeyString) {
				std::cout << "a" << "\n";
				if (realval.isString()) setValue(data_key, realval.asString());
			}
			else if (val->second == Configure::KeyDouble) {
				std::cout << "b" << "\n";
				if (realval.isNumeric()) setValue(data_key, realval.asDouble());
			}
			else if (val->second == Configure::KeyUInt) {
				std::cout << "c" << "\n";
				if (realval.isNumeric()) setValue(data_key, realval.asUInt());
			}
			else if (val->second == Configure::KeyInt) {
				std::cout << "d" << "\n";
				if (realval.isNumeric()) setValue(data_key, realval.asInt());
			}
		}
		/*if (data_key == "random_freq" || data_key == "random_prob") {
			Json::Value val = Data::getInstance()->getJsonValue(data_json, data_key);
			if (val.isNumeric()) {
				setValue(data_key, val.asUInt());
			}
		}
		else if (data_key == "bite_prob" || data_key == "bite_threshold" ||
			 data_key == "zombie_limiter") {
			Json::Value val = Data::getInstance()->getJsonValue(data_json, data_key);
			if (val.isNumeric()) {
				setValue(data_key, val.asDouble());
			}
		}
		*/
	}
}
