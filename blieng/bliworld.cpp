#include "bliworld.h"

using blieng::BliWorld;

BliWorld::BliWorld(float gravityX, float gravityY)
    : world(b2World(b2Vec2(gravityX, gravityY))),
      timeStep(1.0f / 60.0f)
{
    // FIXME defaults
    setPixelConversion(100);
    setOrtho(-10, 10, -10, 10);
    setViewport(640, 480);
}

BliWorld::~BliWorld()
{
}

void BliWorld::setOrtho(float l, float r, float b, float t)
{
    left = l;
    right = r;
    bottom = b;
    top = t;
}

void BliWorld::setViewport(float w, float h)
{
    width = w;
    height = h;
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

float BliWorld::screenToX(float pixels)
{
    return left + pixels / width * (right - left);
}

float BliWorld::screenToY(float pixels)
{
    return top - pixels / height * (top - bottom);
}

float BliWorld::XToScreen(float pixels)
{
    return (left + pixels) * width / (right - left);
}

float BliWorld::YToScreen(float pixels)
{
    return (top - pixels) * height / (top - bottom);
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
