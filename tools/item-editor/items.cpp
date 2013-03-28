#include "items.h"
#include "data.h"
#include <math.h>
#include <QDebug>

Items *__items_instance = NULL;

Items *Items::getInstance()
{
	if (__items_instance == NULL) {
		__items_instance = new Items();
	}
	return __items_instance;
}

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
	BOOST_FOREACH(ViewItem *item, view_items) {
		if (item->getItem()->base != "") {
			scene->addItem(item);
			item->setVisible(true);
		}
	}
	alignItems();
	addLines(scene);
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

ViewItem *Items::findNext(std::vector<ViewItem *> handled)
{
	ViewItem *max_item = NULL;
	int max_cnt = -1;
	BOOST_FOREACH(ViewItem *item, view_items) {
		std::vector<std::string> cc = item->getItem()->getConsumes();
		if ((int)cc.size() > max_cnt) {
			bool hand = false;
			BOOST_FOREACH(ViewItem *hh, handled) {
				if (hh == item) {
					hand = true;
					break;
				}
			}
			if (!hand) {
				max_cnt = cc.size();
				max_item = item;
			}
		}
	}
	return max_item;
}

bool Items::isReserved(QPointF pos)
{
	BOOST_FOREACH(QPointF test, reserved) {
		if (test == pos) return true;
	}
	
	return false;
}

QPointF Items::findPos(ViewItem *last)
{
	QPointF pp = last->pos();
	
	int x = pp.x();
	int y = pp.y();
	int diff = 100;
	//QPointF tmp = QPointF(pp.x(), pp.y());
	QPointF tmp = QPointF(pp.x() + diff, pp.y());
	if (!isReserved(tmp)) return tmp;

	tmp = QPointF(pp.x(), pp.y() + diff);
	if (!isReserved(tmp)) return tmp;

	tmp = QPointF(pp.x(), pp.y() - diff);
	if (!isReserved(tmp)) return tmp;

	tmp = QPointF(pp.x() - diff, pp.y());
	if (!isReserved(tmp)) return tmp;

	tmp = QPointF(pp.x() + diff, pp.y() + diff);
	if (!isReserved(tmp)) return tmp;

	tmp = QPointF(pp.x() + diff, pp.y() - diff);
	if (!isReserved(tmp)) return tmp;

	tmp = QPointF(pp.x() - diff, pp.y() + diff);
	if (!isReserved(tmp)) return tmp;

	tmp = QPointF(pp.x() - diff, pp.y() - diff);
	if (!isReserved(tmp)) return tmp;

	/*
	ok = true;
	if (i==0) item->setPos(pp.x(), pp.y() + 50);
	else if (i==1) item->setPos(pp.x() + 50, pp.y());
	else if (i==2) item->setPos(pp.x() - 50, pp.y());
	else if (i==3) item->setPos(pp.x(), pp.y() - 50);
	else ok = false;
	*/

	return tmp;
}

bool Items::doMap(ViewItem *item, ViewItem *last)
{
	bool ok = false;
	for (int i=0; i<4; i++) {
		if (last->maps[i] == NULL) {
			last->maps[i] = item;
			QPointF pp = findPos(last);
			reserved.push_back(pp);
			item->setPos(pp);
			ok = true;
			qDebug() << "mapped to" << item->pos();
			break;
		}
	}
	if (!ok) {
		for (int i=0; i<4; i++) {
			if (doMap(item, last->maps[i])) {
				ok = true;
				break;
			}
		}
		
	}
	return ok;
}

void Items::groupAlign()
{
	unsigned int posx = 0;
	unsigned int posy = 0;
	ViewItem *last = NULL;
	std::vector<ViewItem *> handled;
	qDebug() << "aligning";
	while (true) {
		ViewItem *n = findNext(handled);
		if (n == NULL) {
			qDebug() << "oho";
			break;
		}
		handled.push_back(n);
		if (last != NULL) {
			qDebug() << "mapping";
			if (!doMap(n, last)){
				qDebug() << "No map";
			}
		} else {
			qDebug() << "nolast";
			last = n;
			n->setPos(posx, posy);	
			reserved.push_back(QPointF(posx, posy));
		}
	}
}

void Items::alignItems()
{
	//rowAlign();
	groupAlign();
}

ViewItem *Items::findItem(std::string name)
{
	ViewItem *res = NULL;

	BOOST_FOREACH(ViewItem *s, view_items) {
		if (s->getItem()->base == name) {
			res = s;
			break;
		}
	}

	return res;
}

void Items::addLines(QGraphicsScene *scene)
{
	BOOST_FOREACH(ViewItem *src, view_items) {
		std::vector<std::string> cc = src->getItem()->getConsumes();
		BOOST_FOREACH(std::string cons, cc) {
			ViewItem *target = findItem(cons);
			if (target != NULL) {
				LineData *tmp = new LineData(src, target);
				line_items.push_back(tmp);
				scene->addItem(tmp);
			}
		}
	}
}

void Items::update()
{
	BOOST_FOREACH(LineData *line, line_items) {
		line->updatePos();
	}
}

LineData::LineData(ViewItem *from, ViewItem *to, QGraphicsItem *parent) : QGraphicsLineItem(parent), from_item(from), to_item(to)
{
	updatePos();
}


void LineData::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsLineItem::paint(painter, option, widget);

	/*
	qreal arrowSize = 20;
	QBrush br = QBrush(Qt::SolidPattern);
	br.setColor(Qt::blue);
	painter->setBrush(br);
	*/

	//painter->drawEllipse(line().p2(), 2, 2);
	/*
	double angle = ::acos(line().dx() / line().length());
	if (line().dy() > 0) {
		angle = (Pi * 2) - angle;
		QPointF arrowP1 = line().p2() + QPointF(sin(angle + Pi / 3) * arrowSize, cos(angle + Pi / 3) * arrowSize);
		QPointF arrowP2 = line().p2() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);

		arrowHead.clear();
		arrowHead << line().p2() << arrowP1 << arrowP2;
		painter->drawLine(line());
		painter->drawPolygon(arrowHead);
	} else {
		QPointF arrowP1 = line().p2() + QPointF(sin(angle + Pi / 3) * arrowSize, cos(angle + Pi / 3) * arrowSize);
		QPointF arrowP2 = line().p2() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);

		arrowHead.clear();
		arrowHead << line().p2() << arrowP1 << arrowP2;
		painter->drawLine(line());
		painter->drawPolygon(arrowHead);
	}
	*/
	/*
	double angle = ::acos(line().dx() / line().length());
	if (line().dy() > 0) {
		angle = (Pi * 2) - angle;
		QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize, cos(angle + Pi / 3) * arrowSize);
		QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);

		arrowHead.clear();
		arrowHead << line().p1() << arrowP1 << arrowP2;
		painter->drawLine(line());
		painter->drawPolygon(arrowHead);
	}
	*/
}

void LineData::updatePos()
{
	QPointF pos1 = from_item->pos() + from_item->boundingRect().center();
	QPointF pos2 = to_item->pos() + to_item->boundingRect().center();
	//QLineF line = QLineF(, to_item->boundingRect().center())
	QLineF line = QLineF(pos1, pos2);
	setLine(line);
}

ViewItem::ViewItem(blieng::Item *item, QGraphicsItem *parent) : QGraphicsItem(parent), item(item)
{
	moving = false;
	setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
	//setAcceptedMouseButtons(Qt::LeftButton);
	setAcceptHoverEvents(true);
	setMinimumSize(60, 60);
	maps[0] = NULL;
	maps[1] = NULL;
	maps[2] = NULL;
	maps[3] = NULL;
	bg = QColor(0,0,0,0);
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
	painter->fillRect(boundingRect(), bg);
	if (!img.isNull()) {
		painter->drawImage(boundingRect().topLeft(), img);
	} else {
		painter->drawText(boundingRect(), QString(item->base.get().c_str()));
	}
}


#if 0
void ViewItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ShiftModifier)) {
		qDebug() << "Moving";
		moving = true;
	}
	qDebug() << "Moving";
	moving = true;
}

void ViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	/*
	if (event->buttons() & 0x1) {
	}
	*/
	qDebug() << "NOT Moving";
	moving = false;
}

void ViewItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (moving) {
		qDebug() << "do Move";
		setPos(event->pos());
	}
	qDebug() << "do Move";
}

void ViewItem::hoverMoveEvent (QGraphicsSceneHoverEvent *event)
{
	qDebug() << "hovering";
}
bool ViewItem::sceneEvent ( QEvent * event )
{
	qDebug() << "eventing";
	return QGraphicsWidget::sceneEvent(event);
}

     void ViewItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	qDebug() << "drag1";
	event->setAccepted(true);
}
     void ViewItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	qDebug() << "drag2";
}
     void ViewItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	qDebug() << "drag3";
}
#endif
