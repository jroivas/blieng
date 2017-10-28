#include "bliworld.h"

using blieng::BliWorld;

BliWorld::BliWorld(float gravityX, float gravityY)
    : world(b2World(b2Vec2(gravityX, gravityY))),
      timeStep(1.0f / 60.0f)
{
    // FIXME defaults
    setPixelConversion(100);
}

BliWorld::~BliWorld()
{
}

float BliWorld::getGravityX()
{
    return world.GetGravity().x;
}

float BliWorld::getGravityY()
{
    return world.GetGravity().y;
}

void BliWorld::setPixelConversion(float p)
{
    pixelsPerMeter = p;
    metersPerPixel = 1.0f / p;
}

float BliWorld::pixelsToMeters(float pixels)
{
    return metersPerPixel * pixels;
}

float BliWorld::metersToPixels(float meters)
{
    return pixelsPerMeter * meters;
}

void BliWorld::setFrequency(float hertz)
{
    // Default 60Hz anyway
    if (hertz == 0) timeStep = 1.0f / 60.0f;
    else timeStep = 1.0f / hertz;
}

void BliWorld::step(int velocityIterations, int positionIterations)
{
    world.Step(timeStep, velocityIterations, positionIterations);
}
