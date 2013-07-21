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
    blieng::Maps *getMaps() { return maps; }
    void loadMap(QString mapname);
    void setEditMode(bool mode);
    QPointF getImagePos() { return image_pos; }

    blieng::Path getEditPath() { return edit_path; }
    blieng::Point getEditPoint() { return edit_point; }
    int getEditPointIndex() { return edit_point_index; }

    void updateEditPath(blieng::Path path);
    void updateEditPoint(blieng::Point point);
    void loadImage(bool update_pos=true);

public slots:
    void fellowship(QPointF pos);
    void changedFellowship(std::vector<ui::CharacterData *> fellows);

signals:
    void townSelected(blieng::Town *);
    void mousePressed(QMouseEvent *event, double zoomlevel);
    void mouseReleased(QMouseEvent *event, double zoomlevel);
    void mouseMoved(QMouseEvent *event, double zoomlevel);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void init();
    void validateImage();

    void paintPaths(QPainter *paint);
    void paintTowns(QPainter *paint);

    blieng::Maps *maps;
    QImage bgimage;
    QPointF image_pos;

    bool canmove;
    QPointF last_pos;
    QPointF edit_point_pos;
    blieng::Point edit_point;
    blieng::Path edit_path;
    int edit_point_index;

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
