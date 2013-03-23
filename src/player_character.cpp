#include "player_character.h"
#include <string>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

PlayerCharacter::PlayerCharacter()
{
	gen = new boost::random::random_device();
	roll();
}

void PlayerCharacter::roll()
{
	generateName();
}

boost::filesystem::path *PlayerCharacter::findDataPath()
{
	std::list<std::string> locations;
	locations.push_back("");
	locations.push_back("/usr/share/zombiebli");
	locations.push_back("../");
	std::list<std::string>::iterator li = locations.begin();

	boost::filesystem::path *data_path = new boost::filesystem::path;
	while (li != locations.end()) {
		*data_path = (*li + "data");
		if (boost::filesystem::exists(*data_path) && boost::filesystem::is_directory(*data_path)) {
			return data_path;
		}
		li++;
	}
	delete data_path;
	return NULL;
}

std::vector<std::string> PlayerCharacter::readNamesFromFile(boost::filesystem::path *data_path, std::string name)
{
	std::vector<std::string> tmp;
	if (data_path == NULL) return tmp;
	
	boost::filesystem::path first_path = *data_path;
	first_path += "/" + name;
	if (boost::filesystem::exists(first_path)) {
		boost::filesystem::ifstream fd(first_path);
		while (!fd.eof()) {
			std::string line;
			std::getline(fd, line);
			boost::algorithm::trim(line);
			if (line.length() > 0) {
				tmp.push_back(line);
			}
		}
		fd.close();
	}

	return tmp;
}
void PlayerCharacter::readNames()
{
	boost::filesystem::path *data_path = findDataPath();
	first_names = readNamesFromFile(data_path, "first_names");
	last_names = readNamesFromFile(data_path, "last_names");
}

void PlayerCharacter::generateName()
{
	readNames();
	std::string name;

	boost::random::uniform_int_distribution<> first_dist(0, first_names.size());
	boost::random::uniform_int_distribution<> last_dist(0, last_names.size());
	name = first_names.at(first_dist(*gen));
	name += " " + last_names.at(last_dist(*gen));

	setValue("name", name);
}
