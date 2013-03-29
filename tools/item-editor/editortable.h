#ifndef __EDITOR_TABLE_H
#define __EDITOR_TABLE_H

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QGraphicsProxyWidget>

class EditorTable : public QGraphicsWidget
{
	Q_OBJECT
public:
	EditorTable(QGraphicsItem *parent=0);
	
private:
	QStandardItemModel *model;
	QTableView *table;
	QGraphicsLinearLayout layout;
};

#endif
