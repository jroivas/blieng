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

	addbutton = new SimpleButton("Add");
	okbutton = new SimpleButton("OK");
	layout.addItem(addbutton);
	layout.addItem(okbutton);
	connect(addbutton, SIGNAL(released()), this, SLOT(addLine()));
	connect(okbutton, SIGNAL(released()), this, SLOT(doHide()));

	setFlag(ItemIsMovable);
	setFlag(ItemSendsGeometryChanges);

	setLayout(&layout);
}

void EditorTable::addLine()
{
	model->appendRow(new QStandardItem(""));
	update();
}

void EditorTable::doHide()
{
	setVisible(false);
}
