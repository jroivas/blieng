#ifndef __BLIENG_ITEM_H
#define __BLIENG_ITEM_H

#include <string>
#include <vector>
#include <map>
#include <boost/random/random_device.hpp>
#include <boost/format.hpp>
#include <boost/flyweight.hpp>
#include "bliobject.h"
#include "auto_vector.h"
#include "configure.h"
#include "data.h"

#if defined(__cplusplus) && __cplusplus >= 201103L
#include <memory>
using namespace std;
#else

#if defined(__cplusplus) && __cplusplus < 201103L
#include <tr1/memory>
using namespace std;
#else
#include <boost/smart_ptr/shared_ptr.hpp>
using namespace boost;
#endif

#endif


using std::unique_ptr;

namespace blieng {

/**
 * Item base, which contais mandatory parts of the item.
 */
class ItemBase : public BliObject
{
public:
    /**
     * Just initialize some sane item base.
     */
    ItemBase() : BliObject(), base(""), type(""), image(""), rarity(1.0), amount(0.0), life(-1), usable(false) { }
    virtual ~ItemBase() { }

    boost::flyweight<std::string> base; //!< Names of different item bases
    boost::flyweight<std::string> type; //!< Type of the bases
    boost::flyweight<std::string> image; //!< Images of the bases
    double rarity; //!< Tells how rare this item is
    double amount; //!< Amount of the item, ie. quantity
    long int life; //!< Items life, can be eternal (-1) or decreasing, when life ends item is unusable
    bool usable; //!< Tells if one can use the item

    boost::flyweight<std::map<std::string, double> > consumes; //!< What items this item consumes?
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
 * Item object itself, may contain item specific stuff
 */
class Item : public ItemBase
{
public:
    Item(shared_ptr<blieng::Configure> configure, shared_ptr<blieng::Data> data);
    Item(const std::string &name);
    virtual ~Item() { }

    std::unique_ptr<Item> copy();

    bool consume(std::unique_ptr<Item>);
    std::unique_ptr<Item> produce(double produce_amount=1) throw (char *);
    bool isUsable() const { return usable; }
    void setUsable() { usable = true; }
    std::vector<std::string> listItems() const;
    bool isItem(const std::string &name) const;
    bool removeItem(std::unique_ptr<Item> item);

    std::string generateBaseJson();
    virtual std::string toString() const;

private:
    void init();
    void getItemBases();

    static bool ok;
    static auto_vector<ItemBase> item_bases;
    shared_ptr<blieng::Configure> config;
    shared_ptr<blieng::Data> data;
};

}

#endif
