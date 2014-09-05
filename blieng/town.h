/*
 * Copyright 2014 Blistud:io
 */

#ifndef __BLIENG_TOWN_H
#define __BLIENG_TOWN_H

#include <string>
#include <vector>

#include "blieng/bliobject.h"
#include "blieng/item.h"
#include "blieng/character.h"
#include "blieng/path.h"

/* Represents a Town or Node where resources can gather
 */

namespace blieng
{

/**
 * Town may contain characters and items,
 * it has location and name.
 */
class Town : public BliObject
{
public:
    /**
     * Initialize empty town in location (0,0)
     */
    Town();
    virtual ~Town() {}

    /**
     * Set a name for the town.
     *
     * \param town_name New name of the town
     */
    void setName(const std::string &town_name);
    /**
     * Get name of the town.
     *
     * \return Name of the town
     */
    const std::string getName() const
    {
        return name;
    }

    /**
     * Set town size.
     * Size here means physical size in area.
     *
     * \param town_size New size of the town
     */
    void setSize(unsigned int town_size);
    /**
     * Get size of town.
     *
     * \returns Current size of town
     */
    unsigned int getSize() const
    {
        return size;
    }

    /**
     * Sets town position in coordinates.
     *
     * \param x New X position
     * \param y New Y position
     */
    void setPosition(double x, double y);
    /**
     * Sets town X position.
     *
     * \param x New X position
     */
    void setPositionX(double x);
    /**
     * Sets town Y position.
     *
     * \param y New Y position
     */
    void setPositionY(double y);
    /**
     * Get town X position.
     *
     * \returns Town X poisition
     */
    inline double getPositionX() const
    {
        return pos.x;
    }
    /**
     * Get town Y position.
     *
     * \returns Town Y poisition
     */
    inline double getPositionY() const
    {
        return pos.y;
    }
    /**
     * Get town position.
     *
     * \returns Point to the center of the city
     */
    blieng::Point getPosition() const
    {
        return pos;
    }

    /**
     * Add new item to town
     * FIXME: Should we handle items as unique_ptr/shared_ptr??
     *
     * \param item Item to be added to town
     */
    void addItem(Item *item);
    /**
     * Remove item from town
     *
     * \param item Item to be removed from town
     * \returns True if item was removed, false otherwise
     */
    bool removeItem(Item *item);
    /**
     * List all items in town.
     *
     * \returns Vector of items in town
     */
    std::vector<Item *> getItems();

    /**
     * Add a character to the town
     * FIXME: Should we handle characters as unique_ptr/shared_ptr??
     *
     * \param chr New characted to be added to town
     */
    void addCharacter(Character *chr);
    /**
     * Remove a character from the town
     *
     * \param chr Characted to be removed from town
     */
    bool removeCharacter(Character *chr);
    /**
     * List all characters in town.
     *
     * \returns Vector of characters in town
     */
    std::vector<Character *> getCharacters() const;

    /**
     * Get string representation of town
     *
     * \returns Town contens in string format
     */
    virtual std::string toString() const;

    /**
     * Recalculate town population.
     * Usually town population is kept in sync automatically,
     * but if problems found this can be utilized.
     * For example if charcter is killed it might be necessary
     * to update population manually.
     */
    void updatePopulation();
    /**
     * Get current population of town.
     * Will calculate number of all alive characters.
     *
     * \returns Number of alive characters
     */
    unsigned int getPopulation() const;
    /**
     * Get number of certain character class characters.
     * Characters might have different types on different games,
     * this can be utilized to get certain group of characters.
     *
     * \param character_class Name of the character class.
     * \returns Number of characters in character class.
     */
    unsigned int getCharacterClassCnt(const std::string &character_class);
    /**
     * Get characters in certain character class.
     * See also \ref getCharacterClassCnt
     *
     * \param character_class Name of the character class.
     * \returns Vector of characters in character class.
     */
    std::vector<blieng::Character *> getCharacterClass(
        const std::string &character_class);

private:
    std::vector<Item *> items;  //!< List of items
    std::vector<Character *> characters;  //!< List of characters
    std::string name;  //!< Name of the town
    unsigned int size;  //!< Town size
    blieng::Point pos;  //!< Town position
};

}  // namespace blieng

#endif  // __BLIENG_TOWN_H
