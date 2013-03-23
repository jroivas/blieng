#include <QtCore>
#include <QtGui>

#include "blieng/player_character.h"
#include <iostream>

void printCharacter(blieng::PlayerCharacter *c)
{
	std::cout << "name   " << c->getStringValue("name") << "\n";
	std::cout << "health " << c->getIntValue("health") << "\n";
	std::cout << "zombie " << c->getIntValue("zombie-prob") << "\n";
	std::cout << c->toString();
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);


	blieng::PlayerCharacter c;
	printCharacter(&c);

	// Re-roll the character
	c.roll();
	printCharacter(&c);
	
	return 0;
}
