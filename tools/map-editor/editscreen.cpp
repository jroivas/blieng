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
    map->setEditMode(true);

    moving = false;

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

    connect(map, SIGNAL(mousePressed(QMouseEvent*, double)), this, SLOT(mouseDown(QMouseEvent*, double)));
    connect(map, SIGNAL(mouseReleased(QMouseEvent*, double)), this, SLOT(mouseRelease(QMouseEvent*, double)));
    connect(map, SIGNAL(mouseMoved(QMouseEvent*, double)), this, SLOT(mouseMove(QMouseEvent*, double)));
}

void EditScreen::loadMap(QString mapname)
{
    map->loadMap(mapname);
}

void EditScreen::doUpdate()
{
    map->update();
}

void EditScreen::mouseDown(QMouseEvent *e, double zoomlevel)
{
    blieng::Town *town = town_prop->getTown();
    //if (town == NULL) return;

    if (map->getEditPath().isValid()) {
        moving_path = true;
        edit_point = map->getEditPoint();
    }
}

void EditScreen::mouseRelease(QMouseEvent *e, double zoomlevel)
{
    moving = false;
    moving_path = false;

    blieng::Town *town = town_prop->getTown();
    if (town == NULL) return;
}

void EditScreen::mouseMove(QMouseEvent *e, double zoomlevel)
{
    blieng::Town *town = town_prop->getTown();

    QPointF pos = e->pos() * ( zoomlevel / 100);
    pos -= map->getImagePos();
    if (town != NULL && moving) {
        town->setPositionX(pos.x());
        town->setPositionY(pos.y());
        town_prop->updatePos();
        map->update();
    }
    if (moving_path) {
        blieng::Point new_point = blieng::Point(pos.x(), pos.y());
        blieng::Path new_path = map->getMaps()->updatePath(map->getEditPath(), map->getEditPointIndex(), new_point);
        edit_point = new_point;
        map->updateEditPoint(new_point);
        map->updateEditPath(new_path);
        map->update();
    }
}

void EditScreen::townSelected(blieng::Town *town)
{
    town_prop->setTown(town);
    town_prop->setVisible(true);
    moving = true;
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
