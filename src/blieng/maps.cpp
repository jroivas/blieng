#include "maps.h"
#include "data.h"
#include <json/reader.h>

using blieng::Maps;

Maps::Maps(std::string mapname)
{
	loadMap(mapname);
}

void Maps::loadMap(std::string name)
{
	mapname = name;
	mapfile = Data::getInstance()->findFile(name + ".json");
	if (mapfile != "") {
		std::cout << mapfile << "\n";
		Json::Value res = Data::getInstance()->readJson(mapfile);
	}
}
