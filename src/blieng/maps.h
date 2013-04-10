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

private:
	void loadMap(std::string name);
	void parseMap();

	std::string map_name;
	std::string map_file;
	Json::Value map_json;

	std::string map_image_file;
	std::vector<Town *> towns;
};

}
#endif
