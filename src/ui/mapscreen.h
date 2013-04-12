#ifndef __GAMESCREEN_H
#define __GAMESCREEN_H

#include <QtGui>
#include "blieng/maps.h"
#include "zomb/createworld.h"

namespace ui
{

class MapScreen : public QWidget
{
	Q_OBJECT
public:
	MapScreen(QString mapname, QWidget *parent=0);
	void paintEvent(QPaintEvent *event);

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	void loadImage();
	void validateImage();

	blieng::Maps *maps;
	QImage bgimage;
	QPoint image_pos;

	bool canmove;
	QPoint last_pos;

	zomb::CreateWorld *create_world;
};

}

#endif
