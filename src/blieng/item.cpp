#include "item.h"
#include <boost/random/uniform_int_distribution.hpp>
#include "data.h"
#include <string>

using blieng::Item;

Item::Item()
{
	gen = new boost::random::random_device();
	//rarity = getRandomInt(0, 100);
	getItemBases();
}

void Item::getItemBases()
{
	Json::Value val = Data::getInstance()->readJson("items.json");
	if (!val.isObject()) return;

	/* Go thorough items */
	Json::Value::Members mem = val.getMemberNames();
	Json::Value::Members::iterator mi = mem.begin();

	while (mi != mem.end()) {
		Json::Value item_val = Data::getInstance()->getJsonValue(val, *mi);
		if (item_val.isObject()) {
			if (Data::getInstance()->isJsonKey(item_val, "type")) {
				Json::Value val_type = Data::getInstance()->getJsonValue(item_val, "type");
				std::cout << *mi << " type: " << val_type.asString() << "\n";
			}
			if (Data::getInstance()->isJsonKey(item_val, "rarity")) {
				Json::Value val_type = Data::getInstance()->getJsonValue(item_val, "rarity");
				std::cout << *mi << " rari: " << val_type.asDouble() << "\n";
			}
		} else {
			std::cout << *mi << "\n";
		}
		mi++;
	}
}

int Item::getRandomInt(int limit_low, int limit_max)
{
	boost::random::uniform_int_distribution<> dist(limit_low, limit_max);
	return dist(*gen);
}

bool Item::consume(Item *)
{
	return false;
}

Item *Item::produce()
{
	return this;
}
