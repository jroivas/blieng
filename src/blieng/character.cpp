#include "character.h"

using blieng::Character;

Character::Character() : blieng::BliObject()
{
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
	std::cout << "OBJ\n";
}

void Character::assignObject(Character *another)
{
	if (another == NULL) return;
	std::cout << "CHR\n";
	blieng::BliObject::assignObject((BliObject*)another);
	items = another->items;
}
