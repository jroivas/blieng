/* Test blieng
 */

#include <cpptest.h>
#include "point_test.h"
#include "path_test.h"
#include "bliobject_test.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    Test::Suite tests;

    tests.add(std::auto_ptr<Test::Suite>(new PointTest));
    tests.add(std::auto_ptr<Test::Suite>(new PathTest));
    tests.add(std::auto_ptr<Test::Suite>(new BliObjectTest));

    Test::TextOutput output(Test::TextOutput::Verbose);
    return tests.run(output) ? 0 : 1;
}
