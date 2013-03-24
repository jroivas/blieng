#ifndef _DATAS_H
#define _DATAS_H

#include <qjson/parser.h>
#include <QString>
#include <QVariant>

class Item
{
public:
	Item();

	QString toString();
	
	QString base;
	QString type;
	double amount;
	double rarity;
	long int life;
	QMap<QString, int> consumes;
private:
};

class Datas
{
public:
	Datas(QString s);
	bool isOk() { return ok; }
	QVariant getResult() { return res; }

private:
	void parseFile(QString s);
	void itemProps(Item *item, QVariant val);
	void toItems();

	QList<Item*> items;

	bool ok;
	QVariant res;
	
};

#endif
