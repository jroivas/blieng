#ifndef __PLAYER_CHARACTER
#define __PLAYER_CHARACTER

#include <string>
#include <vector>
#include "character.h"
#include "data.h"

#include <boost/random/random_device.hpp>

namespace blieng {

class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	void roll();

private:
	// Generators
	void generateName();
	void generateHealth();

	void readNames();
	int getRandomInt(int limit_low, int limit_max);

	std::vector<std::string> first_names;
	std::vector<std::string> last_names;

	boost::random::random_device *gen;
};

}

#endif
