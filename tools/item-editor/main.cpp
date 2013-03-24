#include <QtGui>
#include <QDeclarativeView>
#include <iostream>

#include "datas.h"

int main(int argc, char**argv)
{
	if (argc <= 1) {
		std::cout << "Usage: " << argv[0] << " file.json\n";
		return 1;
	}
	QApplication app(argc, argv);

	Datas *d = new Datas(argv[1]);
	
	QGraphicsScene scene;
	scene.addText("Aaa");

	QGraphicsView view(&scene);
	view.show();

	return app.exec();
}
