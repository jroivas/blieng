#include "blibody.h"

using blieng::BliBody;

BliBody::BliBody(b2BodyType bodyType, BliWorld *w, float x, float y) :
    BliObject(),
    world(w),
    shape(nullptr)
{
    bodydef.type = bodyType;
    bodydef.position.Set(x, y);
    body = world->world.CreateBody(&bodydef);
}

BliBody::~BliBody()
{
    removeFromWorld();
    //TODO: delete body;
}

void BliBody::removeFromWorld()
{
    body->GetWorld()->DestroyBody(body);
}

void BliBody::setPosition(float x, float y)
{
    b2Vec2 pos(x, y);
    body->SetTransform(pos, 0);
}

void BliBody::setAngle(float angle)
{
    body->SetTransform(body->GetPosition(), angle);
}

float BliBody::posX()
{
    return body->GetPosition().x;
}

float BliBody::posY()
{
    return body->GetPosition().y;
}

float BliBody::angle()
{
    return body->GetAngle();
}

void BliBody::applyForce(float forceX, float forceY)
{
    body->ApplyForce(b2Vec2(forceX, forceY), body->GetWorldCenter(), true);
}

void BliBody::applyImpulse(float forceX, float forceY)
{
    body->ApplyLinearImpulse(b2Vec2(forceX, forceY), body->GetWorldCenter(), true);
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

void BliBody::setFixture(float density, float friction, float restitution)
{
    if (shape == nullptr) throw "No shape for body!";
    b2FixtureDef fixtureDef;
    fixtureDef.shape = shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    body->CreateFixture(&fixtureDef);
}

void BliBody::setActive()
{
    body->SetActive(true);
}

void BliBody::setPassive()
{
    body->SetActive(false);
}

/*
void BliBody::setMass(float mass)
{
    
}
*/
