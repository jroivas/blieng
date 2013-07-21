#ifndef __BLIENG_TOWN_H
#define __BLIENG_TOWN_H

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
    const std::string getName() const { return name; }
    void setSize(unsigned int town_size);
    unsigned int getSize() const { return size; }

    void setPosition(double x, double y);
    void setPositionX(double x);
    void setPositionY(double y);
    inline double getPositionX() const { return pos.x; }
    inline double getPositionY() const { return pos.y; }

    blieng::Point getPosition() const { return pos; }

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
    blieng::Point pos;
};

}

#endif
