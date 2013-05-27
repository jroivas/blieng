#include <QtCore>
#include <QApplication>

#include "editscreen.h"
#include "zomb/configure.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    if (!zomb::initializeConfiguration()) {
            return 1;
    }

    MapEditor::EditScreen *edit = new MapEditor::EditScreen();

    edit->loadMap("world1");
    edit->show();

    return app.exec();
}
