/*
 * Copyright 2014-2018 Jouni Roivas
 */

#pragma once

#include "blieng/bliobject.h"
#include "Box2D/Box2D.h"

namespace blieng
{

class BliBody;

/**
 * World to be simulated
 */
class BliWorld : public BliObject
{
public:
    /**
     * Every world needs gravity, however it can be zero
     *
     * \param gravityX The applied gravity for X axis (horizontal)
     * \param gravityY The applied gravity for Y axis (vertical)
     */
    BliWorld(float gravityX, float gravityY);
    virtual ~BliWorld();

    friend class BliBody;

    /**
     * Get gravity for X axis
     *
     * \returns Horizontal gravity
     */
    float getGravityX();
    /**
     * Get gravity for Y axis
     *
     * \returns Vertical gravity
     */
    float getGravityY();

    /**
     * Set orthological view of the world.
     * By default it's (-10, -10), (10, 10) but can be changed
     * to suit better one's needs
     *
     * \param left Left boundary of X axis
     * \param right Right boundary of X axis
     * \param bottom Bottom boundary of Y axis
     * \param top Top boundary of Y axis
     */
    void setOrtho(float left, float right, float bottom, float top);
    /**
     * Set viewport size.
     * Viewport is expressed in pixels.
     * Used to convert ortohological view to viewport pixels and vise versa
     *
     * \param width Viewport width in pixels
     * \param height Viewport height in pixels
     */
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

    /**
     * Set pixel conversion rate, how many pixels is one meter
     *
     * \param pixelsPerMeter How many pixels is in a meter, default 100
     */
    void setPixelConversion(float pixelsPerMeter);
    /**
     * Convert pixels to meters
     *
     * \param pixels Distance in pixels
     * \returns Distance in meters, using conversion rate
     */
    float pixelsToMeters(float pixels);
    /**
     * Convert meters to pixels
     *
     * \param meters Distance in meters
     * \returns Distance in pixels, using conversion rate
     */
    float metersToPixels(float meters);

    /**
     * Convert screen position to othological position
     *
     * \param pixels X position on screen in pixels
     * \returns X position on orthological view
     */
    float screenToX(float pixels);
    /**
     * Convert screen position to othological position
     *
     * \param pixels Y position on screen in pixels
     * \returns Y position on orthological view
     */
    float screenToY(float pixels);
    /**
     * Convert othological position to screen position
     *
     * \param pixels X position on orthological view
     * \returns X position on screen in pixels
     */
    float XToScreen(float pixels);
    /**
     * Convert othological position to screen position
     *
     * \param pixels Y position on orthological view
     * \returns Y position on screen in pixels
     */
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
