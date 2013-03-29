#include "editortable.h"

EditorTable::EditorTable(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
	model = new QStandardItemModel(0, 2, this);
	table = new QTableView();

	model->setHorizontalHeaderItem(0, new QStandardItem("Property"));
	model->setHorizontalHeaderItem(1, new QStandardItem("Value"));

	table->setModel(model);
	QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
	proxy->setWidget(table);
	layout.addItem(proxy);


	setLayout(&layout);
}
