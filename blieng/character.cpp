#include "character.h"

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
}

blieng::Item *Character::removeItem(blieng::Item *item)
{
    return item;
}

std::vector<blieng::Item *> Character::getItems()
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
    items = another->items;
}
