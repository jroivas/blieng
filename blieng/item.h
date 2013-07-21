#ifndef __BLIENG_ITEM_H
#define __BLIENG_ITEM_H

#include <string>
#include <vector>
#include <map>
#include <boost/random/random_device.hpp>
#include <boost/format.hpp>
#include <boost/flyweight.hpp>
#include "bliobject.h"

namespace blieng {

/* Item base, which contais mandatory parts of the item.
 */
class ItemBase : public BliObject
{
public:
    ItemBase() : BliObject(), base(""), type(""), image(""), rarity(1.0), amount(0.0), life(-1), usable(false) { }

    boost::flyweight<std::string> base;
    boost::flyweight<std::string> type;
    boost::flyweight<std::string> image;
    double rarity;
    double amount;
    long int life;
    bool usable;

    boost::flyweight<std::map<std::string, double> > consumes;
    std::map<std::string, double> stocks;

    bool doesConsume(std::string);
    double consumeCount(std::string name);
    std::vector<std::string> getConsumes();
    void updateConsume(std::string name, double count);
    void removeConsume(std::string name);
    void setConsume(std::map<std::string, double> new_consumes);
    void clearConsume();

    std::string itemToString();
    void assignItem(ItemBase *parent);
    bool equals(ItemBase *another);
    void setupStock();

    bool hasStock();
    double hasStock(std::string name);

    bool update(ItemBase *another);
    bool age(long int amount);
    bool exhausted();

    std::string generateJson(std::string indent="");
};

/* Item object itself, may contain item specific stuff
*/
class Item : public ItemBase
{
public:
    Item();
    Item(std::string name);
    virtual ~Item() { }

    bool consume(Item *);
    Item *produce(double amount=1);
    bool isUsable() const { return usable; }
    void setUsable() { usable = true; }
    std::vector<std::string> listItems();
    bool isItem(std::string name);
    bool removeItem(Item *);
    bool registerItem(Item *);
    std::string generateBaseJson();
    virtual std::string toString();

private:
    void init();
    void getItemBases();

    static bool ok;
    static std::map<std::string, ItemBase *> item_bases;
};

}

#endif
