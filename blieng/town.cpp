#include "town.h"
#include <boost/foreach.hpp>

using blieng::Town;

Town::Town() : BliObject(), name(""), size(0)
{
    pos = blieng::Point(0, 0);
    setValue("population", (unsigned int)0);
}

void Town::updatePopulation()
{
    unsigned int pop = 0;
    BOOST_FOREACH(Character *ch, characters) {
        if (ch->isAlive()) ++pop;
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
    pos.x = x;
    pos.y = y;
}

void Town::setPositionX(double x)
{
    pos.x = x;
}

void Town::setPositionY(double y)
{
    pos.y = y;
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
    updatePopulation();
}

bool Town::removeCharacter(Character *chr)
{
    std::vector<Character *>::iterator iter = characters.begin();
    while (iter != characters.end()) {
        if (*iter == chr) {
            characters.erase(iter);
            updatePopulation();
            return true;
        }
        ++iter;
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
    res += pos.toString() + "\n";
    //res += (boost::format("pos : %f, %f\n") % pos->y % pos->y).str();

    return res;
}

unsigned int Town::getCharacterClassCnt(std::string character_class)
{
    unsigned int zombies = 0;
    BOOST_FOREACH(Character *ch, characters) {
        if (ch->isValue("class") && ch->getStringValue("class") == character_class) {
            if (ch->isAlive()) {
                ++zombies;
            }
        }
    }
    return zombies;
}

std::vector<blieng::Character *> Town::getCharacterClass(std::string character_class)
{
    std::vector<Character *> zombies; 
    BOOST_FOREACH(Character *ch, characters) {
        if (ch->isValue("class") && ch->getStringValue("class") == character_class) {
            zombies.push_back(ch);
        }
    }
    return zombies;
}
