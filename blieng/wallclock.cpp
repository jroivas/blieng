#include "wallclock.h"
#include <boost/foreach.hpp>

using blieng::Wallclock;

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

bool Wallclock::addProducer(Item *item)
{
    if (item == NULL) return false;

    producers.push_back(item);

    return true;
}

void Wallclock::produceTime(unsigned long int amount)
{
    std::vector<Item *> items; 
    BOOST_FOREACH(Item *item, producers) {
        Item *time = NULL;
        if (time_producer != NULL) {
            time = time_producer->produce(amount);
            if (time == NULL) {
                std::cout << "Can't create time\n";
                exit(1);
            }
            if (item->doesConsume(time->base)) {
                item->consume(time);
            }
        }
        item->age(amount);
    }
}

std::vector<blieng::Item *> Wallclock::produceTier1()
{
    // Generate first items
    BOOST_FOREACH(Item *item, producers) {
        Item *new_item = NULL;
        int count = 0x1000;
        do  {
            new_item = item->produce();
            if (new_item != NULL) {
                items.push_back(new_item);
            }
        } while (new_item != NULL && (--count)>0);
    }
    return items;
}

std::vector<blieng::Item *> Wallclock::produceTier2(std::vector<Item *> items)
{
    // Go thorough the items
    BOOST_FOREACH(Item *item, items) {
        Item *new_item = NULL;
        int count = 0x1000;
        do  {
            new_item = item->produce();
            if (new_item != NULL) {
                items.push_back(new_item);
            }
        } while (new_item != NULL && (--count)>0);
    }
    // Go thorough the producers
    BOOST_FOREACH(Item *item, producers) {
        Item *new_item = NULL;
        int count = 0x1000;
        do  {
            new_item = item->produce();
            if (new_item != NULL) {
                items.push_back(new_item);
            }
        } while (new_item != NULL && (--count)>0);
    }
    return items;
}

bool Wallclock::consume(std::vector<Item *> items)
{
    bool changed = false;
    BOOST_FOREACH(Item *item, items) {
        while (item->amount>0) {
            bool last_ok = false;
            Item *in_need = NULL;
            BOOST_FOREACH(Item *item2, producers) {
                if (item == item2) continue;
                if (item2->doesConsume(item->base)) {
                    if (in_need == NULL) in_need = item2;
                    else if (in_need->hasStock(item->base) > item2->hasStock(item->base)) {
                        in_need = item2;
                    }
                }
            }
            if (in_need != NULL && in_need->consume(item)) {
                changed = true;
                last_ok = true;
            }
            if (!last_ok) break;
        }
    }
    BOOST_FOREACH(Item *item, items) {
        while (item->amount>0) {
            bool last_ok = false;
            Item *in_need = NULL;
            BOOST_FOREACH(Item *item2, producers) {
                if (item == item2) continue;
                if (item2->doesConsume(item->base)) {
                    if (in_need == NULL) in_need = item2;
                    else if (in_need->hasStock(item->base) > item2->hasStock(item->base)) {
                        in_need = item2;
                    }
                }
            }
            if (in_need != NULL && in_need->consume(item)) {
                changed = true;
                last_ok = true;
            }
            if (!last_ok) break;
        }
    }
    return changed;
}

std::vector<blieng::Item *> Wallclock::combineItems(std::vector<Item *> items)
{
    std::vector<blieng::Item *> new_items;
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
}

std::vector<blieng::Item *> Wallclock::cleanItems(std::vector<Item *> items)
{
    std::vector<blieng::Item *> new_items;
    BOOST_FOREACH(Item *item, items) {
        if (item->exhausted()) {
            std::cout << item->base << " exhausted\n";
            // TODO: delete it?
        }
        else if (item->amount > 0) new_items.push_back(item);
        else if (item->hasStock()) new_items.push_back(item);
    }
    return new_items;
}


std::vector<blieng::Item *> Wallclock::produce(unsigned long int amount)
{
    // First produce time and generate time based products
    produceTime(amount);

    //std::vector<Item *> items;
    items = produceTier2(items);

    // Produce rest of the products, consume
    int counter = 0x1000;
    while (true && (--counter > 0)) {
        if (!consume(items)) break;
        items = produceTier2(items);
    }
    
    items = Wallclock::combineItems(items);
    items = Wallclock::cleanItems(items);
    std::cout << "Produced: \n";
    BOOST_FOREACH(Item *item, producers) {
            std::cout << item->toString() << "\n";
    }
    BOOST_FOREACH(Item *item, items) {
            std::cout << item->toString() << "\n";
    }

    return items;
}
