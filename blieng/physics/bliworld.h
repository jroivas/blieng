#pragma once

#include "blieng/bliobject.h"
#include "Box2D/Box2D.h"

namespace blieng
{

class BliBody;

class BliWorld : public BliObject
{
public:
    BliWorld(float gravityX, float gravityY);
    virtual ~BliWorld();

    friend class BliBody;

    float getGravityX();
    float getGravityY();

    void setOrtho(float left, float right, float bottom, float top);
    void setViewport(float width, float height);

    /**
     * Set the frequenzy in hertz the world is simulated.
     * Default is 60Hz
     *
     * \param hertz Frequenzy in hertz, default 60Hz
     */
    void setFrequency(float hertz=60.0f);

    /**
     * Step the world forward.
     *
     * \param velocityIterations Constant for velocity solver
     * \param positionIterations Constant for position solver
     */
    void step(int velocityIterations = 6, int positionIterations = 2);

    void setPixelConversion(float pixelsPerMeter);
    float pixelsToMeters(float pixels);
    float metersToPixels(float meters);

    float screenToX(float pixels);
    float screenToY(float pixels);
    float XToScreen(float pixels);
    float YToScreen(float pixels);

protected:
    b2World world;
    float timeStep;
    float pixelsPerMeter;
    float metersPerPixel;
    float width;
    float height;
    float left;
    float right;
    float top;
    float bottom;
};

}