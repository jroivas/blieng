#ifndef __TEST_CHARACTER_H
#define __TEST_CHARACTER_H

#include <cpptest.h>

class CharacterTest : public Test::Suite
{
public:
    CharacterTest() {
        TEST_ADD(CharacterTest::alive_kill);
        TEST_ADD(CharacterTest::items);
        TEST_ADD(CharacterTest::assign);
        TEST_ADD(CharacterTest::assign_object);
    }
private:
    void alive_kill();
    void items();
    void assign();
    void assign_object();
};

#endif
