#include "item.h"
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/foreach.hpp>
#include "data.h"
#include "configure.h"
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
    return;
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
    std::cout << "Creating: " << name << "\n";
    if (!item_bases.empty()) {
        ItemBase *orig = NULL;
        BOOST_FOREACH(item_bases_t val, item_bases) {
            if (val.first == name) {
                orig = val.second;
            }
        }
        if (orig != NULL) {
            assignItem(orig);
        } else {
            base = name;
        }
    }
    std::cout << "Got: " << base << "\n";
}

std::vector<std::string> Item::listItems()
{
    std::vector<std::string> tmp;
    BOOST_FOREACH(item_bases_t val, item_bases) {
        tmp.push_back(val.first);
    }
    return tmp;
}

bool Item::isItem(std::string name)
{
    BOOST_FOREACH(item_bases_t val, item_bases) {
        if (val.first == name) return true;
    }
    return false;
}

bool Item::removeItem(Item *item)
{
    if (item == NULL) return false;
    bool res = false;
    BOOST_FOREACH(item_bases_t val, item_bases) {
        if (item->base == val.first) {
            item_bases.erase(val.first);
            return true;
        }
    }

    return res;
}

bool Item::registerItem(Item *item)
{
    if (item == NULL) return false;
    BOOST_FOREACH(item_bases_t val, item_bases) {
        if (item->base == val.first) {
            return false;
        }
    }
    item_bases[item->base] = item;
    return true;
}


void Item::getItemBases()
{
    if (ok) return;

    std::string items_file = Configure::getInstance()->getStringValue("itemfile");
    Json::Value root_val = Data::getInstance()->readJson(items_file); //FIXME
    if (!root_val.isObject()) return;

    /* Go thorough items */
    //TODO Refactor
    BOOST_FOREACH(std::string mi, root_val.getMemberNames()) {
        Json::Value item_val = Data::getInstance()->getJsonValue(root_val, mi);
        ItemBase *item = new ItemBase();
        if (item_val.isObject()) {
            item->base = mi;
            std::vector<std::string> item_names = Data::getInstance()->getJsonKeys(item_val);
            BOOST_FOREACH(std::string keyname, item_names) {

                Json::Value val = Data::getInstance()->getJsonValue(item_val, keyname);
                bool ok = false;
                if (keyname == "type") {
                    if (val.isString()) item->type = val.asString();
                    ok = true;
                }
                else if (keyname == "image") {
                    if (val.isString()) item->image = val.asString();
                    ok = true;
                }
                else if (keyname == "rarity") {
                    if (val.isNumeric()) {
                        item->rarity = val.asDouble();
                        ok = true;
                    }
                }
                else if (keyname == "life") {
                    if (val.isNumeric()) {
                        item->life = val.asInt();
                        ok = true;
                    }
                }
                //TODO XXX
                #if 0
                else if (keyname == "life") {
                    if (val.isNumeric()) {
                        item->amount = val.asDouble();
                        ok = true;
                    }
                }
                #endif
                else if (keyname == "consume") {
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
                    ok = true;
                }
                if (val.type() == Json::intValue) item->setValue(keyname, val.asInt());
                else if (val.type() == Json::uintValue) item->setValue(keyname, val.asUInt());
                else if (val.type() == Json::realValue) item->setValue(keyname, val.asDouble());
                else if (val.type() == Json::stringValue) item->setValue(keyname, val.asString());
                else if (val.type() == Json::booleanValue) item->setValue(keyname, val.asBool());
                else { //TODO unsupported
                    if (!ok) std::cout << keyname << "has unsupported type!\n";
                }
            }
            #if 0
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
            #endif
            item_bases[mi] = item;
        }
    }
    ok = true;
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
    std::cout << base << "  " << can_consume << " rai" << produce_amount <<"\n";
    if (!can_consume) return NULL;

    Item *produced = new Item(base);
    if (produced == NULL) {
        std::cout << "Can't create item!\n";
        exit(1);
    }
    produced->assignItem(this);
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

void ItemBase::assignItem(ItemBase *parent) {
    if (parent == NULL) return;

    base = parent->base;
    type = parent->type;
    rarity = parent->rarity;
    amount = parent->amount;
    consumes = parent->consumes;
    life = parent->life;
    image = parent->image;
    setupStock();
    assignObject(parent);
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

void ItemBase::updateConsume(std::string name, double count)
{
    std::map<std::string, double> new_consumes = consumes.get();
    new_consumes[name] = count;
    consumes = new_consumes;
}

void ItemBase::removeConsume(std::string name)
{
    std::map<std::string, double> new_consumes;
    BOOST_FOREACH(consume_t val, consumes.get()) {
        if (val.first != name) {
            new_consumes[name] = val.second;
        }
    }
    consumes = new_consumes;
}

void ItemBase::clearConsume()
{
    std::map<std::string, double> new_consumes;
    consumes = new_consumes;
}

void ItemBase::setConsume(std::map<std::string, double> new_consumes)
{
    consumes = new_consumes;
}

std::vector<std::string> ItemBase::getConsumes()
{
    std::vector<std::string> res;
    BOOST_FOREACH(consume_t val, consumes.get()) {
        res.push_back(val.first);
    }
    return res;
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

std::string ItemBase::itemToString() {
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

std::string Item::toString()
{
    return blieng::BliObject::toString() + itemToString();
}

std::string ItemBase::generateJson(std::string indent)
{
    std::list<std::string> res;
    if (type != "") res.push_back("\"type\": \"" + type.get() + "\"");
    if (image != "") res.push_back("\"image\": \"" + image.get() + "\"");
    if (amount != 0) res.push_back((boost::format("\"amount\": %f") % amount).str());
    if (rarity != 0) res.push_back((boost::format("\"rarity\": %f") % rarity).str());
    if (life != -1) res.push_back((boost::format("\"life\": %f") % life).str());

    std::string tmp = "\"consume\": {";
    bool first = true;
    bool got = false;
    BOOST_FOREACH(consume_t val, consumes.get()) {
        if (!first) tmp += ", ";
        first = false;
        tmp += (boost::format("\"%s\": %f") % val.first % val.second).str();
        got = true;
    }
    tmp += "}";
    if (got) res.push_back(tmp);

    std::string res_str = indent + "\"" + base.get() + "\": {";
    first = true;
    BOOST_FOREACH(std::string val, res) {
        if (!first) res_str += ", ";
        first = false;
        res_str += val;
    }
    res_str += "}";
    return res_str;
}

std::string Item::generateBaseJson()
{
    std::string res = "";
    bool first = true;
    BOOST_FOREACH(item_bases_t val, item_bases) {
        if (!first) res += ",\n";
        first = false;
        res += val.second->generateJson("  ");
    }
    return "{\n" + res + "\n}\n";
}
