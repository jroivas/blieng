#ifndef __TOWN_H
#define __TOWN_H

#include "bliobject.h"
#include "item.h"
#include "character.h"

/* Represents a Town or Node where resources can gather
 */

namespace blieng
{

class Town : public BliObject
{
public:
	Town();
	
	void setName(std::string town_name);
	void setSize(unsigned int town_size);
	void setPosition(double x, double y);

	void addItem(Item *item);
	bool removeItem(Item *item);
	std::vector<Item *> getItems();

	void addCharacter(Character *chr);
	bool removeCharacter(Character *chr);
	std::vector<Character *> getCharacters();

private:
	std::vector<Item *> items;
	std::vector<Character *> characters;
	std::string name;
	unsigned int size;
	double xpos;
	double ypos;
};

}

#endif
