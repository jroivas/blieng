#include "bliphysics_test.h"
#include "test_tools.h"

#include <bliobject.h>
#include <bliworld.h>
#include <blidynamicbody.h>
#include <blistaticbody.h>

CPPUNIT_TEST_SUITE_REGISTRATION( BliPhysicsTest );

using blieng::BliWorld;
using blieng::BliDynamicBody;
using blieng::BliStaticBody;

void BliPhysicsTest::setUp()
{
}

void BliPhysicsTest::tearDown()
{
}

void BliPhysicsTest::world()
{
    BliWorld world(0.2f, -10.0f);

    CPPUNIT_ASSERT_EQUAL(0.2f, world.getGravityX());
    CPPUNIT_ASSERT_EQUAL(-10.0f, world.getGravityY());
}

void BliPhysicsTest::dynamicbody()
{
    BliWorld *world = new BliWorld(0.0, -9.81);
    BliDynamicBody body(world, 1.0f, 2.0f);
    body.setCircle(0, 0, 0.5);
    body.setFixture(1.0, 0.3);

    CPPUNIT_ASSERT_EQUAL(1.0f, body.posX());
    CPPUNIT_ASSERT_EQUAL(2.0f, body.posY());

    body.setPosition(-1.0f, 5.0f);
    CPPUNIT_ASSERT_EQUAL(-1.0f, body.posX());
    CPPUNIT_ASSERT_EQUAL(5.0f, body.posY());
}

void BliPhysicsTest::staticbody()
{
    BliWorld *world = new BliWorld(0.0, -9.81);
    BliStaticBody body(world, 1, 2);
    body.setBox(50.0, 10.0f);
    body.setFixture(1.0, 0.3);

    CPPUNIT_ASSERT_EQUAL(1.0f, body.posX());
    CPPUNIT_ASSERT_EQUAL(2.0f, body.posY());
}

void BliPhysicsTest::simulation()
{

    BliWorld *world = new BliWorld(0.0f, -9.81f);

    BliStaticBody ground(world, 0, 0);
    ground.setBox(50.0f, 10.0f);
    ground.setFixture(0.0, 0.0);

    BliDynamicBody ball(world, 1.0f, 3.0f);
    ball.setCircle(0, 0, 0.5f);
    ball.setFixture(1.0f, 0.3f);

    for (int i = 0; i < 60; i++) {
        world->step();
    }
}
