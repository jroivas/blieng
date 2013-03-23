#include "player_character.h"
#include <string>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/filesystem/fstream.hpp>

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
	//, "../", "/usr/share/zombiebli/"}
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

void PlayerCharacter::readNames()
{
	boost::filesystem::path *data_path = findDataPath();
	if (data_path == NULL) return;

	
	boost::filesystem::path first_path = *data_path;
	first_path += "/first_names";
	if (boost::filesystem::exists(first_path)) {
		boost::filesystem::ifstream fd(first_path);
		while (!fd.eof()) {
			std::string line;
			std::getline(fd, line);
			std::cout << "got: " << line << "\n";
			first_names.push_back(line);
		}
		fd.close();
	}
}

void PlayerCharacter::generateName()
{
	std::string name;

	boost::random::uniform_int_distribution<> dist((int)'a', (int)'z');
	for (int i=0; i<6; i++) {
		name += (char)dist(*gen);
	}
	setValue("name", name);
}
