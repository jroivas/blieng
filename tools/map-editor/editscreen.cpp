#include "editscreen.h"
#include "blieng/town.h"

using MapEditor::EditScreen;

static const char * NEW_TOWN_NAME = "[New town]";
static const unsigned int NEW_TOWN_SIZE = 10;
static const unsigned int NEW_TOWN_POSX = 200;
static const unsigned int NEW_TOWN_POSY = 200;

EditScreen::EditScreen(QWidget *parent) : QWidget(parent)
{
    setLayout(&layout);
    map = new ui::MapScreen();
    new_town = new QPushButton("Add town");
    new_path = new QPushButton("Draw path");
    town_prop = new TownProperties();

    new_town->setMaximumWidth(150);
    new_path->setMaximumWidth(150);
    new_path->setCheckable(true);
    town_prop->setMaximumWidth(150);
    map->setMinimumWidth(width()/10*9);

    control_layout.addWidget(new_town);
    control_layout.addWidget(new_path);
    control_layout.addWidget(town_prop);
    layout.addLayout(&control_layout);

    layout.addWidget(map);

    town_prop->setVisible(false);

    connect(new_town, SIGNAL(clicked()), this, SLOT(addTown()));
    connect(map, SIGNAL(townSelected(blieng::Town*)), this, SLOT(townSelected(blieng::Town*)));
    connect(town_prop, SIGNAL(updated()), this, SLOT(doUpdate()));
}

void EditScreen::loadMap(QString mapname)
{
    map->loadMap(mapname);
}

void EditScreen::doUpdate()
{
    map->update();
}

void EditScreen::townSelected(blieng::Town *town)
{
    town_prop->setTown(town);
    town_prop->setVisible(true);
}

void EditScreen::addTown()
{
    blieng::Town *town = new blieng::Town();
    town->setName(NEW_TOWN_NAME);
    town->setSize(NEW_TOWN_SIZE);
    town->setPositionX(NEW_TOWN_POSX);
    town->setPositionY(NEW_TOWN_POSY);
    map->getMaps()->addTown(town);
    map->update();
}
