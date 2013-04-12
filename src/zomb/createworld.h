#ifndef __CREATE_WORLD_H
#define __CREATE_WORLD_H

#include "blieng/maps.h"

namespace zomb
{

class CreateWorld
{
public:
	CreateWorld(blieng::Maps *maps);

private:
	void initialize();
	blieng::Maps *maps;
};

}

#endif
