#include "editscreen.h"

using MapEditor::EditScreen;

EditScreen::EditScreen(QWidget *parent) : QWidget(parent)
{
    setLayout(&layout);
    map = new ui::MapScreen();
    layout.addWidget(map);
}

void EditScreen::loadMap(QString mapname)
{
    map->loadMap(mapname);
}
