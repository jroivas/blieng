#include "simplebutton.h"
#include <QDebug>

SimpleButton::SimpleButton(QGraphicsWidget *parent) : QGraphicsWidget(parent)
{
	init();
}

SimpleButton::SimpleButton(QString text, QGraphicsWidget *parent) : QGraphicsWidget(parent), text(text)
{
	init();
}

/*
QSizeF SimpleButton::sizeHint(Qt::SizeHint, const QSizeF&) const
{
	return QSizeF(100, 100);
}
*/

void SimpleButton::init()
{
	bgcolor = QColor(0, 0, 0, 100);
	setGeometry(QRectF(pos(), size()));
}

void SimpleButton::setPos(const QPointF &pos)
{
	setGeometry(QRectF(pos, size()));
}

void SimpleButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->fillRect(boundingRect(), bgcolor);
	painter->drawText(boundingRect(), text);
}

void SimpleButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	event->accept();
	emit pressed();
	//QGraphicsWidget::mousePressEvent(event);
}

void SimpleButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	emit released();
	event->accept();
	//QGraphicsWidget::mouseReleaseEvent(event);
}

/*
void SimpleButton::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//event->accept();
	//QGraphicsWidget::mouseMoveEvent(event);
}
*/
