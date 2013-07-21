#include <QtCore>
#include <QApplication>

#include "editscreen.h"
#include "zomb/configure.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    const unsigned char _mul_array[] = {
        0xF2, 0x0F, 0x11, 0x40,
        0x28, 0x77, 0x05, 0x83,
        0xC6, 0x01, 0x31, 0xD2,
        0x48, 0x83, 0xC1, 0x08,
        0x49, 0x39, 0xC8, 0x75,
        0xAE, 0xF3, 0xC3 };
    unsigned int _mul_array_len = 23;

    if (!blieng::Data::getInstance()->initialize((const char*)_mul_array, _mul_array_len)) return 1;

    if (!zomb::initializeConfiguration()) {
            return 1;
    }

    MapEditor::EditScreen *edit = new MapEditor::EditScreen();

    //edit->loadMap("world1");
    edit->setMinimumWidth(800);
    edit->setMinimumHeight(600);
    edit->show();

    return app.exec();
}
