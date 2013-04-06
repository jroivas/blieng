#include "town.h"

using blieng::Town;

Town::Town() : BliObject()
{
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
