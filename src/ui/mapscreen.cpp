#include "mapscreen.h"
#include "blieng/data.h"
#include "blieng/configure.h"
#include <boost/foreach.hpp>

using ui::MapScreen;


/* FIXME: Should this be implemented with QGraphicsItem, QGraphicsScene, etc.?
 */

MapScreen::MapScreen(QWidget *parent) : QWidget(parent)
{
    maps = new blieng::Maps("");
    create_world = NULL;
    edit_mode = false;
    edit_point = blieng::Point();
    edit_path = blieng::Path();
    init();
}

MapScreen::MapScreen(QString mapname, QWidget *parent) : QWidget(parent)
{
    loadMap(mapname);
    init();
}

void MapScreen::init()
{
    zoomlevel = blieng::Configure::getInstance()->getUIntValue("default_zoom_level");
    fella_size = blieng::Configure::getInstance()->getUIntValue("chr_map_size");
    clicked_town = NULL;
}

void MapScreen::loadMap(QString mapname)
{
    maps = new blieng::Maps(mapname.toStdString());
    create_world = new zomb::CreateWorld(maps);
    loadImage();
}

void MapScreen::loadImage()
{
    bgimage = QImage(maps->getSolvedMapImageFile().c_str());
    image_pos = QPointF(0, 0);
}

/* Validate that map fits on the screen
*/
void MapScreen::validateImage()
{
    if (image_pos.x() > 0) image_pos.setX(0);
    if (image_pos.y() > 0) image_pos.setY(0);
    if (image_pos.x() < -(bgimage.width() - width())) image_pos.setX(-(bgimage.width() - width()));
    if (image_pos.y() < -(bgimage.height() - height())) image_pos.setY(-(bgimage.height() - height()));
}

void MapScreen::fellowship(QPointF pos)
{
    fellowship_pos = pos;
    update();
}

void MapScreen::changedFellowship(std::vector<ui::CharacterData *> fellows)
{
    the_fellowship = fellows;
    update();
}

void MapScreen::paintEvent(QPaintEvent *event)
{
    if (maps == NULL) return;
    QPainter paint(this);

    double zoomfactor = zoomlevel / 100.0;
    if (!bgimage.isNull()) {
        validateImage();
        paint.drawImage(image_pos / zoomfactor, bgimage);
    }

    QPen blackpen(QColor(0,0,0,255));
    double path_width = 1;
    try {
        path_width = blieng::Configure::getInstance()->getUIntValue("path_width");
    }
    catch (std::string res) {
        path_width = 3;
    }

    blackpen.setWidth(path_width);
    paint.setPen(blackpen);
    BOOST_FOREACH(blieng::Path path, maps->getPaths()) {
        blieng::Point prev(false);
        BOOST_FOREACH(blieng::Point point, path.getPoints()) {
            if (prev.isValid()) {
                paint.setPen(blackpen);
                QPointF a = QPointF(prev.x, prev.y) + image_pos;
                QPointF b = QPointF(point.x, point.y) + image_pos;
                a /= zoomfactor;
                b /= zoomfactor;
                paint.drawLine(a, b);
            }
            if (edit_mode) {
                paint.setPen(QColor(255,0,0,255));
                paint.setBrush(QColor(255,0,0,255));
                QPointF b = QPointF(point.x, point.y) + image_pos;
                b /= zoomfactor;
                paint.drawEllipse(b, 3/zoomfactor, 3/zoomfactor);
            }
            prev = point;
        }
    }

    paint.setBrush(QColor(0,0,0,255));
    BOOST_FOREACH(blieng::Town *town, maps->getTowns()) {
        paint.setPen(QColor(0,0,0,255));
        paint.setBrush(QColor(0,0,0,255));
        QPointF town_pos(town->getPositionX(), town->getPositionY());
        paint.drawEllipse((image_pos + town_pos) / zoomfactor, qreal((town->getSize()) / zoomfactor), qreal((town->getSize()) / zoomfactor));

        QRect namebox = paint.boundingRect(0, 0, blieng::Configure::getInstance()->getUIntValue("townname_max_width") / zoomfactor, 100 / zoomfactor, Qt::AlignLeft, town->getName().c_str());
        QPointF textpos = image_pos + town_pos;
        paint.setPen(QColor(255,0,0,255));
        textpos.setX(textpos.x() - namebox.width()/2);

        paint.drawText(textpos / zoomfactor, town->getName().c_str());
        if (town == clicked_town) {
            QPointF sizemin = QPointF(town->getSize() / zoomfactor * 3, town->getSize() / zoomfactor * 3);
            paint.setBrush(QColor(0,0,0,0));
            QRectF selection = QRectF(((image_pos + town_pos) / zoomfactor) - (sizemin / 2), ((image_pos + town_pos) / zoomfactor) + (sizemin / 2));
            paint.drawRect(selection);
        }
    }

    if (edit_mode) {
        if (!edit_point_pos.isNull()) {
            paint.setPen(QColor(255,0,0,255));
            paint.setBrush(QColor(0,0,0,255));
            paint.drawEllipse((image_pos + edit_point_pos) / zoomfactor, 8/zoomfactor, 8/zoomfactor);
        }
    }

    unsigned int fella = 0;
    BOOST_FOREACH(ui::CharacterData *fellow, the_fellowship) {
        QPointF fpos;
        switch(fella) {
            case 0:
                fpos = QPointF(-1*fella_size/2, -1*fella_size/2);
                break;
            case 1:
                fpos = QPointF(fella_size/2, -1*fella_size/2);
                break;
            case 2:
                fpos = QPointF(-1*fella_size/2, fella_size/2);
                break;
            case 3:
                fpos = QPointF(fella_size/2, fella_size/2);
                break;
            default:
                fpos = QPointF(0, 0);
                break;
        }
        fpos -= QPointF(fella_size/2, fella_size/2);
        paint.drawImage((image_pos + fellowship_pos + fpos) / zoomfactor, fellow->image.scaled(fella_size / zoomfactor, fella_size / zoomfactor));
        ++fella;
    }
}

void MapScreen::mousePressEvent(QMouseEvent *event)
{
    QPointF now_pos = event->pos() * (zoomlevel / 100.0);
    if (edit_mode) {
        emit mousePressed(event, zoomlevel);

        BOOST_FOREACH(blieng::Path path, maps->getPaths()) {
            int index = 0;
            BOOST_FOREACH(blieng::Point point, path.getPoints()) {
                QPointF point_pos(point.x, point.y);
                QPointF point_diff = point_pos  + image_pos - now_pos;
                if (point_diff.manhattanLength() <= 5) {
                    //qDebug() << "press" << point.x << point.y;
                    edit_point_pos = point_pos;
                    edit_point = point;
                    edit_point_index = index;
                    edit_path = path;
                    update();
                    emit mousePressed(event, zoomlevel);
                    return;
                }
                index++;
            }
        }
    }
    if (event->buttons() & Qt::LeftButton) {
        canmove = true;
        last_pos = event->pos() * (zoomlevel / 100.0);
    }
}

void MapScreen::wheelEvent(QWheelEvent *event)
{
    int numDegrees = -1 * event->delta() / 8;
    int numSteps = numDegrees / 15;

    zoomlevel += numSteps * 2;

    bgimage = QImage(maps->getSolvedMapImageFile().c_str());
    bgimage = bgimage.scaled(bgimage.size() / (zoomlevel / 100.0));
    update();
}

void MapScreen::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF now_pos = event->pos() * (zoomlevel / 100.0);
    if (edit_mode) {
        emit mouseReleased(event, zoomlevel);

        edit_path = blieng::Path();
        edit_point = blieng::Point();
        /*
        BOOST_FOREACH(blieng::Path path, maps->getPaths()) {
            BOOST_FOREACH(blieng::Point point, path.getPoints()) {
                QPointF point_pos(point.x, point.y);
                point_pos += image_pos;
                QPointF point_diff = point_pos - now_pos;
                if (point_diff.manhattanLength() <= 5) {
                    qDebug() << point.x << point.y;
                    edit_point_pos = point_pos;
                    edit_path = path;
                    update();
                }
            }
        }
        */
    }
    if (canmove) {
        QPointF pos_diff = now_pos - last_pos;
        if (pos_diff.manhattanLength() > blieng::Configure::getInstance()->getUIntValue("town_sensitivity_distance")) {
            image_pos += pos_diff;
            update();
        } else {
            BOOST_FOREACH(blieng::Town *town, maps->getTowns()) {
                QPointF town_pos(town->getPositionX(), town->getPositionY());
                town_pos += image_pos;
                QPointF town_diff = town_pos - now_pos;
                if (town_diff.manhattanLength() <= town->getSize()) {
                    if (blieng::Configure::getInstance()->getBoolValue("debug")) qDebug() << "Clicked town " << town->getName().c_str();
                    clicked_town = town;
                    emit townSelected(town);
                }
            }
        }
    }
    canmove = false;
}

void MapScreen::mouseMoveEvent(QMouseEvent *event)
{
    if (edit_mode) {
        emit mouseMoved(event, zoomlevel);
    }
}

void MapScreen::setEditMode(bool mode)
{
    edit_mode = mode;
}

void MapScreen::updateEditPoint(blieng::Point point)
{
    edit_point = point;
    edit_point_pos = QPointF(point.x, point.y);
}

void MapScreen::updateEditPath(blieng::Path path)
{
    edit_path = path;
}
