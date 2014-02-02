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

    void setName(const std::string &town_name);
    const std::string getName() const { return name; }
    void setSize(unsigned int town_size);
    unsigned int getSize() const { return size; }

    void setPosition(double x, double y);
    void setPositionX(double x);
    void setPositionY(double y);
    inline double getPositionX() const { return pos.x; }
    inline double getPositionY() const { return pos.y; }

    blieng::Point getPosition() const { return pos; }

    //FIXME: Should we handle items as unique_ptr/shared_ptr??
    void addItem(Item *item);
    bool removeItem(Item *item);
    std::vector<Item *> getItems();

    //FIXME: Should we handle characters as unique_ptr/shared_ptr??
    void addCharacter(Character *chr);
    bool removeCharacter(Character *chr);
    std::vector<Character *> getCharacters() const;

    virtual std::string toString() const;

    void updatePopulation();
    unsigned int getPopulation() const;
    unsigned int getCharacterClassCnt(const std::string &character_class);
    std::vector<blieng::Character *> getCharacterClass(const std::string &character_class);

private:
    std::vector<Item *> items;
    std::vector<Character *> characters;
    std::string name;
    unsigned int size;
    blieng::Point pos;
};

}

#endif
