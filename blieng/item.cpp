/*
 * Copyright 2014-2017 Jouni Roivas
 */

#include "blieng/item.h"

#include <boost/random/uniform_int_distribution.hpp>

#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "blieng/json.h"

using blieng::Item;
using blieng::ItemBase;

bool Item::m_ok = false;
auto_vector<ItemBase> Item::m_item_bases;

Item::Item(boost::shared_ptr<blieng::BliengState> _state) :
    ItemBase(), m_state(_state)
{
    init();
    return;
}

void Item::init()
{
    life = static_cast<long int>(-1);
    usable = false;
    getItemBases();
}

Item::Item(const std::string &name) : ItemBase()
{
    init();
    usable = false;
    std::cout << "Creating: " << name << "\n";
    if (!m_item_bases.empty()) {
        const ItemBase *orig = nullptr;
        // std::unique_ptr<ItemBase> orig;
        for (auto val : m_item_bases) {
            if (val->base == name) {
                orig = val;
            }
        }
        if (orig != nullptr) {
            assignItem(orig);
        } else {
            base = name;
        }
    }
    std::cout << "Got: " << base << "\n";
}

std::vector<std::string> Item::listItems() const
{
    std::vector<std::string> tmp;
    for (auto val : m_item_bases) {
        tmp.push_back(val->base);
    }
    return tmp;
}

bool Item::isItem(const std::string &name) const
{
    for (auto val : m_item_bases) {
        if (val->base == name) return true;
    }
    return false;
}

bool Item::removeItem(std::unique_ptr<Item> item)
{
    if (!item.get()) return false;

    auto it = m_item_bases.begin();
    while (it != m_item_bases.end()) {
        if (item->base == (*it)->base) {
            m_item_bases.erase(it);
            return true;
        }
    }

    return false;
}

#if 0
bool Item::registerItem(std::unique_ptr<Item> &item)
{
    if (!item.get()) return false;

    BOOST_FOREACH(auto val, m_item_bases) {
        if (item->base == val->base) {
            return false;
        }
    }
    m_item_bases.push_back(item);
    // m_item_bases[item->base] = item;

    return true;
}
#endif

void Item::getItemBases()
{
    if (m_ok) return;
    if (!m_state->m_config->isValue("itemfile")) return;

    std::string items_file = m_state->m_config->getStringValue("itemfile");
    json_value *root_val = m_state->m_data->readJson(items_file);  // FIXME
    if (!root_val->isObject()) return;

    /* Go thorough items */
    // TODO Refactor
    for (std::string mi : root_val->getMemberNames()) {
        const json_value *item_val = m_state->m_data->getJsonValue(root_val, mi);
        // ItemBase *item = new ItemBase();
        std::unique_ptr<ItemBase> item(new ItemBase());

        if (item_val->isObject()) {
            item->base = mi;
            std::vector<std::string> item_names = m_state->m_data->getJsonKeys(item_val);
            for (std::string keyname : item_names) {
                const json_value *val = m_state->m_data->getJsonValue(item_val, keyname);
                bool item_is_ok = false;
                if (keyname == "type") {
                    if (val->isString()) item->type = val->asString();
                    item_is_ok = true;
                }
                else if (keyname == "image") {
                    if (val->isString()) item->image = val->asString();
                    item_is_ok = true;
                }
                else if (keyname == "rarity") {
                    if (val->isNumeric()) {
                        item->rarity = val->asDouble();
                        item_is_ok = true;
                    }
                }
                else if (keyname == "life") {
                    if (val->isNumeric()) {
                        item->life = val->asInt();
                        item_is_ok = true;
                    }
                }
                else if (keyname == "consume") {
                    std::map<std::string, double> _consumes;
                    if (val->isObject()) {
                        for (std::string cmi : val->getMemberNames()) {
                            const json_value *cnt_val = m_state->m_data->getJsonValue(val, cmi);
                            if (cnt_val->isNumeric()) {
                                _consumes[cmi] = cnt_val->asDouble();
                            }
                        }
                    }
                    item->consumes = _consumes;
                    item_is_ok = true;
                }
                if (val->type == json_uinteger) item->setValue(keyname, val->asUInt());
                else if (val->type == json_integer) item->setValue(keyname, val->asInt());
                else if (val->type == json_double) item->setValue(keyname, val->asDouble());
                else if (val->type == json_string) item->setValue(keyname, val->asString());
                else if (val->type == json_boolean) item->setValue(keyname, val->asBool());
                else { //TODO unsupported
                    if (!item_is_ok) std::cout << keyname << "has unsupported type!\n";
                }
            }
            m_item_bases.push_back(std::move(item));
            // m_item_bases[mi] = item;
        }
    }
    m_ok = true;
}

bool Item::consume(std::unique_ptr<Item> another)
{
    if (!doesConsume(another->base)) return false;
    if (!another->isUsable()) return false;

    double cnt = consumeCount(another->base);

    if (cnt > another->amount) return false;

    // std::cout << base << " consumes " << cnt << "  " << another->base << "\n";
    stocks[another->base] += cnt;
    another->amount -= cnt;
    // std::cout << another->amount << "  " << another->base << "\n";

    return true;
}

std::unique_ptr<Item> Item::produce(double produce_amount)
{
    bool can_consume = true;
    for (auto val : consumes.get()) {
        if (stocks[val.first] < val.second) {
            can_consume = false;
        }
    }
    std::cout << base << "  " << can_consume << " rai" << produce_amount <<"\n";
    if (!can_consume) throw "Can't consume";

    std::unique_ptr<Item> produced (new Item(base));
    if (!produced.get()) {
        throw "Can't create item!";
        /*std::cout << "Can't create item!\n";
        exit(1);
        */
    }

    produced->assignItem(this);
    produced->usable = true;
    if (amount > 0) {
        produced->amount = produce_amount * amount;
    } else {
        produced->amount = produce_amount;
    }

    for (auto val : consumes.get()) {
        stocks[val.first] -= val.second;
    }
    return produced;
}

void ItemBase::assignItem(const ItemBase* parent)
{
    if (parent == nullptr) return;

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

bool ItemBase::equals(ItemBase *another) const
{
    return (base == another->base &&
            type == another->type &&
            rarity == another->rarity);
}

bool ItemBase::doesConsume(const std::string &name) const
{
    for (auto val : consumes.get()) {
        if (val.first == name) return true;
    }
    return false;
}

void ItemBase::updateConsume(const std::string &name, double count)
{
    std::map<std::string, double> new_consumes = consumes.get();
    new_consumes[name] = count;
    consumes = new_consumes;
}

void ItemBase::removeConsume(const std::string &name)
{
    std::map<std::string, double> new_consumes;
    for (auto val : consumes.get()) {
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
    for (auto val : consumes.get()) {
        res.push_back(val.first);
    }
    return res;
}

bool ItemBase::exhausted() const
{
    if (life == static_cast<long int>(-1)) return false;

    if (life == 0) return true;
    return false;
}

bool ItemBase::age(long int _amount)
{
    if (_amount <= 0) return false;
    if (life == static_cast<long int>(-1)) return true;

    if (life > _amount) {
        life -= _amount;
        return true;
    }

    life = 0;
    usable = false;
    return false;
}

bool ItemBase::hasStock() const
{
    for (auto val : stocks) {
        if (val.second > 0) return true;
    }
    return false;
}

double ItemBase::hasStock(const std::string &name) const
{
    for (auto val : stocks) {
        if (val.first == name) return val.second;
    }
    return 0;
}

double ItemBase::consumeCount(const std::string &name)
{
    for (auto val : consumes.get()) {
        if (val.first == name) return val.second;
    }
    return 0;
}

bool ItemBase::update(ItemBase *another)
{
    if (another == nullptr) return false;
    if (another->base != base) return false;
    if (another->type != type) return false;
    if (another->life == 0) return false;

    if (another->base != base) return false;

    amount += another->amount;
    another->amount = 0;
    life = another->life;

    for (auto val : another->stocks) {
        stocks[val.first] += val.second;
        val.second = 0;
    }

    return true;
}

void ItemBase::setupStock()
{
    stocks.clear();

    for (auto val : consumes.get()) {
        stocks[val.first] = 0;
    }
}

std::string ItemBase::itemToString() const
{
    std::string tmp = "";
    tmp += "base    : " + base.get() + "\n";
    tmp += "type    : " + type.get() + "\n";
    tmp += "usable  : " + std::string((usable?"yes":"no")) + "\n";
    if (life == static_cast<long int>(-1)) {
        tmp += "life    : inf\n";
    } else {
        tmp += "life    : " + (boost::format("%d") % life).str() + "\n";
    }
    tmp += "amount  : " + (boost::format("%f") % amount).str() + "\n";
    tmp += "rarity  : " + (boost::format("%f") % rarity).str() + "\n";

    tmp += "consumes: ";

    bool first = true;
    for (auto val : consumes.get()) {
        if (!first) tmp += ", ";
        first = false;
        tmp += (boost::format("%s (%f units)") % val.first % val.second).str();
    }
    tmp += "\n";

    tmp += "stock   : ";
    first = true;
    for (auto val : stocks) {
        if (!first) tmp += ", ";
        first = false;
        tmp += (boost::format("%s (%f units)") % val.first % val.second).str();
    }
    tmp += "\n";

    return tmp;
}

std::string Item::toString() const
{
    return blieng::BliObject::toString() + itemToString();
}

std::string ItemBase::generateJson(const std::string &indent) const
{
    std::list<std::string> res;
    if (type != "")
        res.push_back("\"type\": \"" + type.get() + "\"");
    if (image != "")
        res.push_back("\"image\": \"" + image.get() + "\"");
    if (amount != 0)
        res.push_back((boost::format("\"amount\": %f") % amount).str());
    if (rarity != 0)
        res.push_back((boost::format("\"rarity\": %f") % rarity).str());
    if (life != -1)
        res.push_back((boost::format("\"life\": %f") % life).str());

    std::string tmp = "\"consume\": {";
    bool first = true;
    bool got = false;
    for (auto val : consumes.get()) {
        if (!first) tmp += ", ";
        first = false;
        tmp += (boost::format("\"%s\": %f") % val.first % val.second).str();
        got = true;
    }
    tmp += "}";
    if (got) res.push_back(tmp);

    std::string res_str = indent + "\"" + base.get() + "\": {";
    first = true;
    for (std::string val : res) {
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
    for (auto val : m_item_bases) {
        if (!first) res += ",\n";
        first = false;
        res += val->generateJson("  ");
    }
    return "{\n" + res + "\n}\n";
}

std::unique_ptr<Item> Item::copy()
{
    std::unique_ptr<Item> res(new Item(m_state));
    res->assignItem(this);
    return res;
}
