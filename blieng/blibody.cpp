#include "blibody.h"

using blieng::BliBody;

BliBody::BliBody(b2BodyType bodyType, BliWorld *w, float x, float y) :
    BliObject(),
    world(w),
    shape(nullptr)
{
    //bodydef.type = b2_dynamicBody;
    bodydef.type = bodyType;
    bodydef.position.Set(x, y);
    body = world->world.CreateBody(&bodydef);
}

BliBody::~BliBody()
{
}

void BliBody::setPosition(float x, float y)
{
    b2Vec2 pos(x, y);
    body->SetTransform(pos, 0);
}

float BliBody::posX()
{
    return body->GetPosition().x;
}

float BliBody::posY()
{
    return body->GetPosition().y;
}

void BliBody::setCircle(float x, float y, float radius)
{
    b2CircleShape *circle = new b2CircleShape;
    circle->m_p.Set(x, y);
    circle->m_radius = radius;
    shape = circle;
}

void BliBody::setBox(float w, float h)
{
    b2PolygonShape *box = new b2PolygonShape;
    box->SetAsBox(w, h);
    shape = box;
}

void BliBody::setFixture(float density, float friction)
{
    b2FixtureDef fixtureDef;
    fixtureDef.shape = shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    body->CreateFixture(&fixtureDef);
}

/*
void BliBody::setMass(float mass)
{
    
}
*/
