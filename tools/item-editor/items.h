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

class ViewItem : public QGraphicsWidget
{
	Q_OBJECT
public:
	ViewItem(blieng::Item *item, QGraphicsItem *parent=0);
	~ViewItem() {}
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
	void loadImage();
	QImage img;
	blieng::Item *item;

};

class Items
{
public:
	Items();
	void addToScene(QGraphicsScene *scene);

private:
	void generateViewItems();
	blieng::Item *base;
	std::vector<std::string> item_names;
	std::vector<ViewItem *> view_items;
};

#endif
