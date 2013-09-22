#ifndef __BLIENG_CHARACTER_H
#define __BLIENG_CHARACTER_H

/** Basic character building blocks
 */

#include "bliobject.h"
#include "item.h"
#include "auto_vector.h"
#include <boost/shared_ptr.hpp>

namespace blieng
{

class Character : public blieng::BliObject
{
public:
    Character();

    bool kill();
    bool isAlive();

    //void addItem(blieng::Item*);
    void addItem(std::unique_ptr<blieng::Item>);
    //bool removeItem(std::unique_ptr<blieng::Item>);
    bool removeItem(const blieng::Item *);
    //const std::vector<blieng::Item *> getItems() const;
    const auto_vector<blieng::Item>* getItems();

    //virtual void assignObject(blieng::BliObject *another);
    virtual void assignObject(Character *another);

protected:
    //std::vector<blieng::Item*> items;
    auto_vector<blieng::Item> items;
};

}

#endif
