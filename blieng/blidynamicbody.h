#pragma once

#include "blieng/bliobject.h"
#include "blieng/bliworld.h"
#include "Box2D/Box2D.h"

namespace blieng
{

class BliDynamicBody : public BliObject
{
public:
    BliDynamicBody(BliWorld *world, float x, float y);
    virtual ~BliDynamicBody();

    //void setMass(float mass);
    void setPosition(float x, float y);
    void setCircle(float x, float y, float radius);
    void setBox(float width, float height);
    void setFixture(float density, float friction);

private:
    BliWorld *world;
    b2Shape *shape;
    b2BodyDef bodydef;
    b2Body *body;
};

}
