#include "blidynamicbody.h"

using blieng::BliDynamicBody;

BliDynamicBody::BliDynamicBody(BliWorld *w, float x, float y) :
    BliObject(),
    world(w),
    shape(nullptr)
{
    bodydef.type = b2_dynamicBody;
    bodydef.position.Set(x, y);
    body = world->world.CreateBody(&bodydef);
}

BliDynamicBody::~BliDynamicBody()
{
}

void BliDynamicBody::setPosition(float x, float y)
{
    b2Vec2 pos(x, y);
    body->SetTransform(pos, 0);
}

void BliDynamicBody::setCircle(float x, float y, float radius)
{
    b2CircleShape *circle = new b2CircleShape;
    circle->m_p.Set(x, y);
    circle->m_radius = radius;
    shape = circle;
}

void BliDynamicBody::setBox(float w, float h)
{
    b2PolygonShape *box = new b2PolygonShape;
    box->SetAsBox(w, h);
    shape = box;
}

void BliDynamicBody::setFixture(float density, float friction)
{
    b2FixtureDef fixtureDef;
    fixtureDef.shape = shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    body->CreateFixture(&fixtureDef);
}

/*
void BliDynamicBody::setMass(float mass)
{
    
}
*/
