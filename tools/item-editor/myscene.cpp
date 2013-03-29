#include "myscene.h"
#include <QDebug>
#include <boost/foreach.hpp>
#include <QGraphicsLineItem>

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent)
{
	moving = false;
	move_item = NULL;
	edit_item = NULL;
	editor = new EditorTable();
	addItem(editor);
	editor->setVisible(false);
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (move_item != NULL) {
		move_item->setPos(event->scenePos());
		move_item->setBg(QColor(0,0,255, 255));
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
			if (event->buttons() & Qt::LeftButton) {
				move_item = tmp;
				event->accept();
			}
		}
	}
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (move_item != NULL) {
		move_item->setBg(QColor(0,0,0,0));
		move_item = NULL;
		update();
	}
	else if (event->button() & Qt::RightButton) {
		QGraphicsItem *item = itemAt(event->scenePos());
		qDebug() << item;
		if (item != NULL) {
			ViewItem *tmp = dynamic_cast<ViewItem *>(item);
			edit_item = tmp;
			qDebug() << "Edit " << edit_item;
			editor->setVisible(true);
			editor->setZValue(100);
			event->accept();
		}
	}
}

