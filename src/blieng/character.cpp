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
	blieng::BliObject::assignObject((BliObject*)another);
	items = another->items;
}
