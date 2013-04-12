#include "town.h"
#include <boost/foreach.hpp>

using blieng::Town;

Town::Town() : BliObject(), name(""), size(0), xpos(0), ypos(0)
{
	setValue("population", (unsigned int)0);
}

void Town::updatePopulation()
{
	unsigned int pop = 0;
	BOOST_FOREACH(Character *ch, characters) {
		if (!ch->getBoolValue("dead")) ++pop;
	}
	setValue("population", pop);
}

unsigned int Town::getPopulation()
{
	return getUIntValue("population");
}

void Town::setName(std::string town_name)
{
	name = town_name;
}

void Town::setSize(unsigned int town_size)
{
	size = town_size;
}

void Town::setPosition(double x, double y)
{
	xpos = x;
	ypos = y;
}

void Town::setPositionX(double x)
{
	xpos = x;
}

void Town::setPositionY(double y)
{
	ypos = y;
}

void Town::addItem(Item *item)
{
	items.push_back(item);
}

bool Town::removeItem(Item *item)
{
	std::vector<Item *>::iterator iter = items.begin();
	while (iter != items.end()) {
		if (*iter == item) {
			items.erase(iter);
			return true;
		}
	}

	return false;
}

std::vector<blieng::Item *> Town::getItems()
{
	return items;
}

void Town::addCharacter(Character *chr)
{
	characters.push_back(chr);
}

bool Town::removeCharacter(Character *chr)
{
	std::vector<Character *>::iterator iter = characters.begin();
	while (iter != characters.end()) {
		if (*iter == chr) {
			characters.erase(iter);
			return true;
		}
	}

	return false;
}

std::vector<blieng::Character *> Town::getCharacters()
{
	return characters;
}

std::string Town::toString()
{
	std::string res = blieng::BliObject::toString();

	res += "name: " + name + "\n";
	res += (boost::format("size: %u\n") % size).str();
	res += (boost::format("pos : %f, %f\n") % xpos % ypos).str();

	return res;
}
