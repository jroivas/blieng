#include "character.h"
#include <iostream>
#include <boost/format.hpp>

Character::Character()
{
}

boost::any Character::getValue(std::string key)
{
	return values[key];
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
	return boost::any_cast<std::string>(val);
}

int Character::getIntValue(std::string key)
{
	boost::any val = getValue(key);
	if (val.empty()) {
		std::cout << "Error, key not found: " + key + "\n";
		throw "Error, key not found: " + key;
	}
	return boost::any_cast<int>(val);
}

double Character::getDoubleValue(std::string key)
{
	boost::any val = getValue(key);
	if (val.empty()) return 0.0;
	return boost::any_cast<double>(val);
}

std::string Character::toString()
{
	std::string res = "";
	std::map<std::string, boost::any>::iterator vi = values.begin();
	while (vi != values.end()) {
		std::string key = vi->first;
		boost::any val = vi->second;

		res += (boost::format("%20s: ") % key).str();

		if (val.type() == typeid(int)) {
			res += (boost::format("%d") % boost::any_cast<int>(val)).str();
		}
		else if (val.type() == typeid(std::string)) {
			res += boost::any_cast<std::string>(val);
		}
		else {
			res += "<unknown type>";
		}
		res += "\n";

		vi++;
	}

	return res;
}
