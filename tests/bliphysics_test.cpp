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

    CPPUNIT_ASSERT_EQUAL(1.0f, world.pixelsToMeters(100));
    CPPUNIT_ASSERT_EQUAL(0.5f, world.pixelsToMeters(50));
    CPPUNIT_ASSERT_EQUAL(0.25f, world.pixelsToMeters(25));
    CPPUNIT_ASSERT_EQUAL(500.0f, world.metersToPixels(5));

    world.setPixelConversion(50.0f);
    CPPUNIT_ASSERT_EQUAL(2.0f, world.pixelsToMeters(100));
    CPPUNIT_ASSERT_EQUAL(1.0f, world.pixelsToMeters(50));
    CPPUNIT_ASSERT_EQUAL(0.5f, world.pixelsToMeters(25));
    CPPUNIT_ASSERT_EQUAL(250.0f, world.metersToPixels(5));
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

    BliWorld *world = new BliWorld(0.0f, -10.0f);

    BliStaticBody ground(world, 0, -10.0f);
    ground.setBox(50.0f, 10.0f);
    ground.setFixture(0.0, 0.0);

    BliDynamicBody ball(world, 0.0f, 4.0f);
    ball.setCircle(0, 0, 1.0f);
    ball.setFixture(1.0f, 0.3f);

    float x = ball.posX();
    float y = ball.posY();

    for (int i = 0; i < 60; i++)  world->step();

    CPPUNIT_ASSERT(x == ball.posX());
    CPPUNIT_ASSERT(y != ball.posY());
}

void BliPhysicsTest::simulation2()
{
    BliWorld *world = new BliWorld(0.0f, -10.0f);

    BliStaticBody ground(world, 0, -10.0f);
    ground.setBox(50.0f, 10.0f);
    ground.setFixture(0.0, 0.0);
    ground.setAngle(-0.42f);

    BliDynamicBody ball(world, 0.0f, 4.0f);
    ball.setCircle(0, 0, 1.0f);
    ball.setFixture(1.0f, 0.3f);

    float x = ball.posX();
    float y = ball.posY();

    for (int i = 0; i < 200; i++) world->step();

    CPPUNIT_ASSERT(ball.posX() > x);
    CPPUNIT_ASSERT(ball.posY() < y);
    CPPUNIT_ASSERT(ball.posY() < 0);
}

void BliPhysicsTest::ortho()
{
    BliWorld world(0.0f, -10.0f);
    world.setOrtho(-10, 10, -10, 10);
    world.setViewport(640, 480);

    CPPUNIT_ASSERT_EQUAL(-10.0f, world.screenToX(0));
    CPPUNIT_ASSERT_EQUAL(10.0f, world.screenToX(640));
    CPPUNIT_ASSERT_EQUAL(10.0f, world.screenToY(0));
    CPPUNIT_ASSERT_EQUAL(-10.0f, world.screenToY(480));

    CPPUNIT_ASSERT_EQUAL(0.0f, world.XToScreen(-10));
    CPPUNIT_ASSERT_EQUAL(640.0f, world.XToScreen(10));
    CPPUNIT_ASSERT_EQUAL(320.0f, world.XToScreen(0));
    CPPUNIT_ASSERT_EQUAL(480.0f, world.YToScreen(-10));
    CPPUNIT_ASSERT_EQUAL(240.0f, world.YToScreen(0));
    CPPUNIT_ASSERT_EQUAL(0.0f, world.YToScreen(10));

    world.setOrtho(0, 10, 0, 10);
    world.setViewport(640, 480);


    CPPUNIT_ASSERT_EQUAL(0.0f, world.screenToX(0));
    CPPUNIT_ASSERT_EQUAL(10.0f, world.screenToX(640));
    CPPUNIT_ASSERT_EQUAL(10.0f, world.screenToY(0));
    CPPUNIT_ASSERT_EQUAL(0.0f, world.screenToY(480));

    CPPUNIT_ASSERT_EQUAL(-640.0f, world.XToScreen(-10));
    CPPUNIT_ASSERT_EQUAL(640.0f, world.XToScreen(10));
    CPPUNIT_ASSERT_EQUAL(320.0f, world.XToScreen(5));
    CPPUNIT_ASSERT_EQUAL(0.0f, world.XToScreen(0));

    CPPUNIT_ASSERT_EQUAL(960.0f, world.YToScreen(-10));
    CPPUNIT_ASSERT_EQUAL(480.0f, world.YToScreen(0));
    CPPUNIT_ASSERT_EQUAL(240.0f, world.YToScreen(5));
    CPPUNIT_ASSERT_EQUAL(0.0f, world.YToScreen(10));
}
