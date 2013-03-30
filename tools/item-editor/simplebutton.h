#ifndef __SIMPLEBUTTON_H
#define __SIMPLEBUTTON_H

#include <QtGui>
#include <QGraphicsWidget>
#include <QGraphicsLayoutItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsSceneMouseEvent> 

class SimpleButton : public QGraphicsWidget
{
	Q_OBJECT
public:
	SimpleButton(QGraphicsWidget *parent=0);
	SimpleButton(QString text, QGraphicsWidget *parent=0);
	void setText(QString new_text) { text = new_text; }
	void setBgColor(QColor bg) { bgcolor = bg; }
	void setPos(const QPointF &pos);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
	//virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
	void pressed();
	void released();

private:
	void init();

	QString text;
	QColor bgcolor;
};

#endif
