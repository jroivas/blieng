#ifndef __BLIENG_CARD_H
#define __BLIENG_CARD_H

#include "bliobject.h"
#include <vector>
#include <memory>
#include "auto_vector.h"

using std::unique_ptr;

namespace blieng
{

class Card : public BliObject
{
public:
    Card();
    virtual ~Card();

    virtual void combine(std::unique_ptr<blieng::Card> card);
    virtual bool remove(size_t index);
    virtual void remove(auto_vector<blieng::Card>::iterator iter);

    virtual std::string toString() { return ""; }

protected:
    auto_vector<blieng::Card> combined;
};

}

#endif
