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
	return boost::any_cast<std::string>(getValue(key));
}

int Character::getIntegerValue(std::string key)
{
	return boost::any_cast<int>(getValue(key));
}
