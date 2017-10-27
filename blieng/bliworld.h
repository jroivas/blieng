#pragma once

#include "blieng/bliobject.h"
#include "Box2D/Box2D.h"

namespace blieng
{

class BliDynamicBody;

class BliWorld : public BliObject
{
public:
    BliWorld(float gravity1, float gravity2);
    virtual ~BliWorld();

    friend class BliDynamicBody;

protected:
    b2World world;
};

}
