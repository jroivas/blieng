#ifndef __ITEM_H
#define __ITEM_H

#include <string>
#include <vector>
#include <map>
#include <boost/random/random_device.hpp>
#include <boost/format.hpp>

namespace blieng {

class ItemBase
{
public:
	ItemBase():base(""), type(""), rarity(1.0) { }

	std::string base;
	std::string type;
	double rarity;

	std::vector<std::string> consumes;

	std::string toString() {
		std::string tmp = "";
		tmp += "base    : " + base + "\n";
		tmp += "type    : " + type + "\n";
		tmp += "rarity  : " + (boost::format("%f") % rarity).str() + "\n";
		tmp += "consumes: ";
		
		std::vector<std::string>::iterator ci = consumes.begin();
		bool first = true;
		while (ci != consumes.end()) {
			if (!first) tmp += ", ";
			tmp += *ci;
			first = false;
			ci++;
		}
		tmp += "\n";

		return tmp;
	}

	void assign(ItemBase *parent) {
		if (parent == NULL) return;

		// TODO flyweight
		base = parent->base;
		type = parent->type;
		rarity = parent->rarity;
		consumes = parent->consumes;
	}
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
