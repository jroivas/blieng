#ifndef __PLAYER_CHARACTER
#define __PLAYER_CHARACTER

#include <string>
#include <vector>
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
	boost::filesystem::path *findDataPath();
	std::vector<std::string> readNamesFromFile(boost::filesystem::path *data_path, std::string name);
	void readNames();

	std::vector<std::string> first_names;
	std::vector<std::string> last_names;

	boost::random::random_device *gen;
};

#endif
