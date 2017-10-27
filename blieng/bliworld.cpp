#include "bliworld.h"

using blieng::BliWorld;

BliWorld::BliWorld(float gravity1, float gravity2)
    : world(b2World(b2Vec2(gravity1, gravity2)))
{
}

BliWorld::~BliWorld()
{
}
