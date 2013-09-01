#include "card.h"
#include <boost/foreach.hpp>

using blieng::Card;

Card::Card() : BliObject()
{
}

void Card::combine(blieng::Card *card)
{
    combined.push_back(card);
}

std::vector<blieng::Card *> Card::getCombined()
{
    return combined;
}

void Card::remove(blieng::Card *card)
{
    std::vector<blieng::Card *>::iterator it = combined.begin();

    while (it != combined.end()) {
        if (*it == card) {
            combined.erase(it);
            return;
        }
        ++it;
    }
}
