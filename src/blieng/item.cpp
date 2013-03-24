#include "item.h"
#include <boost/random/uniform_int_distribution.hpp>
#include "data.h"
#include <string>

using blieng::Item;
using blieng::ItemBase;

bool Item::ok = false;
std::map<std::string, blieng::ItemBase *> Item::item_bases;

Item::Item()
{
	gen = new boost::random::random_device();
	getItemBases();

	int num = getRandomInt(0, item_bases.size());
	if (item_bases.size() > 0) {
		std::map<std::string, ItemBase *>::iterator ib = item_bases.begin();
		int index = 0;
		ItemBase *orig = NULL;
		while (ib != item_bases.end()) {
			if (num == index) {
				orig = ib->second;
			}
			ib++;
			index++;
		}
		if (orig == NULL) orig = item_bases.begin()->second;
		assign(orig);
	}
}

void Item::getItemBases()
{
	if (ok) return;

	Json::Value val = Data::getInstance()->readJson("items.json");
	if (!val.isObject()) return;

	/* Go thorough items */
	Json::Value::Members mem = val.getMemberNames();
	Json::Value::Members::iterator mi = mem.begin();

	while (mi != mem.end()) {
		Json::Value item_val = Data::getInstance()->getJsonValue(val, *mi);
		ItemBase *item = new ItemBase();
		if (item_val.isObject()) {
			item->base = *mi;
			if (Data::getInstance()->isJsonKey(item_val, "type")) {
				Json::Value val = Data::getInstance()->getJsonValue(item_val, "type");
				if (val.isString()) item->type = val.asString();
			}
			if (Data::getInstance()->isJsonKey(item_val, "rarity")) {
				Json::Value val = Data::getInstance()->getJsonValue(item_val, "rarity");
				if (val.isDouble()) {
					item->rarity = val.asDouble();
				}
			}
			if (Data::getInstance()->isJsonKey(item_val, "consume")) {
				Json::Value val = Data::getInstance()->getJsonValue(item_val, "consume");
				std::vector<std::string> consumes;
				if (val.isArray()) {
					Json::Value::ArrayIndex len = val.size();
					for (Json::Value::ArrayIndex index = 0; index < len; index++) {
						Json::Value arr_val = val.get(index, Json::Value());
						if (arr_val.isString()) {
							consumes.push_back(arr_val.asString());
						}
					}
				}
				item->consumes = consumes;
			}
			item_bases[*mi] = item;
		}
		mi++;
	}
	ok = true;
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

void ItemBase::assign(ItemBase *parent) {
	if (parent == NULL) return;

	base = parent->base;
	type = parent->type;
	rarity = parent->rarity;
	consumes = parent->consumes;
}

bool ItemBase::equals(ItemBase *another)
{
	return (base == another->base && type == another->type && rarity == another->rarity);
}

std::string ItemBase::toString() {
	std::string tmp = "";
	tmp += "base    : " + base.get() + "\n";
	tmp += "type    : " + type.get() + "\n";
	tmp += "rarity  : " + (boost::format("%f") % rarity.get()).str() + "\n";
	tmp += "consumes: ";
	
	std::vector<std::string>::const_iterator ci = consumes.get().begin();
	bool first = true;
	while (ci != consumes.get().end()) {
		if (!first) tmp += ", ";
		tmp += *ci;
		first = false;
		ci++;
	}
	tmp += "\n";

	return tmp;
}
