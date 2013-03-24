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
	ItemBase():base(""), type(""), rarity(1.0), amount(0.0), usable(false) { }

	boost::flyweight<std::string> base;
	boost::flyweight<std::string> type;
	boost::flyweight<double> rarity;
	double amount;
	bool usable;

	boost::flyweight<std::map<std::string, double> > consumes;

	bool doesConsume(std::string);
	double consumeCount(std::string name);

	std::string toString();
	void assign(ItemBase *parent);
	bool equals(ItemBase *another);
	std::map<std::string, double> stocks;
	void setupStock();

	bool hasStock();

};

class Item : public ItemBase
{
public:
	Item();
	Item(std::string name);

	bool consume(Item *);
	Item *produce(double amount=1);
	bool isUsable() { return usable; }
	void setUsable() { usable = true; }

private:
	void init();
	int getRandomInt(int limit_low, int limit_max);
	void getItemBases();

	static bool ok;
	static std::map<std::string, ItemBase *> item_bases;


	boost::random::random_device *gen;
};

}

#endif
