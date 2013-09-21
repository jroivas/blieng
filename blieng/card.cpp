#include "card.h"
#include <boost/foreach.hpp>
#include <boost/assert.hpp>

using blieng::Card;

Card::Card() : BliObject()
{
}

Card::~Card()
{
    while (!combined.empty()) {
        combined.erase(combined.begin());
    }
}

void Card::combine(auto_ptr<blieng::Card> card)
{
    combined.push_back(card);
}

/*
std::vector<blieng::Card *> Card::getCombined()
{
    return combined;
}
*/

bool Card::remove(size_t index)
{
    size_t cnt = 0;

    std::vector<blieng::Card *>::iterator it = combined.begin();
    while (it != combined.end()) {
        if (cnt == index) {
            combined.erase(it);
            return true;
        }
        ++cnt;
        ++it;
    }

    return false;
}

void Card::remove(auto_vector<blieng::Card>::iterator iter)
{
    BOOST_ASSERT( iter < combined.end() );
    if (iter < combined.end()) {
        combined.erase(iter);
    }
}
