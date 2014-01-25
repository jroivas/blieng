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
    virtual void remove(auto_vector<blieng::Card>::iterator &iter);

    virtual inline std::string toString() const { return ""; }
    virtual inline std::string uuid() const { return card_uuid; }

    virtual inline unsigned int size() const { return combined.size(); }

    virtual inline auto_vector<blieng::Card>::iterator begin() { return combined.begin(); }
    virtual inline auto_vector<blieng::Card>::iterator end() { return combined.end(); }
    virtual inline auto_vector<blieng::Card>::const_iterator begin() const { return combined.begin(); }
    virtual inline auto_vector<blieng::Card>::const_iterator end() const { return combined.end(); }

protected:
    auto_vector<blieng::Card> combined;
    void genUUID();

private:
    std::string card_uuid;
};

}

#endif
