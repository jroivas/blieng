/* Test blieng
 */

#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "point_test.h"
#include "path_test.h"
#include "bliobject_test.h"
#include "character_test.h"
#include "data_test.h"
#include "auto_vector_test.h"
#include "auto_map_test.h"
#include "bliany_test.h"
#include "card_test.h"
#include "configure_test.h"
#include "town_test.h"
#include "maps_test.h"
#include "logging_test.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

    CppUnit::TextUi::TestRunner runner;
    runner.addTest( suite );

    runner.setOutputter( new CppUnit::XmlOutputter( &runner.result(), std::cerr ) );
    bool wasSucessful = runner.run();

    return wasSucessful ? 0 : 1;
}
