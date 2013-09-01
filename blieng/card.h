#ifndef __BLIENG_CARD_H
#define __BLIENG_CARD_H

#include "bliobject.h"
#include <vector>

namespace blieng
{

class Card : public BliObject
{
public:
    Card();

    virtual void combine(blieng::Card *card);
    virtual void remove(blieng::Card *card);
    virtual std::vector<blieng::Card *> getCombined();

    virtual std::string toString() { return ""; }

protected:
    std::vector<blieng::Card *> combined;
};

}

#endif
