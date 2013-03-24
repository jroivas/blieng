#include "datas.h"
#include <QFile>
#include <iostream>

Datas::Datas(QString s)
{
	parseFile(s);
}

void Datas::parseFile(QString s)
{
	QJson::Parser parser;
	QFile f(s);
	ok = false;

	res = parser.parse(&f, &ok);
	f.close();
	if (!ok) std::cout << parser.errorString().toStdString() << "\n";
	else toItems();
}

Item::Item() : base(""), type(""), amount(0), rarity(0), life(0)
{
}

QString Item::toString()
{
	QString tmp = "";
	tmp += "base  : " + base + "\n";
	tmp += "type  : " + type + "\n";
	tmp += "amount: " + QString::number(amount) + "\n";
	tmp += "rarity: " + QString::number(rarity) + "\n";
	tmp += "life  : " + QString::number(life) + "\n";

	QMap<QString, int>::iterator citer = consumes.begin();
	tmp += "consumes: ";
	bool first = true;
	while (citer != consumes.end()) {
		//tmp += *citer;
		if (!first) tmp += ", ";
		first = false;
		tmp += citer.key() + " (" + QString::number(citer.value()) + " units)";
		citer++;
	}
	tmp += "\n";

	return tmp;
}

void Datas::itemProps(Item *item, QVariant val)
{
	if (val.type() == QVariant::Map) {
		QVariantMap map = val.toMap();
		QList<QString> k = map.keys();
		QList<QString>::iterator kiter = k.begin();
		while (kiter != k.end()) {
			QVariant citem = map[*kiter];
			if (*kiter == "type" && citem.type() == QVariant::String) {
				item->type = citem.toString();
			}
			else if (*kiter == "amount" && citem.canConvert(QVariant::Double)) {
				item->amount = citem.toFloat();
			}
			else if (*kiter == "rarity" && citem.canConvert(QVariant::Double)) {
				item->rarity = citem.toFloat();
			}
			else if (*kiter == "life" && citem.canConvert(QVariant::LongLong)) {
				item->life = citem.toLongLong();
			}
			else if (*kiter == "consume" && citem.type() == QVariant::Map) {
				QVariantMap citem_map = citem.toMap();
				QList<QString> consumekeys = citem_map.keys();
				QList<QString>::iterator consume_iter = consumekeys.begin();
				while (consume_iter != consumekeys.end()) {
					if (citem_map[*consume_iter].canConvert(QVariant::Int)) {
						//std::cout<< consume_iter->toStdString() << "=" << citem_map[*consume_iter].toInt() << "\n";
						item->consumes[*consume_iter] = citem_map[*consume_iter].toInt();
					}
					consume_iter++;
				}
			}
			else {
				std::cout << "  " << kiter->toStdString() << "\n";
			}
			kiter++;
		}
	}
}
void Datas::toItems()
{
	QVariant v = res;
	QVariantMap map = v.toMap();
	QList<QString> k = map.keys();
	QList<QString>::iterator kiter = k.begin();
	while (kiter != k.end()) {
		//std::cout << kiter->toStdString() << "\n";
		Item *item = new Item();
		item->base = *kiter;

		QVariant tmp = map[*kiter];
		itemProps(item, tmp);

		items.push_back(item);
		kiter++;
		std::cout << item->toString().toStdString() << "\n";
	}

}
