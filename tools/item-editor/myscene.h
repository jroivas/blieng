#ifndef __MYSCENE_H
#define __MYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "items.h"
#include "editortable.h"
#include "simplebutton.h"

class MyScene : public QGraphicsScene
{
	Q_OBJECT
public:
	MyScene(QObject *parent=0);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *);
	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

public slots:
	void updated();
	void newItem();
	void deletedItem(ViewItem *);

private:
	QGraphicsWidget *actions;
	QGraphicsLinearLayout but_layout;
	bool moving;
	ViewItem *move_item;
	ViewItem *edit_item;
	EditorTable *editor;
	SimpleButton *add_item_button;
};

#endif
