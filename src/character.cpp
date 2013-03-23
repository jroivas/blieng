#include "character.h"

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
	if (val.empty()) return "";
	return boost::any_cast<std::string>(val);
}

int Character::getIntegerValue(std::string key)
{
	boost::any val = getValue(key);
	if (val.empty()) return 0;
	return boost::any_cast<int>(val);
}

double Character::getDoubleValue(std::string key)
{
	boost::any val = getValue(key);
	if (val.empty()) return 0.0;
	return boost::any_cast<double>(val);
}

