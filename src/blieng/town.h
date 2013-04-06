#ifndef __TOWN_H
#define __TOWN_H

#include "bliobject.h"
#include "item.h"
#include "character.h"

namespace blieng
{

class Town : public BliObject
{
public:
	Town();

	void addItem(Item *item);
	bool removeItem(Item *item);
	std::vector<Item *> getItems();

	void addCharacter(Character *chr);
	bool removeCharacter(Character *chr);
	std::vector<Character *> getCharacters();

private:
	std::vector<Item *> items;
	std::vector<Character *> characters;
};

}

#endif
