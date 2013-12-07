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

    virtual std::string toString() const { return ""; }
    virtual std::string uuid() const { return card_uuid; }

protected:
    auto_vector<blieng::Card> combined;
    void genUUID();

private:
    std::string card_uuid;
};

}

#endif
