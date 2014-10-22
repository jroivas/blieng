/*
 * Copyright 2014 Blistud:io
 */

#include "blieng/character.h"

#include <boost/foreach.hpp>

using blieng::Character;

Character::Character()
    : blieng::BliObject()
{
    setValue("dead", false);
}

bool Character::kill()
{
    setValue("dead", true);
    return true;
}

bool Character::isAlive() const
{
    if (!isValue("dead"))
        throw std::string("Object does not have value 'dead'");
        //return true;

    return !getBoolValue("dead");
}

unsigned int Character::addItem(
    std::unique_ptr<blieng::Item> item)
{
    items.push_back(std::move(item));
    return items.size();
}

bool Character::removeItem(
    unsigned int index)
{
    if (index >= size())
        throw std::string("Index out of bounds");
        //return false;

    items.erase(index);
    return true;
}

unsigned int Character::size() const
{
    return items.size();
}

void Character::assignObject(
    const BliObject *another)
{
    blieng::BliObject::assignObject(another);
}

void Character::assignObject(
    Character *another)
{
    if (another == nullptr)
        return;

    blieng::BliObject::assignObject(static_cast<BliObject*>(another));

    for (blieng::Item *item : another->items) {
        items.push_back(item->copy());
    }
}
