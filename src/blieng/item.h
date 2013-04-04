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
	ItemBase():base(""), type(""), image(""), rarity(1.0), amount(0.0), life(-1), usable(false) {
		/*std::map<std::string, double> empty;
		consumes = empty;
		*/
	}

	boost::flyweight<std::string> base;
	boost::flyweight<std::string> type;
	boost::flyweight<std::string> image;
	double rarity;
	double amount;
	long int life;
	bool usable;

	boost::flyweight<std::map<std::string, double> > consumes;

	bool doesConsume(std::string);
	double consumeCount(std::string name);
	std::vector<std::string> getConsumes();
	void updateConsume(std::string name, double count);
	void removeConsume(std::string name);
	void setConsume(std::map<std::string, double> new_consumes);
	void clearConsume();

	std::string toString();
	void assign(ItemBase *parent);
	bool equals(ItemBase *another);
	std::map<std::string, double> stocks;
	void setupStock();

	bool hasStock();
	double hasStock(std::string name);

	bool update(ItemBase *another);
	bool age(long int amount);
	bool exhausted();

	std::string generateItemJson(std::string indent="");

};

class Item : public ItemBase
{
public:
	Item(bool randomze=false);
	Item(std::string name);
	~Item() {
		base = "";
	}

	bool consume(Item *);
	Item *produce(double amount=1);
	bool isUsable() { return usable; }
	void setUsable() { usable = true; }
	std::vector<std::string> listItems();
	bool removeItem(Item *);
	bool registerItem(Item *);
	std::string generateJson();

private:
	void init();
	int getRandomInt(int limit_low, int limit_max);
	void getItemBases();

	static bool ok;
	static std::map<std::string, ItemBase *> item_bases;
};

}

#endif
