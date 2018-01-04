/*
 * Copyright 2014-2018 Jouni Roivas
 */

#pragma once

#include "blieng/bliobject.h"
#include "blieng/physics/bliworld.h"
#include "Box2D/Box2D.h"

namespace blieng
{

/**
 * Base body for physics simulations.
 *
 * All other bodies in simulation should inherit from this one.
 * This class should only be inherited, not to use as-is,
 * thus only protected constructor.
 * If wanted body type is missing, it's easy to implement by inheriting.
 */
class BliBody : public BliObject
{
protected:
    BliBody(b2BodyType bodyType, BliWorld *world, float x=0.0f, float y=0.0f);

public:
    virtual ~BliBody();

    //void setMass(float mass);
    /**
     * Set body position in world
     *
     * \param x Body position in X axis
     * \param y Body position in Y axis
     */
    void setPosition(float x, float y);
    /**
     * Set body to be circle
     *
     * \param posx Body center in X axis (usually 0 to center of circle)
     * \param posy Body center in Y axis (usually 0 to center of circle)
     * \param radius Body radius from center of circle
     */
    void setCircle(float posx, float posy, float radius);
    /**
     * Set body to be a box
     *
     * \param width Box width
     * \param width Box height
     */
    void setBox(float width, float height);
    /**
     * Set fixture of body
     *
     * \param density The density of body
     * \param friction Friction of body
     * \param restitution Restitution of body
     */
    void setFixture(float density, float friction, float restitution=0.0);
    /**
     * Set body angle
     *
     * \param angle Body angle in radians
     */
    void setAngle(float angle);

    /**
     * Unregister body from world
     */
    void removeFromWorld();

    /**
     * Get body X axis position in world
     */
    float posX();
    /**
     * Get body Y axis position in world
     */
    float posY();
    /**
     * Get body angle
     */
    float angle();

    /**
     * Apply force for body, to make it move (if allowed by body type)
     *
     * \param forceX The force applied to X axis
     * \param forceY The force applied to Y axis
     */
    void applyForce(float forceX, float forceY);
    /**
     * Apply sudden impulse for body, to make it move (if allowed by body type)
     *
     * \param forceX The force applied to X axis
     * \param forceY The force applied to Y axis
     */
    void applyImpulse(float forceX, float forceY);

    /**
     * Set body to be active and take part in simulation
     */
    void setActive();
    /**
     * Set body to be passive and be excluded from simulation
     */
    void setPassive();

protected:
    BliWorld *world;
    b2Shape *shape;
    b2BodyDef bodydef;
    b2Body *body;
};

}
