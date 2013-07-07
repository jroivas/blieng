#ifndef __UI_MAPSCREEN_H
#define __UI_MAPSCREEN_H

#include <QtGui>
#include "blieng/maps.h"
#include "blieng/town.h"
#include "zomb/createworld.h"
#include "ui/characterview.h"

namespace ui
{

class MapScreen : public QWidget
{
    Q_OBJECT
public:
    MapScreen(QWidget *parent=0);
    MapScreen(QString mapname, QWidget *parent=0);
    void paintEvent(QPaintEvent *event);
    blieng::Maps *getMaps() { return maps;}
    void loadMap(QString mapname);
    void setEditMode(bool mode);
    QPointF getImagePos() { return image_pos; }

public slots:
    void fellowship(QPointF pos);
    void changedFellowship(std::vector<ui::CharacterData *> fellows);

signals:
    void townSelected(blieng::Town *);
    void mousePressed(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);
    void mouseMoved(QMouseEvent *event);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void loadImage();
    void validateImage();

    blieng::Maps *maps;
    QImage bgimage;
    QPointF image_pos;

    bool canmove;
    QPointF last_pos;

    zomb::CreateWorld *create_world;
    std::vector<ui::CharacterData *> the_fellowship;
    QPointF fellowship_pos;
    blieng::Town *clicked_town;

    unsigned int zoomlevel;
    double fella_size;

    bool edit_mode;
};

}

#endif
