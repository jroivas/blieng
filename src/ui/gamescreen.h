#ifndef __GAMESCREEN_H
#define __GAMESCREEN_H

#include <QtGui>
#include "blieng/maps.h"

namespace ui
{

class GameScreen : public QWidget
{
	Q_OBJECT
public:
	GameScreen(QWidget *parent=0);
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
};

}

#endif
