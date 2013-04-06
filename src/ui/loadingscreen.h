#ifndef __LOADINGSCREEN_H
#define __LOADINGSCREEN_H

#include <QtGui>

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
