#include <QtCore>
#include <QtGui>

#include <iostream>
#include "ui/generate_character.h"
#include "blieng/item.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	for (int j=0; j<5; j++) {
	blieng::Item *i = new blieng::Item();
	std::cout << i->toString() << "\n";
	}

	#if 0
	GenerateCharacter *cg = new GenerateCharacter();
	cg->show();
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	
	return app.exec();
	#else
	return 0;
	#endif
}
