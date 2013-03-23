#ifndef __PLAYER_CHARACTER
#define __PLAYER_CHARACTER

#include <string>
#include "character.h"

#include <boost/random/random_device.hpp>
#include <boost/filesystem.hpp>

class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	void roll();

private:
	void generateName();
	void readNames();
	boost::filesystem::path *findDataPath();

	std::list<std::string> first_names;
	std::list<std::string> last_names;

	boost::random::random_device *gen;
};

#endif
