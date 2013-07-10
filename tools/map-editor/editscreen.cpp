#include "editscreen.h"
#include "blieng/town.h"
#include "blieng/data.h"
#include "blieng/configure.h"
#include <boost/foreach.hpp>

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
    drawing_path = false;

    new_town = new QPushButton("Add town");
    new_path = new QPushButton("Draw path");
    load_bg = new QPushButton("Select BG image");
    load_map = new QPushButton("Load map");
    save_map = new QPushButton("Save map");
    quit = new QPushButton("Quit editor");
    town_prop = new TownProperties();

    new_path->setCheckable(true);
    map->setMinimumWidth(width()/10*9);

    control_layout.addWidget(new_town);
    control_layout.addWidget(new_path);
    control_layout.addWidget(load_bg);
    control_layout.addWidget(load_map);
    control_layout.addWidget(save_map);
    control_layout.addWidget(quit);
    control_layout.addWidget(town_prop);
    layout.addLayout(&control_layout);

    layout.addWidget(map);

    town_prop->setVisible(false);
    new_path->setCheckable(true);

    connect(new_town, SIGNAL(clicked()), this, SLOT(addTown()));
    connect(load_bg, SIGNAL(clicked()), this, SLOT(loadBG()));
    connect(load_map, SIGNAL(clicked()), this, SLOT(loadMap()));
    connect(save_map, SIGNAL(clicked()), this, SLOT(saveMap()));
    connect(new_path, SIGNAL(toggled(bool)), this, SLOT(drawPath(bool)));
    connect(quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

    connect(map, SIGNAL(townSelected(blieng::Town*)), this, SLOT(townSelected(blieng::Town*)));
    connect(town_prop, SIGNAL(updated()), this, SLOT(doUpdate()));

    connect(map, SIGNAL(mousePressed(QMouseEvent*, double)), this, SLOT(mouseDown(QMouseEvent*, double)));
    connect(map, SIGNAL(mouseReleased(QMouseEvent*, double)), this, SLOT(mouseRelease(QMouseEvent*, double)));
    connect(map, SIGNAL(mouseMoved(QMouseEvent*, double)), this, SLOT(mouseMove(QMouseEvent*, double)));
}

void EditScreen::resizeEvent(QResizeEvent *event)
{
    map->setMinimumWidth(width()/10*9);
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

    if (map->getEditPath().isValid()) {
        moving_path = true;
        if ((e->buttons() & Qt::RightButton) > 0) {
            blieng::Point pt = map->getEditPoint();
            if (pt == edit_point) {
                QPointF pos = e->pos() * (zoomlevel / 100);
                pos -= map->getImagePos();
                blieng::Path new_path = map->getMaps()->updatePath(map->getEditPath(), blieng::Point(pos.x(), pos.y() + 30));
                map->updateEditPath(new_path);
                map->update();
            }
        } else {
            edit_point = map->getEditPoint();
        }
    }
}

void EditScreen::mouseRelease(QMouseEvent *e, double zoomlevel)
{
    moving = false;
    moving_path = false;

    blieng::Town *town = town_prop->getTown();
    if (drawing_path) {
        QPointF pos = e->pos() * (zoomlevel / 100);
        pos -= map->getImagePos();
        if (draw_path.size() >= 2) {
            draw_path = map->getMaps()->updatePath(draw_path, blieng::Point(pos.x(), pos.y()));
            map->update();
        } else {
            draw_path.addPoint(blieng::Point(pos.x(), pos.y()));
            if (draw_path.size() == 2) {
                map->getMaps()->addPath(draw_path);
                map->update();
            }
        }
    }
}

void EditScreen::drawPath(bool drawpath)
{
    drawing_path = drawpath;
    draw_path = blieng::Path();
}

void EditScreen::mouseMove(QMouseEvent *e, double zoomlevel)
{
    blieng::Town *town = town_prop->getTown();

    QPointF pos = e->pos() * (zoomlevel / 100);
    pos -= map->getImagePos();
    if (town != NULL && moving) {
        town->setPositionX(pos.x());
        town->setPositionY(pos.y());
        town_prop->updatePos();
        map->update();
    }
    if (moving_path && map->getEditPath().isValid()) {
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

void EditScreen::saveMap()
{
    bool ok;
    QString mapname = QInputDialog::getText(this, tr("Save map"), tr("Give map name:"), QLineEdit::Normal, map->getMaps()->getMapName().c_str(), &ok);
    if (ok && !mapname.isEmpty()) {
        qDebug() << "Saving" << mapname;
    } else {
        qDebug() << "failed";
    }
}

void EditScreen::loadMap()
{
    QStringList items;
    std::vector<std::string> maps = blieng::Data::getInstance()->listMaps();
    BOOST_FOREACH(std::string map, maps) {
        items << QString(map.c_str()).replace(".json", "");
    }

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Load map)"), tr("Map name:"), items, 0, false, &ok);
    if (ok && !item.isEmpty()) {
        map->loadMap(item.toLatin1().constData());
    }
}

void EditScreen::loadBG()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load background image"), "", tr("Images (*.png *.jpg *.jpeg)"));
    map->getMaps()->setBackgroundImage(filename.toLatin1().constData());
    map->loadImage();
    map->update();
}
