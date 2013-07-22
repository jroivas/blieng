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

void Character::addItem(blieng::Item *item)
{
    items.push_back(item);
}

bool Character::removeItem(blieng::Item *item)
{
    std::vector<blieng::Item *>::iterator ii = items.begin();
    while (ii != items.end()) {
        if (*ii == item) {
            items.erase(ii);
            return true;
        }
        ++ii;
    }
    return false;
}

std::vector<blieng::Item *> Character::getItems() const
{
    return items;
}

void Character::assignObject(BliObject *another)
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
