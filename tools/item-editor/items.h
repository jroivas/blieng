#ifndef __ITEMS_H
#define __ITEMS_H

#include <vector>
#include <string>
#include <boost/foreach.hpp>
#include "item.h"
#include <QGraphicsItem>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>

class ViewItem : public QGraphicsItem
{
public:
	ViewItem(blieng::Item *item, QGraphicsItem *parent=0);
	~ViewItem() {
		if (item != NULL) {
			delete item;
			item = NULL;
		}
	}
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;
	blieng::Item *getItem() { return item; }
	ViewItem *maps[4];
	QSizeF size() { return QSizeF(wid, hei); }
	void setMinimumSize(int w, int h) { wid = w; hei = h; }
	void setBg(QColor c) { bg = c;}

protected:
	int wid;
	int hei;
	QColor bg;

private:
	void loadImage();
	QImage img;
	blieng::Item *item;
	bool moving;

};

class LineData : public QGraphicsLineItem
{
public:
	LineData(ViewItem *from, ViewItem *to, QGraphicsItem *parent=0);
	void updatePos();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	double amount;

private:
	ViewItem *from_item;
	ViewItem *to_item;
	QPolygonF arrowHead;
};

class Items
{
public:
	static Items *getInstance();
	void addToScene(QGraphicsScene *scene);
	void alignItems();
	void update();
	void updateLines(QGraphicsScene *scene);
	bool appendItem(QGraphicsScene *scene, ViewItem *item);
	void deleteItem(QGraphicsScene *scene, ViewItem *item);

private:
	Items();
	ViewItem *findItem(std::string name);
	void rowAlign();
	void groupAlign();
	void addLines(QGraphicsScene *scene);
	void generateViewItems();
	ViewItem *findNext(std::vector<ViewItem *> handled);
	bool doMap(ViewItem *item, ViewItem *last);
	bool isReserved(QPointF pos);
	QPointF findPos(ViewItem *last);

	blieng::Item *base;
	std::vector<std::string> item_names;
	std::vector<ViewItem *> view_items;
	std::vector<LineData *> line_items;

	std::vector<QPointF> reserved;
};

#endif
