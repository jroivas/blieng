#include <QtCore>
#include <QtGui>

#include <iostream>
#include "ui/generate_character.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	GenerateCharacter *cg = new GenerateCharacter();
	cg->show();
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	
	return app.exec();
}
