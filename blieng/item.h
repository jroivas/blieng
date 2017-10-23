/*
 * Copyright 2014-2017 Jouni Roivas
 */

#ifndef __BLIENG_ITEM_H
#define __BLIENG_ITEM_H

#include <boost/random/random_device.hpp>
#include <boost/format.hpp>
#include <boost/flyweight.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>
#include <vector>
#include <map>

#include "blieng/blieng.h"
#include "blieng/bliobject.h"
#include "blieng/auto_vector.h"
#include "blieng/configure.h"
#include "blieng/data.h"

using std::unique_ptr;

namespace blieng {

/**
 * Item base, which contais mandatory parts of the item.
 * Is not utilized as-is, but Item is inherited from this one.
 */
class ItemBase : public BliObject
{
public:
    /**
     * Just initialize some sane item base.
     */
    ItemBase() :
        BliObject(),
        base(""),
        type(""),
        image(""),
        rarity(1.0),
        amount(0.0),
        life(-1),
        usable(false) { }
    virtual ~ItemBase() { }

    boost::flyweight<std::string> base; //!< Names of different item bases
    boost::flyweight<std::string> type; //!< Type of the bases
    boost::flyweight<std::string> image; //!< Images of the bases
    double rarity; //!< Tells how rare this item is
    double amount; //!< Amount of the item, ie. quantity
    long int life; //!< Items life, can be eternal (-1) or decreasing
    bool usable; //!< Tells if one can use the item

    boost::flyweight<
        std::map<std::string, double>
    > consumes; //!< What items this item consumes?
    std::map<std::string, double> stocks; //!< User stocks of items

    /**
     * Check if this item consumes another item.
     * If item is consuming something, it might be able
     * to work or produce some other items.
     *
     * \param name Name of consumed item to check
     * \returns True if this item consumes the other item.
     */
    bool doesConsume(const std::string &name) const;
    /**
     * Checks how much of other item this consumes.
     * This might need different amount of the other item
     * to produce something else. Tell the amount of
     * item needed to get something done.
     *
     * \param name Name of consumed item to check
     * \returns How much of queried item is needed, or 0 if not needed at all
     */
    double consumeCount(const std::string &name);
    /**
     * Get list of items this item consumes.
     *
     * \returns Vector of strings containing names of consumed items
     */
    std::vector<std::string> getConsumes();
    /**
     * Update consumed count for item.
     * See also \ref consumeCount
     *
     * \param name Name of consumed item
     * \param count New amount of the consumed item
     */
    void updateConsume(const std::string &name, double count);
    /**
     * Removes item from list of consumed items.
     *
     * \param name Name of consumed item
     */
    void removeConsume(const std::string &name);
    /**
     * Replace the whole list of consumed items with new one.
     * Get list of new consumed items and their amounts.
     *
     * \param new_consumes Map of names and amounts
     */
    void setConsume(std::map<std::string, double> new_consumes);
    /**
     * Clear all consumed items.
     */
    void clearConsume();

    /**
     * Get string presentation of this item
     *
     * \returns String presentation of this item.
     */
    std::string itemToString() const;
    /**
     * Assigns anohter item to this one.
     * Sets all the crucial variables same as on the other item.
     *
     * \param another Other item
     */
    void assignItem(const ItemBase *another);
    /**
     * Checks if this items equals to other.
     * Checks base, type and rarity.
     *
     * \param another Other item
     * \returns True if they're equal
     */
    bool equals(ItemBase *another) const;

    /**
     * Initialize stock of other items with empty values.
     */
    void setupStock();
    /**
     * Check if there's any other item in stock.
     * If there's any amount of anything in stock,
     * returns true.
     *
     * \returns True if any item in stock.
     */
    bool hasStock() const;
    /**
     * Check if there's specific item in stock.
     *
     * \param name Name of item to check
     * \returns Amount of item in stock or 0
     */
    double hasStock(const std::string &name) const;

    /**
     * Updates this item from simial item.
     * Similarity is checked by base and type.
     * On top of that other item must have life.
     *
     * \param another Other, similar, item
     * \returns False if not updateable, true if updated
     */
    bool update(ItemBase *another);
    /**
     * Make the age of this item older.
     * Effectively decreases life,
     * and might exhaust the item in the end.
     *
     * \param amount Amount of time units passed
     * \returns True if item is still alive, false otherwise
     */
    bool age(long int amount);
    /**
     * Checks if item is exhausted.
     *
     * \returns True if item is exhausted and not usable, false is it's usable
     */
    bool exhausted() const;

    /**
     * Generate JSON representation of item.
     * Presentation should be valid JSON as string.
     *
     * \param indent Utilize indentation
     * \returns Item as JSON string
     */
    std::string generateJson(const std::string &indent="") const;
};

/**
 * Item object itself.
 * May contain item specific values, which are not
 * meant to be shared in item base.
 */
class Item : public ItemBase
{
public:
    /**
     * Intializes item, with necessary configuration and data backend instances.
     *
     * \param state Shared state instance
     */
    Item(boost::shared_ptr<blieng::BliengState> state);
    /**
     * Generate empty item, without configuration.
     *
     * \param name Name of the item
     */
    explicit Item(const std::string &name);
    virtual ~Item() { }

    /**
     * Make a copy of this item.
     *
     * \returns A new item
     */
    std::unique_ptr<Item> copy();

    /**
     * Make this item consume other item.
     * Consuming increases stock and enables this item
     * to produce something.
     *
     * \param another Other item
     * \returns True if item is consumed, false otherwise
     */
    bool consume(std::unique_ptr<Item> another);
    /**
     * Produces other item.
     * This item needs proper amount of all
     * of the other required items.
     * If all prequisities are met, a new item is produced.
     * Otherwise throws an error.
     *
     * \param produce_amount Amount of new items to produce
     * \returns A new item
     * \throws char* String to explain the error
     */
    std::unique_ptr<Item> produce(double produce_amount = 1);
    /**
     * Tell if this item is usable.
     * Item might not be usable if it's exhausted,
     * or it's generation is not completed.
     *
     * \returns True is item is usable, false otherwise.
     */
    bool isUsable() const
    {
        return usable;
    }
    /**
     * Force this item to be usable.
     */
    void setUsable()
    {
        usable = true;
    }

    /**
     * List all availale item names.
     *
     * \returns Vector of strings containing item names
     */
    std::vector<std::string> listItems() const;
    /**
     * Check if item with certain name is available.
     * Availability means one can create such item,
     * and it's defined in configuration.
     *
     * \param name Name of item to check
     * \returns True if item is available
     */
    bool isItem(const std::string &name) const;
    /**
     * Removes item from known items.
     * Permanently removes the item from supported items.
     *
     * \param item Item to remove
     * \returns True if removed properly, false otherwise
     */
    bool removeItem(std::unique_ptr<Item> item);

    /**
     * Generate JSON presentation of all item bases.
     *
     * \returns String containing item bases in JSON format
     */
    std::string generateBaseJson();
    /**
     * Get string presentation of this item.
     *
     * \returns String presentation of this item
     */
    virtual std::string toString() const;

private:
    /**
     * Initialize the item, set basic values.
     */
    void init();
    /**
     * Load item bases from configuration file.
     * Search for itemfile in configuration,
     * tries to load the file and parse it.
     * Item file should be JSON.
     */
    void getItemBases();

    static bool m_ok;  //!< Checks if item bases are already loaded to avoid loading it multiple times
    static auto_vector<ItemBase> m_item_bases;  //!< List of item base object
    boost::shared_ptr<blieng::BliengState> m_state;  //!< Shared engine state
};

}  // namespace blieng

#endif  // __BLIENG_ITEM_H
