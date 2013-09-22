#include "wallclock.h"
#include <boost/foreach.hpp>

using blieng::Wallclock;
using blieng::Item;

Wallclock::Wallclock(Item *time_producer) : time_producer(time_producer)
{
    // Start at zero
    wall_time = 0;
}

void Wallclock::forward(unsigned long int amount)
{
    wall_time += amount;
    produce(amount);
}

bool Wallclock::addProducer(std::unique_ptr<Item> item)
{
    if (!item.get()) return false;

    producers.push_back(std::move(item));

    return true;
}

void Wallclock::produceTime(unsigned long int amount)
{
    //auto_vector<Item> items; 
    BOOST_FOREACH(Item *item, producers) {
        if (time_producer != NULL) {
            std::unique_ptr<Item> time = time_producer->produce(amount);
            if (!time.get()) {
                std::cout << "Can't create time\n";
                exit(1);
            }
            if (item->doesConsume(time->base)) {
                item->consume(std::move(time));
            }
        }
        item->age(amount);
    }
}

#if 0
auto_vector<Item> Wallclock::produceTier1()
{
    // Generate first items
    BOOST_FOREACH(Item *item, producers) {
        int count = 0x1000;
        bool ok = true;
        do  {
            std::unique_ptr<Item> new_item = item->produce();
            if (new_item.get()) {
                items.push_back(new_item);
            } else {
                ok = false;
            }
        } while (ok && (--count)>0);
    }
    return items;
}
#endif

//auto_vector<Item> Wallclock::produceTier2(auto_vector<Item> items)
void Wallclock::produceTier2()
{
    // Go thorough the items
    BOOST_FOREACH(Item *item, items) {
        //Item *new_item = NULL;
        int count = 0x1000;
        bool ok = true;
        do  {
            std::unique_ptr<Item> new_item = item->produce();
            if (new_item.get()) {
                items.push_back(std::move(new_item)); //FIXME!
            } else {
                ok = false;
            }
        } while (ok && (--count)>0);
    }
    // Go thorough the producers
    BOOST_FOREACH(Item *item, producers) {
        int count = 0x1000;
        bool ok = true;
        do  {
            std::unique_ptr<Item> new_item = item->produce();
            if (new_item.get()) {
                items.push_back(std::move(new_item));
            } else {
                ok = false;
            }
        } while (ok && (--count)>0);
    }
    //return items;
}

bool Wallclock::consume()
{
    bool changed = false;
    BOOST_FOREACH(Item *item, items) {
        while (item->amount>0) {
            bool last_ok = false;
            std::unique_ptr<Item> in_need;
            //BOOST_FOREACH(const Item *item2, producers) {
            auto_vector<Item>::iterator it = producers.begin();
            while (it != producers.end()) {
                if (item == *it) continue;
                if ((*it)->doesConsume(item->base)) {
                    if (in_need.get()) {
                        in_need = producers.pop(it);
                        //in_need = item2;
                        it = producers.begin();
                    }
                    else if (in_need->hasStock(item->base) > (*it)->hasStock(item->base)) {
                        in_need = producers.pop(it);
                        it = producers.begin();
                    }
                }
                ++it;
            }
            if (in_need.get() &&
                in_need->consume(items.pop(item))) {
                changed = true;
                last_ok = true;
            }
            if (!last_ok) break;
        }
    }
    BOOST_FOREACH(Item *item, items) {
        while (item->amount>0) {
            bool last_ok = false;
            std::unique_ptr<Item> in_need;
            auto_vector<Item>::iterator it = producers.begin();
            while (it != producers.end()) {
                if (item == *it) continue;
                if ((*it)->doesConsume(item->base)) {
                    if (!in_need.get()) {
                        in_need = producers.pop(it);
                        it = producers.begin();
                        //in_need = item2;
                    } else if (in_need->hasStock(item->base) > (*it)->hasStock(item->base)) {
                        //in_need = item2;
                        in_need = producers.pop(it);
                        it = producers.begin();
                    }
                }
            }
            if (in_need.get() && in_need->consume(items.pop(item))) {
                changed = true;
                last_ok = true;
            }
            if (!last_ok) break;
        }
    }
    return changed;
}

void Wallclock::combineItems()
{
#if 0
    auto_vector<Item> new_items;
    BOOST_FOREACH(Item *item, items) {
        bool found = false;
        BOOST_FOREACH(Item *item2, new_items) {
            if (item->base == item2->base) {
                item2->update(item);
                found = true;
            }
        }
        if (!found) {
            new_items.push_back(item);
        }
    }
    return new_items;
#endif
}

void Wallclock::cleanItems()
{
#if 0
    auto_vector<Item> new_items;
    BOOST_FOREACH(Item *item, items) {
        if (item->exhausted()) {
            std::cout << item->base << " exhausted\n";
            // TODO: delete it?
        }
        else if (item->amount > 0) new_items.push_back(item);
        else if (item->hasStock()) new_items.push_back(item);
    }
    return new_items;
#endif
}

void Wallclock::produce(unsigned long int amount)
{
    // First produce time and generate time based products
    produceTime(amount);

    //auto_vector<Item> items;
    produceTier2();

    // Produce rest of the products, consume
    int counter = 0x1000;
    while (true && (--counter > 0)) {
        if (!consume()) break;
        //items = produceTier2(items);
        produceTier2();
    }
    
    Wallclock::combineItems();
    Wallclock::cleanItems();
    std::cout << "Produced: \n";
    BOOST_FOREACH(Item *item, producers) {
            std::cout << item->toString() << "\n";
    }
    BOOST_FOREACH(Item *item, items) {
            std::cout << item->toString() << "\n";
    }

    //return items;
}
