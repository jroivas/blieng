#ifndef __DATA_H
#define __DATA_H

#include <boost/filesystem.hpp>
#include <vector>
#include <json/reader.h>

namespace blieng {

class Data
{
public:
	static Data *getInstance();
	std::string readString(std::string name);
	std::vector<std::string> readLinesFromFile(std::string name);
	Json::Value readJson(std::string name);
	
	std::vector<std::string> getJsonKeys(Json::Value val);
	bool isJsonKey(Json::Value val, std::string key);
	Json::Value getJsonValue(Json::Value val, std::string key);

private:
	Data();
	boost::filesystem::path *findDataPath();
	boost::filesystem::path *data_path;
	static Data *__data_instance;
};

}

#endif
