#include "items.h"
#include "data.h"

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
	//QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
	BOOST_FOREACH(ViewItem *s, view_items) {
		//layout->addItem(s);
		scene->addItem(s);
		//layout->addItem(scene->addWidget(s));
		s->show();
	}
	/*QGraphicsWidget *form = new QGraphicsWidget;
	form->setLayout(layout);
	scene->addItem(form);
	*/
}

ViewItem::ViewItem(blieng::Item *item, QGraphicsItem *parent) : QGraphicsWidget(parent), item(item)
{
	setMinimumSize(25, 25);
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

void ViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	loadImage();
	if (!img.isNull()) {
		static int posx = -100;
		static int posy = -100;
		painter->drawImage(posx, posy, img);
		posy += 50;
		posx += 50;
	}
}
