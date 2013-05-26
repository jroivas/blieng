#ifndef __UI_LOADINGSCREEN_H
#define __UI_LOADINGSCREEN_H

#include <QtGui>
#include <QWidget>

namespace ui
{

class LoadingScreen : public QWidget
{
    Q_OBJECT
public:
    LoadingScreen(QWidget *parent=0);
    void paintEvent(QPaintEvent *event);

private:
    void drawCircle(QPainter *painter, double _angle, double _radius);
    QImage bg_img;
    double angle;
    double radius;
    QTimer *anim;
};

}

#endif
