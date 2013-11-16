#include "card.h"
#include <boost/foreach.hpp>
#include <boost/assert.hpp>

using blieng::Card;

Card::Card() : BliObject()
{
    genUUID();
}

Card::~Card()
{
    while (!combined.empty()) {
        combined.erase(combined.begin());
    }
}

void Card::combine(std::unique_ptr<blieng::Card> card)
{
    combined.push_back(std::move(card));
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

    auto it = combined.begin();
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

void Card::genUUID()
{
    card_uuid = "";
    for (unsigned int i=0; i<10; i++) {
        card_uuid += getRandomInt('A', 'Z');
    }
    card_uuid += "-";
    for (unsigned int i=0; i<10; i++) {
        card_uuid += getRandomInt('a', 'z');
    }
    card_uuid += "-";
    for (unsigned int i=0; i<10; i++) {
        card_uuid += getRandomInt('0', '9');
    }
}
