#ifndef __MAPEDITOR_EDITSCREEN_H
#define __MAPEDITOR_EDITSCREEN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "ui/mapscreen.h"
#include "blieng/maps.h"
#include "blieng/data.h"
#include "townproperties.h"

namespace MapEditor
{

class EditScreen : public QWidget
{
    Q_OBJECT
public:
    EditScreen(QWidget *parent=0);
    void loadMap(QString mapname);

protected slots:
    void addTown();
    void townSelected(blieng::Town *);
    void doUpdate();

    void mouseDown(QMouseEvent *, double);
    void mouseRelease(QMouseEvent *, double);
    void mouseMove(QMouseEvent *, double);

private:
    ui::MapScreen *map;
    QHBoxLayout layout;
    QVBoxLayout control_layout;
    QPushButton *new_town;
    QPushButton *new_path;

    TownProperties *town_prop;
    blieng::Point edit_point;
    bool moving;
    bool moving_path;
};

}

#endif
