#ifndef __WALLCLOCK_H
#define __WALLCLOCK_H

#include <vector>
#include "item.h"

namespace blieng {

class Wallclock
{
public:
	Wallclock(Item *time_producer);
	void forward(unsigned long int amount=1);
	bool addProducer(Item *item);

private:
	//std::vector<blieng::Item *> produceTime(unsigned long int amount);
	void produceTime(unsigned long int amount);
	std::vector<blieng::Item *> produceTier1();
	std::vector<blieng::Item *> produceTier2(std::vector<Item *> items);
	std::vector<blieng::Item *> cleanItems(std::vector<Item *> items);
	std::vector<blieng::Item *> combineItems(std::vector<Item *> items);
	std::vector<Item *> produce(unsigned long int amount);
	bool consume(std::vector<Item *> items);

	std::vector<Item *> items;

	std::vector<Item *> producers;
	Item *time_producer;
	unsigned long int wall_time;
};

}

#endif
