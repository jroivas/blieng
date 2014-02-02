#include "maps_test.h"
#include "test_tools.h"
#include <maps.h>

CPPUNIT_TEST_SUITE_REGISTRATION( MapsTest );

void MapsTest::setUp()
{
    mock_add_folder("data");
    mock_set_file("data/map42.json",
        "{ \"image\": \"random_img.png\"," \
        "\"towns\": [" \
        " { \"name\": \"town1\", \"size\": 10, \"posx\": 100, \"posy\":100, \"population-index\": 1 }," \
        " { \"name\": \"another\", \"size\": 99, \"posx\": 200, \"posy\":150, \"population-index\": 2 }" \
        "]," \
        "\"paths\": [" \
        " [[100,100], [100,150], [175,150], [200,150]]," \
        " [[100,100], [90,110], [80,120], [70,130], [200,150]]" \
        "]" \
        "}"
        );
    mock_io_start();
}

void MapsTest::tearDown()
{
    mock_io_stop();
}

void MapsTest::basic()
{
    shared_ptr<blieng::Data> data(new blieng::Data());
    shared_ptr<blieng::Maps> maps(new blieng::Maps(data, "map42"));

    CPPUNIT_ASSERT_EQUAL( std::string("map42"), maps->getMapName() );

    CPPUNIT_ASSERT_EQUAL( std::string("random_img.png"), maps->getMapImageFile() );

    maps->setBackgroundImage("dummy.png");
    CPPUNIT_ASSERT_EQUAL( std::string("dummy.png"), maps->getMapImageFile() );

    std::vector<blieng::Town *> towns = maps->getTowns();
    CPPUNIT_ASSERT_EQUAL( (size_t)2, towns.size() );
    CPPUNIT_ASSERT_EQUAL( std::string("town1"), towns[0]->getName() );
    CPPUNIT_ASSERT_EQUAL( std::string("another"), towns[1]->getName() );

    CPPUNIT_ASSERT( maps->removeTown(towns[0]) );

    towns = maps->getTowns();
    CPPUNIT_ASSERT_EQUAL( (size_t)1, towns.size() );
    CPPUNIT_ASSERT_EQUAL( std::string("another"), towns[0]->getName() );

    blieng::Town *dummy = new blieng::Town();
    dummy->setName("dummy");
    maps->addTown(dummy);

    towns = maps->getTowns();
    CPPUNIT_ASSERT_EQUAL( (size_t)2, towns.size() );
    CPPUNIT_ASSERT_EQUAL( std::string("another"), towns[0]->getName() );
    CPPUNIT_ASSERT_EQUAL( std::string("dummy"), towns[1]->getName() );
}

void MapsTest::paths()
{
    shared_ptr<blieng::Data> data(new blieng::Data());
    shared_ptr<blieng::Maps> maps(new blieng::Maps(data, "map42"));

    blieng::Point p1(99,77);
    blieng::Point p2(555,787);

    blieng::Path path;
    path.addPoint(p1);
    path.addPoint(p2);

    {
        const std::vector<blieng::Path> paths = maps->getPaths();

        CPPUNIT_ASSERT_EQUAL( (size_t)2, paths.size() );

        CPPUNIT_ASSERT_EQUAL( (size_t)4, paths[0].getPoints().size() );

        CPPUNIT_ASSERT_EQUAL( (double)100, paths[0].getPoints()[0].x );
        CPPUNIT_ASSERT_EQUAL( (double)100, paths[0].getPoints()[1].x );
        CPPUNIT_ASSERT_EQUAL( (double)175, paths[0].getPoints()[2].x );
        CPPUNIT_ASSERT_EQUAL( (double)200, paths[0].getPoints()[3].x );

        CPPUNIT_ASSERT_EQUAL( (double)100, paths[0].getPoints()[0].y );
        CPPUNIT_ASSERT_EQUAL( (double)150, paths[0].getPoints()[1].y );
        CPPUNIT_ASSERT_EQUAL( (double)150, paths[0].getPoints()[2].y );
        CPPUNIT_ASSERT_EQUAL( (double)150, paths[0].getPoints()[3].y );

        CPPUNIT_ASSERT_EQUAL( (size_t)5, paths[1].getPoints().size() );

        CPPUNIT_ASSERT_EQUAL( (double)100, paths[1].getPoints()[0].x );
        CPPUNIT_ASSERT_EQUAL( (double)90, paths[1].getPoints()[1].x );
        CPPUNIT_ASSERT_EQUAL( (double)80, paths[1].getPoints()[2].x );
        CPPUNIT_ASSERT_EQUAL( (double)70, paths[1].getPoints()[3].x );
        CPPUNIT_ASSERT_EQUAL( (double)200, paths[1].getPoints()[4].x );

        CPPUNIT_ASSERT_EQUAL( (double)100, paths[1].getPoints()[0].y );
        CPPUNIT_ASSERT_EQUAL( (double)110, paths[1].getPoints()[1].y );
        CPPUNIT_ASSERT_EQUAL( (double)120, paths[1].getPoints()[2].y );
        CPPUNIT_ASSERT_EQUAL( (double)130, paths[1].getPoints()[3].y );
        CPPUNIT_ASSERT_EQUAL( (double)150, paths[1].getPoints()[4].y );
    }

    maps->addPath(path);

    {
        const std::vector<blieng::Path> paths = maps->getPaths();
        CPPUNIT_ASSERT_EQUAL( (size_t)3, paths.size() );

        CPPUNIT_ASSERT_EQUAL( (size_t)4, paths[0].getPoints().size() );
        CPPUNIT_ASSERT_EQUAL( (size_t)5, paths[1].getPoints().size() );
        CPPUNIT_ASSERT_EQUAL( (size_t)2, paths[2].getPoints().size() );

        CPPUNIT_ASSERT_EQUAL( (double)99, paths[2].getPoints()[0].x );
        CPPUNIT_ASSERT_EQUAL( (double)555, paths[2].getPoints()[1].x );

        CPPUNIT_ASSERT_EQUAL( (double)77, paths[2].getPoints()[0].y );
        CPPUNIT_ASSERT_EQUAL( (double)787, paths[2].getPoints()[1].y );

        const std::vector<blieng::Path> rev_paths = maps->getRevPaths();

        CPPUNIT_ASSERT_EQUAL( (size_t)2, rev_paths[2].getPoints().size() );
        CPPUNIT_ASSERT_EQUAL( (double)555, rev_paths[2].getPoints()[0].x );
        CPPUNIT_ASSERT_EQUAL( (double)99, rev_paths[2].getPoints()[1].x );

        CPPUNIT_ASSERT_EQUAL( (double)787, rev_paths[2].getPoints()[0].y );
        CPPUNIT_ASSERT_EQUAL( (double)77, rev_paths[2].getPoints()[1].y );
    }
}
