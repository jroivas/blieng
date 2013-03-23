#include <QtCore>
#include <QtGui>

#include "player_character.h"
#include <iostream>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);


	PlayerCharacter c;
	std::cout << c.getStringValue("name") << "\n";
	#if 0
	Character c;
	c.setValue("test", 10);
	c.setValue("test2", std::string("hep"));

	std::cout << "test1: " << boost::any_cast<int>(c.getValue("test")) << "\n";
	std::cout << "test1: " << c.getIntegerValue("test") << "\n";
	std::cout << "test2: " << boost::any_cast<std::string>(c.getValue("test2")) << "\n";
	std::cout << "test2: " << c.getStringValue("test2") << "\n";
	boost::any a = c.getValue("test2");
	boost::any b = c.getValue("test3");
	if (a.empty()) std::cout << "a: Empty\n";
	if (b.empty()) std::cout << "b: Empty\n";
	std::cout << "test2: " << c.getStringValue("test3") << "\n";
	//std::cout << "test2 :" << c.getValue("test2") << "\n";
	#endif
	
	return 0;
}
