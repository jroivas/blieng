#include <QtGui>
#include <QDeclarativeView>
#include <iostream>

#include "items.h"
#include "myscene.h"
#include "editortable.h"

int main(int argc, char**argv)
{
	/*
	if (argc <= 1) {
		std::cout << "Usage: " << argv[0] << " file.json\n";
		return 1;
	}
	*/
	QApplication app(argc, argv);

	Items *items = Items::getInstance();
	
	MyScene scene;

	QGraphicsView view(&scene);
	view.setMinimumSize(600, 400);
	view.setBackgroundBrush(QBrush(Qt::red, Qt::SolidPattern));

	items->addToScene(&scene);

	view.show();

	return app.exec();
}
