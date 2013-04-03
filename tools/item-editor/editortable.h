#ifndef __EDITOR_TABLE_H
#define __EDITOR_TABLE_H

#include <QtGui>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QGraphicsProxyWidget>

#include "simplebutton.h"
#include "items.h"

class EditorTable : public QGraphicsWidget
{
	Q_OBJECT
public:
	EditorTable(QGraphicsItem *parent=0);
	void loadItem(ViewItem *item);
	void appendItem(QString key, QString value, bool dep=false);
	void updateItem();

public slots:
	void addLine();
	void doHide();

signals:
	void updated();
	
private:
	QStandardItemModel *model;
	QTableView *table;

	QStandardItemModel *depmodel;
	QTableView *deptable;

	QGraphicsLinearLayout layout;
	QGraphicsLinearLayout but_layout;
	SimpleButton *add_dep_button;
	SimpleButton *ok_button;

	ViewItem *current_item;
};

#endif
