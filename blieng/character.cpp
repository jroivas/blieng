#include "character.h"
#include <boost/foreach.hpp>

using blieng::Character;

Character::Character() : blieng::BliObject()
{
    setValue("dead", false);
}

bool Character::kill()
{
    setValue("dead", true);
    return true;
}

bool Character::isAlive()
{
    if (!isValue("dead")) return true;
    return !getBoolValue("dead");
}

unsigned int Character::addItem(std::unique_ptr<blieng::Item> item)
{
    items.push_back(std::move(item));
    return items.size();
}

bool Character::removeItem(unsigned int index)
{
    if ( index >= size() ) return false;

    items.erase(index);
    return true;
}

unsigned int Character::size()
{
    return items.size();
}

void Character::assignObject(const BliObject *another)
{
    blieng::BliObject::assignObject(another);
}

void Character::assignObject(Character *another)
{
    if (another == NULL) return;

    blieng::BliObject::assignObject(static_cast<BliObject*>(another));

    BOOST_FOREACH(blieng::Item *item, another->items) {
        items.push_back(item->copy());
    }
}
