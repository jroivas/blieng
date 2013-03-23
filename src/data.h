#ifndef __DATA_H
#define __DATA_H

#include <boost/filesystem.hpp>
#include <vector>

class Data
{
public:
	static Data *getInstance();
	std::vector<std::string> readLinesFromFile(std::string name);

private:
	Data();
	boost::filesystem::path *findDataPath();
	boost::filesystem::path *data_path;
	static Data *__data_instance;
};


#endif
