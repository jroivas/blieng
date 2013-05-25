#ifndef __TEST_PATH_H
#define __TEST_PATH_H

#include <cpptest.h>

class PathTest : public Test::Suite
{
public:
	PathTest() {
		TEST_ADD(PathTest::basic);
		TEST_ADD(PathTest::append);
		TEST_ADD(PathTest::reverse);
		TEST_ADD(PathTest::copy);
		TEST_ADD(PathTest::take);
		TEST_ADD(PathTest::length);
	}
private:
	void basic();
	void append();
	void reverse();
	void copy();
	void take();
	void length();
};

#endif
