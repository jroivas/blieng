#ifndef __ZOMB_CREATEWORLD_H
#define __ZOMB_CREATEWORLD_H

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
