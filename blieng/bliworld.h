#pragma once

#include "blieng/bliobject.h"
#include "Box2D/Box2D.h"

namespace blieng
{

class BliBody;

class BliWorld : public BliObject
{
public:
    BliWorld(float gravity1, float gravity2);
    virtual ~BliWorld();

    friend class BliBody;

    float getGravityX();
    float getGravityY();

protected:
    b2World world;
};

}
