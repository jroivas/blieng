#ifndef __MAPEDITOR_EDITSCREEN_H
#define __MAPEDITOR_EDITSCREEN_H

#include <QWidget>
#include <QVBoxLayout>
#include "ui/mapscreen.h"
#include "blieng/maps.h"
#include "blieng/data.h"

namespace MapEditor
{

class EditScreen : public QWidget
{
    Q_OBJECT
public:
    EditScreen(QWidget *parent=0);
    void loadMap(QString mapname);

private:
    ui::MapScreen *map;
    QVBoxLayout layout;
};

}

#endif
