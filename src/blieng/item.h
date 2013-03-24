#ifndef __ITEM_H
#define __ITEM_H

#include <string>
#include <vector>
#include <boost/random/random_device.hpp>

namespace blieng {

class Item
{
public:
	Item();

	bool consume(Item *);
	Item *produce();

private:
	int getRandomInt(int limit_low, int limit_max);
	void getItemBases();

	std::string type;
	int rarity;
	std::vector<std::string> consumes;

	boost::random::random_device *gen;
};

}

#endif
