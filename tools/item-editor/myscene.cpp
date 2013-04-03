#include "myscene.h"
#include "simplebutton.h"
#include "items.h"
#include "item.h"
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

	but_layout.setOrientation(Qt::Vertical);

	add_item_button = new SimpleButton("Add item");
	connect(add_item_button, SIGNAL(released()), this, SLOT(newItem()));
	//addItem(add_item_button);
	add_item_button->setGeometry(QRectF(-20, 0, 100, 100));
	add_item_button->setZValue(100);
	//add_item_button->setVisible(false);

	but_layout.addItem(add_item_button);

	actions = new QGraphicsWidget();
	actions->setLayout(&but_layout);
	actions->setVisible(false);
	addItem(actions);

	connect(editor, SIGNAL(updated()), this, SLOT(updated()));
}

void MyScene::newItem()
{
	Items *items = Items::getInstance();
	if (items!=NULL) {
		blieng::Item *item = new blieng::Item();
		item->base = "new item";
		ViewItem *view_item = new ViewItem(item);
		items->appendItem(this, view_item);
		update();
	}
}


void MyScene::updated()
{
	Items *items = Items::getInstance();
	if (items!=NULL) {
		items->updateLines(this);
	}
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	bool ok = false;
	if (move_item != NULL) {
		move_item->setPos(event->scenePos());
		move_item->setBg(QColor(0,0,255, 255));
		Items::getInstance()->update();
		update();
	}

	if (!ok) {
		QGraphicsScene::mouseMoveEvent(event);
	}
}


void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem *item = itemAt(event->scenePos());
	bool ok = false;
	if (item != NULL) {
		ViewItem *tmp = dynamic_cast<ViewItem *>(item);
		if (tmp != NULL) {
			if (event->buttons() & Qt::LeftButton) {
				move_item = tmp;
				event->accept();
				ok = true;
			}
		}
	}

	if (!ok) {
		QGraphicsScene::mousePressEvent(event);
	}
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	bool ok = false;
	if (move_item != NULL) {
		move_item->setBg(QColor(0,0,0,0));
		move_item = NULL;
		update();
		ok = true;
	}
	else if (event->button() & Qt::RightButton) {
		QGraphicsItem *item = itemAt(event->scenePos());
		qDebug() << item;
		if (item != NULL) {
			actions->setVisible(false);
			ViewItem *tmp = dynamic_cast<ViewItem *>(item);
			edit_item = tmp;
			qDebug() << "Edit " << edit_item;
			editor->loadItem(edit_item);
			editor->setVisible(true);
			editor->setZValue(100);
			event->accept();
			ok = true;
		} else {
			actions->setVisible(true);
			actions->setPos(event->scenePos());
			event->accept();
			ok = true;
		}
	}

	if (!ok) {
		QGraphicsScene::mouseReleaseEvent(event);
	}
}

