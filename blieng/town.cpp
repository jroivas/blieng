/*
 * Copyright 2014-2017 Jouni Roivas
 */

#include "blieng/town.h"

#include <string>
#include <vector>

using blieng::Town;

Town::Town() : BliObject(), name(""), size(0)
{
    pos = blieng::Point(0, 0);
    setValue("population", static_cast<unsigned int>(0));
}

void Town::updatePopulation()
{
    unsigned int pop = 0;
    for (Character *ch : characters) {
        if (ch->isAlive()) ++pop;
    }
    setValue("population", pop);
}

unsigned int Town::getPopulation() const
{
    return getUIntValue("population");
}

void Town::setName(const std::string &town_name)
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
    auto iter = items.begin();
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
    auto iter = characters.begin();
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

std::vector<blieng::Character *> Town::getCharacters() const
{
    return characters;
}

std::string Town::toString() const
{
    std::string res = blieng::BliObject::toString();

    res += "name: " + name + "\n";
    res += (boost::format("size: %u\n") % size).str();
    res += pos.toString() + "\n";
    // res += (boost::format("pos : %f, %f\n") % pos->y % pos->y).str();

    return res;
}

unsigned int Town::getCharacterClassCnt(const std::string &character_class)
{
    unsigned int character_class_cnt = 0;
    for (Character *ch : characters) {
        if (ch->isValue("class") &&
            ch->getStringValue("class") == character_class) {
            if (ch->isAlive()) {
                ++character_class_cnt;
            }
        }
    }
    return character_class_cnt;
}

std::vector<blieng::Character *> Town::getCharacterClass(
    const std::string &character_class)
{
    std::vector<Character *> character_class_cnt;
    for (Character *ch : characters) {
        if (ch->isValue("class") &&
            ch->getStringValue("class") == character_class) {
            character_class_cnt.push_back(ch);
        }
    }
    return character_class_cnt;
}
