#include "item.h"
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/foreach.hpp>
#include "data.h"
#include <string>

using blieng::Item;
using blieng::ItemBase;

bool Item::ok = false;
std::map<std::string, blieng::ItemBase *> Item::item_bases;

typedef std::pair<std::string, ItemBase *> item_bases_t;
typedef std::pair<std::string, double> consume_t;

Item::Item() : ItemBase()
{
	init();
	int num = getRandomInt(0, item_bases.size());
	if (item_bases.size() > 0) {
		int index = 0;
		ItemBase *orig = NULL;
		BOOST_FOREACH(item_bases_t val, item_bases) {
			orig = val.second;
			if (num == index) {
				break;
			}
			index++;
		}
		if (orig != NULL) {
			assign(orig);
			usable = true;
		}
	}
}

void Item::init()
{
	life = (long int)(-1);
	usable = false;
	getItemBases();
}

Item::Item(std::string name) : ItemBase()
{
	init();
	usable = false;
	if (item_bases.size() > 0) {
		ItemBase *orig = NULL;
		BOOST_FOREACH(item_bases_t val, item_bases) {
			if (val.first == name) {
				orig = val.second;
			}
		}
		if (orig != NULL) {
			assign(orig);
		}
	}
}

std::vector<std::string> Item::listItems()
{
	std::vector<std::string> tmp;
	BOOST_FOREACH(item_bases_t val, item_bases) {
		tmp.push_back(val.first);
	}
	return tmp;
}


void Item::getItemBases()
{
	if (ok) return;

	Json::Value root_val = Data::getInstance()->readJson("items.json");
	if (!root_val.isObject()) return;

	/* Go thorough items */
	BOOST_FOREACH(std::string mi, root_val.getMemberNames()) {
		Json::Value item_val = Data::getInstance()->getJsonValue(root_val, mi);
		ItemBase *item = new ItemBase();
		if (item_val.isObject()) {
			item->base = mi;
			if (Data::getInstance()->isJsonKey(item_val, "type")) {
				Json::Value val = Data::getInstance()->getJsonValue(item_val, "type");
				if (val.isString()) item->type = val.asString();
			}
			if (Data::getInstance()->isJsonKey(item_val, "image")) {
				Json::Value val = Data::getInstance()->getJsonValue(item_val, "image");
				if (val.isString()) item->image = val.asString();
			}
			if (Data::getInstance()->isJsonKey(item_val, "rarity")) {
				Json::Value val = Data::getInstance()->getJsonValue(item_val, "rarity");
				if (val.isNumeric()) {
					item->rarity = val.asDouble();
				}
			}
			if (Data::getInstance()->isJsonKey(item_val, "life")) {
				Json::Value val = Data::getInstance()->getJsonValue(item_val, "life");
				if (val.isNumeric()) {
					item->life = val.asInt();
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
					BOOST_FOREACH(std::string cmi, val.getMemberNames()) {
						Json::Value cnt_val = Data::getInstance()->getJsonValue(val, cmi);
						if (cnt_val.isNumeric()) {
							consumes[cmi] = cnt_val.asDouble();
						}
					}
				}
				item->consumes = consumes;
			}
			item_bases[mi] = item;
		}
	}
	ok = true;
}

int Item::getRandomInt(int limit_low, int limit_max)
{
	boost::random::uniform_int_distribution<> dist(limit_low, limit_max);
	return dist(*Data::getInstance()->getGen());
}

bool Item::consume(Item *another)
{
	if (!doesConsume(another->base)) return false;
	if (!another->isUsable()) return false;

	double cnt = consumeCount(another->base);

	if (cnt > another->amount) return false;

	//std::cout << base << " consumes " << cnt << "  " << another->base << "\n";
	stocks[another->base] += cnt;
	another->amount -= cnt;
	//std::cout << another->amount << "  " << another->base << "\n";

	return true;
}

Item *Item::produce(double produce_amount)
{
	bool can_consume = true;
	BOOST_FOREACH(consume_t val, consumes.get()) {
		if (stocks[val.first] < val.second) {
			can_consume = false;
		}
	}
	if (!can_consume) return NULL;

	Item *produced = new Item();
	produced->assign(this);
	produced->usable = true;
	if (amount > 0) {
		produced->amount = produce_amount * amount;
	} else {
		produced->amount = produce_amount;
	}

	BOOST_FOREACH(consume_t val, consumes.get()) {
		stocks[val.first] -= val.second;
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
	life = parent->life;
	image = parent->image;
	setupStock();
}

bool ItemBase::equals(ItemBase *another)
{
	return (base == another->base && type == another->type && rarity == another->rarity);
}

bool ItemBase::doesConsume(std::string name)
{
	BOOST_FOREACH(consume_t val, consumes.get()) {
		if (val.first == name) return true;
	}
	return false;
}

bool ItemBase::exhausted()
{
	if (life == (long int)-1) return false;

	if (life == 0) return true;
	return false;
}

bool ItemBase::age(long int amount)
{
	if (amount <= 0) return false;
	if (life == (long int)-1) return true;

	if (life > amount) {
		life -= amount;
		return true;
	}

	life = 0;
	usable = false;
	return false;
}

bool ItemBase::hasStock()
{
	BOOST_FOREACH(consume_t val, stocks) {
		if (val.second > 0) return true;
	}
	return false;
}

double ItemBase::hasStock(std::string name)
{
	BOOST_FOREACH(consume_t val, stocks) {
		if (val.first == name) return val.second;
	}
	return 0;
}

double ItemBase::consumeCount(std::string name)
{
	BOOST_FOREACH(consume_t val, consumes.get()) {
		if (val.first == name) return val.second;
	}
	return 0;
}

bool ItemBase::update(ItemBase *another)
{
	if (another == NULL) return false;
	if (another->base != base) return false;
	if (another->life != life) return false;

	amount += another->amount;
	another->amount = 0;
	life = another->life;

	BOOST_FOREACH(consume_t val, another->stocks) {
		stocks[val.first] += val.second;
		val.second = 0;
	}

	return true;
}

void ItemBase::setupStock()
{
	stocks.clear();
	BOOST_FOREACH(consume_t val, consumes.get()) {
		stocks[val.first] = 0;
	}
}

std::string ItemBase::toString() {
	std::string tmp = "";
	tmp += "base    : " + base.get() + "\n";
	tmp += "type    : " + type.get() + "\n";
	tmp += "usable  : " + std::string((usable?"yes":"no")) + "\n";
	if (life == (long int)-1) {
		tmp += "life    : inf\n";
	} else {
		tmp += "life    : " + (boost::format("%d") % life).str() + "\n";
	}
	tmp += "amount  : " + (boost::format("%f") % amount).str() + "\n";
	tmp += "rarity  : " + (boost::format("%f") % rarity).str() + "\n";

	tmp += "consumes: ";
	
	bool first = true;
	BOOST_FOREACH(consume_t val, consumes.get()) {
		if (!first) tmp += ", ";
		first = false;
		tmp += (boost::format("%s (%f units)") % val.first % val.second).str();
	}
	tmp += "\n";

	tmp += "stock   : ";
	first = true;
	BOOST_FOREACH(consume_t val, stocks) {
		if (!first) tmp += ", ";
		first = false;
		tmp += (boost::format("%s (%f units)") % val.first % val.second).str();
	}
	tmp += "\n";

	return tmp;
}
