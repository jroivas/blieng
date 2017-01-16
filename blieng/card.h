/*
 * Copyright 2014-2017 Jouni Roivas
 */

#ifndef __BLIENG_CARD_H
#define __BLIENG_CARD_H

#include <memory>
#include <string>
#include <vector>

#include "blieng/bliobject.h"
#include "blieng/auto_vector.h"

namespace blieng
{

/**
 * A general card, support for mainly basic combination operations.
 * Idea is to inherit this and extend with real functionality in the game.
 */
class Card : public BliObject
{
public:
    /**
     * Constructor
     * Will initialize unique ID for the card.
     */
    Card();
    virtual ~Card();

    /**
     * Combine other card to this one.
     * The given card is permanently moved under this one.
     *
     * \param card Card to be combined
     */
    virtual void combine(
        std::unique_ptr<blieng::Card> card);
    /**
     * Remove card from combined cards.
     * The card to be removed is marked with a index starting from 0,
     * if invalid index is given, false is returned.
     *
     * \param index Numerical index to remove from
     * \return True if a card is removed, false otherwise
     */
    virtual bool remove(
        size_t index);
    /**
     * Remove card from combined cards.
     * The card is removed from specified iterator position.
     *
     * \param iter Iterator inside the combined stack
     * \returns True if removed, false otherwise
     */
    virtual bool remove(
        auto_vector<blieng::Card>::iterator iter);

    /**
     * Convert the card to string.
     * For now this is just a stub to be implemented in inherited classes.
     *
     * \returns Card contens in string
     */
    virtual inline std::string toString() const
    {
        return "";
    }

    /**
     * Get number of combined cards.
     *
     * \returns Number of combined cards
     */
    virtual inline unsigned int size() const
    {
        return m_combined.size();
    }

    /**
     * Get modifier start iterator of combined cards.
     *
     * \returns Iterator to beginning of combined cards.
     */
    virtual inline auto_vector<blieng::Card>::iterator begin()
    {
        return m_combined.begin();
    }
    /**
     * Get modifier end iterator of combined cards.
     *
     * \returns Iterator to end of combined cards.
     */
    virtual inline auto_vector<blieng::Card>::iterator end()
    {
        return m_combined.end();
    }
    /**
     * Get start iterator of constant combined cards.
     *
     * \returns Iterator to beginning of combined cards.
     */
    virtual inline auto_vector<blieng::Card>::const_iterator begin() const
    {
        return m_combined.begin();
    }
    /**
     * Get end iterator of constant combined cards.
     *
     * \returns Iterator to end of combined cards.
     */
    virtual inline auto_vector<blieng::Card>::const_iterator end() const
    {
        return m_combined.end();
    }

protected:
    auto_vector<blieng::Card> m_combined;  //!< List of combined cards
};

}  // namespace blieng

#endif  // __BLIENG_CARD_H
