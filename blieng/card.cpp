/*
 * Copyright 2014-2017 Jouni Roivas
 */

#include "blieng/card.h"

#include <boost/foreach.hpp>
#include <boost/assert.hpp>

using blieng::Card;

Card::Card() : BliObject()
{
}

Card::~Card()
{
    while (!m_combined.empty()) {
        m_combined.erase(m_combined.begin());
    }
}

void Card::combine(std::unique_ptr<blieng::Card> card)
{
    m_combined.push_back(std::move(card));
}

bool Card::remove(size_t index)
{
    size_t cnt = 0;

    auto it = m_combined.begin();
    while (it != m_combined.end()) {
        if (cnt == index) {
            m_combined.erase(it);
            return true;
        }
        ++cnt;
        ++it;
    }

    return false;
}

bool Card::remove(auto_vector<blieng::Card>::iterator iter)
{
    BOOST_ASSERT(iter < m_combined.end());
    if (iter < m_combined.end()) {
        m_combined.erase(iter);
        return true;
    }
    return false;
}
