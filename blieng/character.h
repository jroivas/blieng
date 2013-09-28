#ifndef __BLIENG_CHARACTER_H
#define __BLIENG_CHARACTER_H

/** Basic character building blocks
 */

#include "bliobject.h"
#include "item.h"
#include "auto_vector.h"
#include <boost/shared_ptr.hpp>
#include <memory>

namespace blieng
{

class Character : public blieng::BliObject
{
public:
    Character();

    bool kill();
    bool isAlive();

    unsigned int addItem(std::unique_ptr<blieng::Item>);
    bool removeItem(unsigned int index);
    unsigned int size();

    void assignObject(const blieng::BliObject *another);
    void assignObject(Character *another);

protected:
    auto_vector<blieng::Item> items;
};

}

#endif
