#include "blistaticbody.h"
#include "Box2D/Box2D.h"

using blieng::BliStaticBody;

BliStaticBody::BliStaticBody(BliWorld *world, float x, float y)
    : BliBody(b2_staticBody, world, x, y)
{
}

BliStaticBody::~BliStaticBody()
{
}
