#ifndef __ITEM_H
#define __ITEM_H

#include <string>
#include <vector>
#include <map>
#include <boost/random/random_device.hpp>
#include <boost/format.hpp>
#include <boost/flyweight.hpp>

namespace blieng {

class ItemBase
{
public:
	ItemBase():base(""), type(""), rarity(1.0) { }

	boost::flyweight<std::string> base;
	boost::flyweight<std::string> type;
	boost::flyweight<double> rarity;

	boost::flyweight<std::vector<std::string> > consumes;

	std::string toString();
	void assign(ItemBase *parent);
	bool equals(ItemBase *another);
};

class Item : public ItemBase
{
public:
	Item();

	bool consume(Item *);
	Item *produce();

private:
	int getRandomInt(int limit_low, int limit_max);
	void getItemBases();

	static bool ok;
	static std::map<std::string, ItemBase *> item_bases;

	boost::random::random_device *gen;
};

}

#endif
