#include <QtGui>
#include <QDeclarativeView>
#include <iostream>

#include "items.h"

int main(int argc, char**argv)
{
	/*
	if (argc <= 1) {
		std::cout << "Usage: " << argv[0] << " file.json\n";
		return 1;
	}
	*/
	QApplication app(argc, argv);

	Items *i = new Items();
	//Datas *d = new Datas(argv[1]);
	
	QGraphicsScene scene;
	//scene.addText("Aaa");

	QGraphicsView view(&scene);
	view.setMinimumSize(600, 400);
	view.setBackgroundBrush(QBrush(Qt::red, Qt::SolidPattern));

	i->addToScene(&scene);

	view.show();

	return app.exec();
}
