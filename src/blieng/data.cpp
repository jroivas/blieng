#include "data.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

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

	boost::filesystem::path *data_path = new boost::filesystem::path;
	BOOST_FOREACH(std::string item, locations) {
		*data_path = (item + "data");
		if (boost::filesystem::exists(*data_path) && boost::filesystem::is_directory(*data_path)) {
			return data_path;
		}
		
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

std::string Data::readString(std::string name)
{
	std::string res = "";
	if (data_path == NULL) return res;
	
	boost::filesystem::path first_path = *data_path;
	first_path += "/" + name;
	if (boost::filesystem::exists(first_path)) {
		boost::filesystem::ifstream fd(first_path, std::ifstream::binary);
		while (!fd.eof()) {
			char tmp[256];
			fd.read(tmp, 255);
			tmp[255] = 0;
			res += tmp;
		}
		fd.close();
	}

	return res;
	
}

Json::Value Data::readJson(std::string name)
{
	std::string datas = Data::getInstance()->readString(name);

	Json::Reader reader;
	Json::Value val;
	bool parse_ok;
	parse_ok = reader.parse(datas, val);
	if (!parse_ok) {
		std::cout << "Parse error: " << reader.getFormattedErrorMessages() << "!\n";
		throw "JSON parse error";	
	}
	return val;
}

std::vector<std::string> Data::getJsonKeys(Json::Value val)
{
	return val.getMemberNames();
}

bool Data::isJsonKey(Json::Value val, std::string key)
{
	if (val.isObject()) {
		return val.isMember(key);
	}
	return false;
}

Json::Value Data::getJsonValue(Json::Value val, std::string key)
{
	if (val.isObject()) {
		if (val.isMember(key)) {
			Json::Value res;
			return val.get(key, res);
		}
	}
	return val;
}
