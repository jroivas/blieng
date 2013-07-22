#ifndef __BLIENG_CHARACTER_H
#define __BLIENG_CHARACTER_H

/** Basic character building blocks
 */

#include "bliobject.h"
#include "item.h"

namespace blieng
{

class Character : public blieng::BliObject
{
public:
    Character();

    bool kill();
    bool isAlive();

    void addItem(blieng::Item *);
    bool removeItem(blieng::Item *);
    std::vector<blieng::Item *> getItems() const;

    virtual void assignObject(blieng::BliObject *another);
    virtual void assignObject(Character *another);

protected:
    std::vector<blieng::Item *> items;
};

}

#endif
