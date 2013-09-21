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

void Character::addItem(auto_ptr<blieng::Item> item)
//void Character::addItem(Item* item)
{
    items.push_back(item);
}

bool Character::removeItem(const blieng::Item* item)
//bool Character::removeItem(const auto_ptr<blieng::Item> item)
{
    auto_vector<blieng::Item>::iterator ii = items.begin();
    while (ii != items.end()) {
        if (*ii == item) {
            items.erase(ii);
            return true;
        }
        ++ii;
    }
    return false;
}

const auto_vector<blieng::Item>* Character::getItems()
//const std::vector<blieng::Item*> Character::getItems() const
{
    return &items;
}

#if 0
void Character::assignObject(BliObject *another)
{
    blieng::BliObject::assignObject(another);
}
#endif

void Character::assignObject(Character *another)
{
    if (another == NULL) return;
    blieng::BliObject::assignObject(static_cast<BliObject*>(another));
    
    BOOST_FOREACH(blieng::Item *item, another->items) {
        items.push_back(item->copy());
    }
}
