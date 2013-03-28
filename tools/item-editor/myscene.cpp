#include "myscene.h"
#include <QDebug>
#include <boost/foreach.hpp>
#include <QGraphicsLineItem>

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent)
{
	moving = false;
	move_item = NULL;
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//qDebug() << "move";
	//qDebug() << "move" << event->pos() << event->buttonDownPos(Qt::LeftButton);
	//QGraphicsItem *item = itemAt(event->scenePos());
	//ViewItem *tmp = dynamic_cast<ViewItem *>(item);
	if (move_item != NULL) {
		//qDebug() << "move" << tmp;
		//tmp->setBg(QColor(0,255,0, 255));
		move_item->setPos(event->scenePos());
		Items::getInstance()->update();
		update();
	}
}


void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem *item = itemAt(event->scenePos());
	if (item != NULL) {
		ViewItem *tmp = dynamic_cast<ViewItem *>(item);
		if (tmp != NULL) {
			move_item = tmp;
			event->accept();
		}
	}
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	move_item = NULL;
	//qDebug() << "release";
}

