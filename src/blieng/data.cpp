#include "data.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

using blieng::Data;

Data *Data::__data_instance = NULL;

Data *Data::getInstance()
{
	if (__data_instance == NULL) {
		__data_instance = new Data();
	}
	return __data_instance;
}

Data::Data()
{
	data_path = findDataPath();
}

boost::filesystem::path *Data::findDataPath()
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

std::vector<std::string> Data::readLinesFromFile(std::string name)
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
