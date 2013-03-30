#ifndef __EDITOR_TABLE_H
#define __EDITOR_TABLE_H

#include <QtGui>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QGraphicsProxyWidget>

#include "simplebutton.h"

class EditorTable : public QGraphicsWidget
{
	Q_OBJECT
public:
	EditorTable(QGraphicsItem *parent=0);

public slots:
	void addLine();
	void doHide();
	
private:
	QStandardItemModel *model;
	QTableView *table;
	QGraphicsLinearLayout layout;
	SimpleButton *addbutton;
	SimpleButton *okbutton;
};

#endif
