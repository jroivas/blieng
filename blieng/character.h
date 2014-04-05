#ifndef __BLIENG_CHARACTER_H
#define __BLIENG_CHARACTER_H

#include "bliobject.h"
#include "item.h"
#include "auto_vector.h"
#include <memory>

namespace blieng
{

/**
 * Basic character building blocks
 */
class Character : public blieng::BliObject
{
public:
    /**
     * Constructor, ensures that character is alive.
     */
    Character();
    virtual ~Character() {}

    /**
     * Kills the character.
     * For basic characters always manages to kill them.
     * This can be reimplemented in inherited classes to create immortal characters.
     * Internally changes BliObject key "dead" to true.
     *
     * \returns True if killed, false otherwise
     */
    virtual bool kill();
    /**
     * Check if character is still alive.
     *
     * \returns True if character is alive, false otherwise.
     */
    bool isAlive() const;

    /**
     * Add item for characters posession
     *
     * \param newitem The item to be added for character
     * \returns Number of items in characters posession
     */
    unsigned int addItem(std::unique_ptr<blieng::Item> newitem);
    /**
     * Remove item from character.
     * Removes the item from specified index starting from 0.
     * If given index is invalid, false is returned.
     *
     * \param index Index of the item to be removed, starting from 0
     * \returns True if item removed, false otherwise
     */
    bool removeItem(unsigned int index);
    /**
     * Get number of items in characters posession.
     *
     * \returns Number of items in characters posession
     */
    unsigned int size() const;

    /**
     * Assigns any other BliObject to this character.
     * Assignment is same as in BliObject::assign
     *
     * \param another Any other BliObject
     */
    void assignObject(const blieng::BliObject *another);
    /**
     * Assigns any other Character to this character.
     * Makes assignment on BliObject level plus copies ites from
     * the other character.
     * \remark Makes COPY of items.
     *
     * \param another Any other Character
     */
    void assignObject(Character *another);

protected:
    auto_vector<blieng::Item> items; //!< Items character owns
};

}

#endif
