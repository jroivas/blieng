#ifndef __BLIENG_WALLCLOCK_H
#define __BLIENG_WALLCLOCK_H

#include <vector>
#include "item.h"
#include "auto_vector.h"

using std::auto_ptr;

namespace blieng
{

class Wallclock
{
public:
    Wallclock(Item *time_producer);
    void forward(unsigned long int amount=1);
    bool addProducer(auto_ptr<Item> item);

private:
    //auto_vector<Item> produceTime(unsigned long int amount);
    void produceTime(unsigned long int amount);
    //auto_vector<Item> produceTier1();
    //auto_vector<Item> produceTier2(auto_vector<Item> items);
    void produceTier2();
    void cleanItems();
    void combineItems();
    void produce(unsigned long int amount);
    bool consume();

    //auto_vector<Item> items;
    auto_vector<Item> items;


    auto_vector<Item> producers;
    Item *time_producer;
    unsigned long int wall_time;
};

}

#endif
