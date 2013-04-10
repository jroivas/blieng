#ifndef __MAPS_H
#define __MAPS_H

#include "bliobject.h"
#include "item.h"
#include "character.h"
#include "town.h"
#include <json/reader.h>

namespace blieng
{

class Maps : public BliObject
{
public:
	Maps(std::string mapname);

	std::string getMapImageFile() { return map_image_file; }
	std::string getSolvedMapImageFile();
	std::vector<blieng::Town *> getTowns() { return towns; }

private:
	void loadMap(std::string name);
	void parseMap();

	std::string map_name;
	std::string map_file;
	Json::Value map_json;

	std::string map_image_file;
	std::string solved_map_image_file;
	std::vector<blieng::Town *> towns;
};

}
#endif
