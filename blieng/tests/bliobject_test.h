#ifndef __TEST_BLIOBJECT_H
#define __TEST_BLIOBJECT_H

#include <cpptest.h>

class BliObjectTest : public Test::Suite
{
public:
	BliObjectTest() {
		TEST_ADD(BliObjectTest::values);
		TEST_ADD(BliObjectTest::getValues);
	}
private:
	void values();
	void getValues();
};

#endif
