#pragma once

#include "blieng/bliobject.h"
#include "blieng/physics/bliworld.h"
#include "Box2D/Box2D.h"

namespace blieng
{

class BliBody : public BliObject
{
protected:
    // This class should only be inherited, not to use as-is
    BliBody(b2BodyType bodyType, BliWorld *world, float x=0.0f, float y=0.0f);

public:
    virtual ~BliBody();

    //void setMass(float mass);
    void setPosition(float x, float y);
    void setCircle(float posx, float posy, float radius);
    void setBox(float width, float height);
    void setFixture(float density, float friction, float restitution=0.0);
    void setAngle(float angle);

    void removeFromWorld();

    float posX();
    float posY();
    float angle();

    void applyForce(float forceX, float forceY);
    void applyImpulse(float forceX, float forceY);

    void setActive();
    void setPassive();

protected:
    BliWorld *world;
    b2Shape *shape;
    b2BodyDef bodydef;
    b2Body *body;
};

}
