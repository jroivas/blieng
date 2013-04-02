#include "editortable.h"
#include <string>

EditorTable::EditorTable(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
	model = new QStandardItemModel(0, 2, this);
	depmodel = new QStandardItemModel(0, 2, this);
	table = new QTableView();
	deptable = new QTableView();

	model->setHorizontalHeaderItem(0, new QStandardItem("Property"));
	model->setHorizontalHeaderItem(1, new QStandardItem("Value"));
	table->setModel(model);

	depmodel->setHorizontalHeaderItem(0, new QStandardItem("Dependecy"));
	depmodel->setHorizontalHeaderItem(1, new QStandardItem("Amount"));
	deptable->setModel(depmodel);

	QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget();
	proxy->setWidget(table);
	layout.addItem(proxy);

	QGraphicsProxyWidget *depproxy = new QGraphicsProxyWidget();
	depproxy->setWidget(deptable);
	layout.addItem(depproxy);


	but_layout.setOrientation(Qt::Vertical);
	addbutton = new SimpleButton("Add dep");
	okbutton = new SimpleButton("OK");
	but_layout.addItem(addbutton);
	but_layout.addItem(okbutton);
	layout.addItem(&but_layout);

	connect(addbutton, SIGNAL(released()), this, SLOT(addLine()));
	connect(okbutton, SIGNAL(released()), this, SLOT(doHide()));

	setFlag(ItemIsMovable);
	setFlag(ItemSendsGeometryChanges);

	setLayout(&layout);
}

void EditorTable::addLine()
{
	depmodel->appendRow(new QStandardItem(""));
	update();
}

void EditorTable::doHide()
{
	updateItem();
	setVisible(false);
	emit updated();
}

void EditorTable::appendItem(QString key, QString value, bool dep)
{
	QList<QStandardItem *> items;
	items.push_back(new QStandardItem(key));
	items.push_back(new QStandardItem(value));
	if (dep) {
		depmodel->appendRow(items);
	} else {
		model->appendRow(items);
	}
}

void EditorTable::updateItem()
{
	current_item->getItem()->clearConsume();
	std::map<std::string, double> datas;

	while (depmodel->rowCount()>0) {
		QList<QStandardItem *> rows = depmodel->takeRow(0);
		bool found = false;
		if (rows.size() == 2) {
			std::string nowitem = rows[0]->text().toStdString();
			bool ok = false;
			double nowval = rows[1]->text().toDouble(&ok);
			if (nowitem != "" && ok) {
				datas[nowitem] = nowval;
			}
		} else qDebug() << "Invalid row" << rows << "len" << rows.size();

	}
	current_item->getItem()->setConsume(datas);
}

void EditorTable::loadItem(ViewItem *item)
{
	if (item==NULL) return;
	current_item = item;

	model->clear();
	depmodel->clear();

	model->setHorizontalHeaderItem(0, new QStandardItem("Property"));
	model->setHorizontalHeaderItem(1, new QStandardItem("Value"));
	depmodel->setHorizontalHeaderItem(0, new QStandardItem("Dependecy"));
	depmodel->setHorizontalHeaderItem(1, new QStandardItem("Amount"));

	appendItem("base", item->getItem()->base.get().c_str());
	appendItem("type", item->getItem()->type.get().c_str());
	appendItem("image", item->getItem()->image.get().c_str());
	appendItem("rarity", QString::number(item->getItem()->rarity));
	appendItem("amount", QString::number(item->getItem()->amount));
	appendItem("life", QString::number(item->getItem()->life));

	BOOST_FOREACH(std::string consume, item->getItem()->getConsumes()) {
		appendItem(consume.c_str(), QString::number(item->getItem()->consumeCount(consume)), true);
	}

	update();
}
