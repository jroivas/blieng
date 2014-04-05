#ifndef __BLIENG_CARD_H
#define __BLIENG_CARD_H

#include "bliobject.h"
#include <vector>
#include <memory>
#include "auto_vector.h"

using std::unique_ptr;

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
    virtual void combine(std::unique_ptr<blieng::Card> card);
    /**
     * Remove card from combined cards.
     * The card to be removed is marked with a index starting from 0,
     * if invalid index is given, false is returned.
     *
     * \param index Numerical index to remove from
     * \return True if a card is removed, false otherwise
     */
    virtual bool remove(size_t index);
    /**
     * Remove card from combined cards.
     * The card is removed from specified iterator position.
     *
     * \param iter Iterator inside the combined stack
     * \returns True if removed, false otherwise
     */
    virtual bool remove(auto_vector<blieng::Card>::iterator &iter);

    /**
     * Convert the card to string.
     * For now this is just a stub to be implemented in inherited classes.
     *
     * \returns Card contens in string
     */
    virtual inline std::string toString() const { return ""; }
    /**
     * Get the unique ID of this card.
     * All cards gets automatically unique ID upon creation,
     * that way card can be referred and handled easily.
     *
     * One should not do any assumptions about this ID,
     * and the format of the sting may ne changed at any time.
     * The only thing that's ensured is that a card can be identified
     * with this string.
     *
     * \returns Unique ID of this card as string
     */
    virtual inline std::string uuid() const { return card_uuid; }

    /**
     * Get number of combined cards.
     *
     * \returns Number of combined cards
     */
    virtual inline unsigned int size() const { return combined.size(); }

    /**
     * Get modifier start iterator of combined cards.
     *
     * \returns Iterator to beginning of combined cards.
     */
    virtual inline auto_vector<blieng::Card>::iterator begin() { return combined.begin(); }
    /**
     * Get modifier end iterator of combined cards.
     *
     * \returns Iterator to end of combined cards.
     */
    virtual inline auto_vector<blieng::Card>::iterator end() { return combined.end(); }
    /**
     * Get start iterator of constant combined cards.
     *
     * \returns Iterator to beginning of combined cards.
     */
    virtual inline auto_vector<blieng::Card>::const_iterator begin() const { return combined.begin(); }
    /**
     * Get end iterator of constant combined cards.
     *
     * \returns Iterator to end of combined cards.
     */
    virtual inline auto_vector<blieng::Card>::const_iterator end() const { return combined.end(); }

protected:
    /**
     * Generates the Unique ID.
     */
    void genUUID();

    auto_vector<blieng::Card> combined; //!< List of combined cards

private:
    std::string card_uuid; //!< Unique ID of card
};

}

#endif
