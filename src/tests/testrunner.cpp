/* Test blieng
 */

#include <cpptest.h>
#include "point_test.h"

int main(int argc, char **argv)
{
        (void)argc;
        (void)argv;

        Test::Suite tests;

        tests.add(std::auto_ptr<Test::Suite>(new PointTest));

        Test::TextOutput output(Test::TextOutput::Verbose);
        return tests.run(output) ? 0 : 1;
}
