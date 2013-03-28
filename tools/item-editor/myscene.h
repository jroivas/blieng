#ifndef __MYSCENE_H
#define __MYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "items.h"

class MyScene : public QGraphicsScene
{
	Q_OBJECT
public:
	MyScene(QObject *parent=0);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *);
	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

private:
	bool moving;
	ViewItem *move_item;
};

#endif
