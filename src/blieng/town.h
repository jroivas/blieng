#ifndef __TOWN_H
#define __TOWN_H

#include "bliobject.h"
#include "item.h"
#include "character.h"
#include "path.h"

/* Represents a Town or Node where resources can gather
 */

namespace blieng
{

class Town : public BliObject
{
public:
	Town();
	virtual ~Town() {}
	
	void setName(std::string town_name);
	const std::string getName() { return name; }
	void setSize(unsigned int town_size);
	unsigned int getSize() { return size; }

	void setPosition(double x, double y);
	void setPositionX(double x);
	void setPositionY(double y);
	#if 0
	inline double getPositionX() { return xpos; }
	inline double getPositionY() { return ypos; }
	#endif
	inline double getPositionX() { return pos.x; }
	inline double getPositionY() { return pos.y; }

	blieng::Point getPosition() {
		return pos;
	}

	void addItem(Item *item);
	bool removeItem(Item *item);
	std::vector<Item *> getItems();

	void addCharacter(Character *chr);
	bool removeCharacter(Character *chr);
	std::vector<Character *> getCharacters();

	virtual std::string toString();

	void updatePopulation();
	unsigned int getPopulation();
	unsigned int getCharacterClassCnt(std::string character_class);
	std::vector<blieng::Character *> getCharacterClass(std::string character_class);

	void convertToZombies(double rate);

private:
	std::vector<Item *> items;
	std::vector<Character *> characters;
	std::string name;
	unsigned int size;
	/*double xpos;
	double ypos;
	*/
	blieng::Point pos;
};

}

#endif
