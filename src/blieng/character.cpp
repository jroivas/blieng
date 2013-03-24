#include "character.h"
#include <iostream>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

using blieng::Character;

typedef std::pair<std::string, boost::any> values_t;
typedef std::map<std::string, boost::any>::iterator values_iter_t;

Character::Character()
{
	gen = new boost::random::random_device();
}

bool Character::isValue(std::string key)
{
	values_iter_t value_iter = values.find(key);
	if (value_iter == values.end()) return false;
	return true;
}

int Character::getRandomInt(int limit_low, int limit_max)
{
	boost::random::uniform_int_distribution<> dist(limit_low, limit_max);
	return dist(*gen);
}

double Character::getRandomDouble(double limit_low, double limit_max)
{
	boost::random::uniform_real_distribution<> dist(limit_low, limit_max);
	return dist(*gen);
}

boost::any Character::getValue(std::string key)
{
	values_iter_t value_iter = values.find(key);
	if (value_iter == values.end()) {
		std::cout << "Error, key not found: " + key + "\n";
		throw "Error, key not found: " + key;
	}
	return value_iter->second;
}

void Character::setValue(std::string key, boost::any value)
{
	values[key] = value;
}

std::string Character::getStringValue(std::string key)
{
	boost::any val = getValue(key);
	if (val.empty()) {
		std::cout << "Error, key not found: " + key + "\n";
		throw "Error, key not found: " + key;
	}
	try {
		return boost::any_cast<std::string>(val);
	} catch (boost::bad_any_cast c) {
		throw "Error, not a string value at: " + key;
	}
}

int Character::getIntValue(std::string key)
{
	boost::any val = getValue(key);
	if (val.empty()) {
		std::cout << "Error, key not found: " + key + "\n";
		throw "Error, key not found: " + key;
	}
	try {
		return boost::any_cast<int>(val);
	} catch (boost::bad_any_cast c) {
		throw "Error, not a int value at: " + key;
	}
}

double Character::getDoubleValue(std::string key)
{
	boost::any val = getValue(key);
	if (val.empty()) return 0.0;
	try {
		return boost::any_cast<double>(val);
	} catch (boost::bad_any_cast c) {
		throw "Error, not a double value at: " + key;
	}
}

std::string Character::toString()
{
	std::string res = "";
	BOOST_FOREACH(values_t item, values) {
		std::string key = item.first;
		boost::any val = item.second;

		res += (boost::format("%20s: ") % key).str();

		if (val.type() == typeid(int)) {
			res += (boost::format("%d") % boost::any_cast<int>(val)).str();
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

std::list<std::string> Character::getKeys()
{
	std::list<std::string> res;
	BOOST_FOREACH(values_t val, values) {
		res.push_back(val.first);
	}
	return res;
}

bool Character::changeIntValue(std::string key, int diff)
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

bool Character::increase(std::string key)
{
	return changeIntValue(key, 1);
}

bool Character::decrease(std::string key)
{
	return changeIntValue(key, -1);
}
