#include "blidynamicbody.h"
#include "Box2D/Box2D.h"

using blieng::BliDynamicBody;

BliDynamicBody::BliDynamicBody(BliWorld *world, float x, float y)
    : BliBody(b2_dynamicBody, world, x, y)
{
}

BliDynamicBody::~BliDynamicBody()
{
}
