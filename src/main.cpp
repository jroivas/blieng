#include <QtCore>
#include <QtGui>

#include "player_character.h"
#include <iostream>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);


	PlayerCharacter c;
	std::cout << "name   " << c.getStringValue("name") << "\n";
	std::cout << "health " << c.getIntValue("health") << "\n";
	std::cout << "zombie " << c.getIntValue("zombie-prob") << "\n";
	
	return 0;
}
