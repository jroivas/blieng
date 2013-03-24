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
	init();
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
		usable = true;
	}
}

void Item::init()
{
	usable = false;
	gen = new boost::random::random_device();
	getItemBases();
}

Item::Item(std::string name)
{
	init();
	usable = false;
	if (item_bases.size() > 0) {
		std::map<std::string, ItemBase *>::iterator ib = item_bases.begin();
		ItemBase *orig = NULL;
		while (ib != item_bases.end()) {
			if (ib->first == name) {
				orig = ib->second;
			}
			ib++;
		}
		if (orig == NULL) orig = item_bases.begin()->second;
		assign(orig);
	}
}

void Item::getItemBases()
{
	if (ok) return;

	Json::Value root_val = Data::getInstance()->readJson("items.json");
	if (!root_val.isObject()) return;

	/* Go thorough items */
	Json::Value::Members mem = root_val.getMemberNames();
	Json::Value::Members::iterator mi = mem.begin();

	while (mi != mem.end()) {
		Json::Value item_val = Data::getInstance()->getJsonValue(root_val, *mi);
		ItemBase *item = new ItemBase();
		if (item_val.isObject()) {
			item->base = *mi;
			if (Data::getInstance()->isJsonKey(item_val, "type")) {
				Json::Value val = Data::getInstance()->getJsonValue(item_val, "type");
				if (val.isString()) item->type = val.asString();
			}
			if (Data::getInstance()->isJsonKey(item_val, "rarity")) {
				Json::Value val = Data::getInstance()->getJsonValue(item_val, "rarity");
				if (val.isNumeric()) {
					item->rarity = val.asDouble();
				}
			}
			if (Data::getInstance()->isJsonKey(item_val, "amount")) {
				Json::Value val = Data::getInstance()->getJsonValue(item_val, "amount");
				if (val.isNumeric()) {
					item->amount = val.asDouble();
				}
			}
			if (Data::getInstance()->isJsonKey(item_val, "consume")) {
				Json::Value val = Data::getInstance()->getJsonValue(item_val, "consume");
				std::map<std::string, double> consumes;
				if (val.isObject()) {
					Json::Value::Members consume_mem = val.getMemberNames();
					Json::Value::Members::iterator cmi = consume_mem.begin();
					while (cmi != consume_mem.end()) {
						Json::Value cnt_val = Data::getInstance()->getJsonValue(val, *cmi);
						if (cnt_val.isNumeric()) {
							consumes[*cmi] = cnt_val.asDouble();
						}
						cmi++;
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

bool Item::consume(Item *another)
{
	if (!doesConsume(another->base)) return false;
	if (!another->isUsable()) return false;

	double cnt = consumeCount(another->base);
	//for (consumes.get()[another->base]);

	if (cnt > another->amount) return false;
	stocks[another->base] += cnt;
	another->amount -= cnt;
	//stocks[another->base] += another->amount;
	//another->amount = 0;

	return true;
}

Item *Item::produce()
{
	bool can_consume = true;
	std::map<std::string, double>::const_iterator ci = consumes.get().begin();
	while (ci != consumes.get().end()) {
		if (stocks[ci->first] < ci->second) {
			can_consume = false;
		}
		ci++;
	}
	if (!can_consume) return NULL;

	Item *produced = new Item();
	produced->assign(this);
	produced->usable = true;
	produced->amount = 1.0;

	ci = consumes.get().begin();
	while (ci != consumes.get().end()) {
		stocks[ci->first] -= ci->second;
		ci++;
	}
	return produced;
}

void ItemBase::assign(ItemBase *parent) {
	if (parent == NULL) return;

	base = parent->base;
	type = parent->type;
	rarity = parent->rarity;
	amount = parent->amount;
	consumes = parent->consumes;
	setupStock();
}

bool ItemBase::equals(ItemBase *another)
{
	return (base == another->base && type == another->type && rarity == another->rarity);
}

bool ItemBase::doesConsume(std::string name)
{
	std::map<std::string, double>::const_iterator ci = consumes.get().begin();
	while (ci != consumes.get().end()) {
		if (ci->first == name) return true;
		ci++;
	}
	return false;
}

double ItemBase::consumeCount(std::string name)
{
	std::map<std::string, double>::const_iterator ci = consumes.get().begin();
	while (ci != consumes.get().end()) {
		if (ci->first == name) return ci->second;
		ci++;
	}
	return 0;
}

void ItemBase::setupStock()
{
	std::map<std::string, double>::const_iterator ci = consumes.get().begin();
	stocks.clear();
	while (ci != consumes.get().end()) {
		stocks[ci->first] = 0;
		ci++;
	}
}

std::string ItemBase::toString() {
	std::string tmp = "";
	tmp += "base    : " + base.get() + "\n";
	tmp += "type    : " + type.get() + "\n";
	tmp += "usable  : " + std::string((usable?"yes":"no")) + "\n";
	//tmp += "amount  : " + (boost::format("%f") % amount.get()).str() + "\n";
	tmp += "amount  : " + (boost::format("%f") % amount).str() + "\n";
	tmp += "rarity  : " + (boost::format("%f") % rarity.get()).str() + "\n";

	tmp += "consumes: ";
	std::map<std::string, double>::const_iterator ci = consumes.get().begin();
	bool first = true;
	while (ci != consumes.get().end()) {
		if (!first) tmp += ", ";
		tmp += (boost::format("%s (%f units)") % ci->first % ci->second).str();
		first = false;
		ci++;
	}
	tmp += "\n";

	tmp += "stock   : ";
	std::map<std::string, double>::iterator sci = stocks.begin();
	first = true;
	while (sci != stocks.end()) {
		if (!first) tmp += ", ";
		tmp += (boost::format("%s (%f units)") % sci->first % sci->second).str();
		first = false;
		sci++;
	}
	tmp += "\n";

	return tmp;
}
