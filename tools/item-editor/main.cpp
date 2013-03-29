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

	Items *i = Items::getInstance();
	//Datas *d = new Datas(argv[1]);
	
	//QGraphicsScene scene;
	MyScene scene;
	//scene.addText("Aaa");

	QGraphicsView view(&scene);
	view.setMinimumSize(600, 400);
	view.setBackgroundBrush(QBrush(Qt::red, Qt::SolidPattern));

	i->addToScene(&scene);
	//scene.addItem(new EditorTable());

	view.show();

	return app.exec();
}
