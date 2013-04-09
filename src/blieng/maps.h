#ifndef __MAPS_H
#define __MAPS_H

#include "bliobject.h"
#include "item.h"
#include "character.h"

namespace blieng
{

class Maps : public BliObject
{
public:
	Maps(std::string mapname);

private:
	void loadMap(std::string name);

	std::string mapname;
	std::string mapfile;
};

}
#endif
