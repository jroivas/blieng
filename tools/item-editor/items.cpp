#include "items.h"
#include "data.h"
#include <math.h>
#include <QDebug>

Items::Items()
{
	base = new blieng::Item();
	item_names = base->listItems();

	generateViewItems();
}

void Items::generateViewItems()
{
	BOOST_FOREACH(std::string s, item_names) {
		blieng::Item *base_item = new blieng::Item(s);
		ViewItem *view_item = new ViewItem(base_item);
		view_items.push_back(view_item);
	}
}

void Items::addToScene(QGraphicsScene *scene)
{
	BOOST_FOREACH(ViewItem *s, view_items) {
		scene->addItem(s);
		s->show();
	}
	alignItems();
}

void Items::rowAlign()
{
	size_t itemcnt = view_items.size();
	size_t xpos = 0;
	size_t ypos = 0;
	size_t colcount = ceil(sqrt(itemcnt)) + 1;
	size_t col = 0;
	BOOST_FOREACH(ViewItem *s, view_items) {
		xpos += s->size().width()*2;
		col++;
		if (col >= colcount) {
			col = 0;
			xpos = 0;
			ypos += s->size().height()*2;
		}
		s->setPos(xpos, ypos);
	}
}

void Items::alignItems()
{
	rowAlign();
}

ViewItem::ViewItem(blieng::Item *item, QGraphicsItem *parent) : QGraphicsWidget(parent), item(item)
{
	setMinimumSize(60, 60);
}

void ViewItem::loadImage()
{
	if (img.isNull() && item->image != "") {
		std::string found = blieng::Data::getInstance()->findFile(item->image);
		if (found != "") {
			img = QImage(found.c_str()).scaled(32, 32, Qt::KeepAspectRatio);
		}
	}
}

QRectF ViewItem::boundingRect() const
{
	return QRectF(-40, -40, 40, 40);
}

void ViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	loadImage();
	if (!img.isNull()) {
		painter->drawImage(-1*img.width()/2, -1*img.height()/2, img);
	} else {
		painter->drawText(0, 0, QString(item->base.get().c_str()));
	}
}
