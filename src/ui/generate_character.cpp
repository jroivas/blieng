#include "generate_character.h"
#include <list>

using ui::GenerateCharacter;

GenerateCharacter::GenerateCharacter(QWidget *parent) : QWidget(parent)
{
	character = new zomb::PlayerCharacter();
	setLayout(&layout);
	improve_points = 5;
	update();
}

void GenerateCharacter::reroll()
{
	improve_points = 5;
	character->roll();
	update();
}

void GenerateCharacter::clearLayout(QLayout *del_layout)
{
	while (del_layout->count() > 0) {
		QLayoutItem *item = del_layout->itemAt(0);
		if (item != NULL) {
			QLayout *lay = item->layout();
			QWidget *wid = item->widget();
			QSpacerItem *space = item->spacerItem();

			if (lay != NULL) {
				del_layout->removeItem(item);
				clearLayout(lay);
				delete lay;
			}
			else if (wid != NULL) {
				del_layout->removeWidget(wid);
				delete wid;
			}
			else if (space != NULL) {
				del_layout->removeItem(item);
				delete space;
			}
			else {
				printf("else\n");
				break;
			}
		} else {
			printf("null\n");
		}
	}
}
void GenerateCharacter::clear()
{
	clearLayout(&layout);
}

void GenerateCharacter::improve()
{
	if (improve_points > 0) {
		std::string key = improve_map[(QPushButton*)sender()];
		if (character->increase(key)) {
			--improve_points;
			update();
		}
	}
}

void GenerateCharacter::done()
{
	emit generated(character);
	character = new zomb::PlayerCharacter();
	reroll();
}

void GenerateCharacter::update()
{
	clear();

	QPushButton *reroll_button = new QPushButton("Re-roll");
	connect(reroll_button, SIGNAL(clicked()), this, SLOT(reroll()));

	QPushButton *done_button = new QPushButton("Done");
	connect(done_button, SIGNAL(clicked()), this, SLOT(done()));

	layout.addWidget(done_button);
	layout.addWidget(reroll_button);

	QLabel *improve = new QLabel("Improve points left: " + QString::number(improve_points));
	layout.addWidget(improve);

	std::list<std::string> keys = character->getKeys();
	std::list<std::string>::iterator key_iter = keys.begin();
	while (key_iter != keys.end()) {
		QLabel *l = new QLabel(QString(key_iter->c_str()));
		QLabel *l2 = NULL;
		bool ok = false;
		bool can_improve = false;
		try {
			int val = character->getIntValue(*key_iter);
			l2 = new QLabel(QString::number(val));
			ok = true;
			if (key_iter->find("max") != std::string::npos) {
				can_improve = false;
			} else {
				can_improve = true;
			}
		}
		catch (std::string e) {
			ok = false;
		}
		if (!ok) {
			try {
				double val = character->getDoubleValue(*key_iter);
				l2 = new QLabel(QString::number(val));
				ok = true;
				can_improve = false;
			}
			catch (std::string e) {
				ok = false;
			}
		}
		if (!ok) {
			try {
				bool val = character->getBoolValue(*key_iter);
				l2 = new QLabel(val?"Yes":"No");
				ok = true;
				can_improve = false;
			}
			catch (std::string e) {
				ok = false;
			}
		}
		if (!ok) {
			try {
				std::string val = character->getStringValue(*key_iter);
				l2 = new QLabel(QString(val.c_str()));
				ok = true;
				can_improve = false;
			}
			catch (std::string e) { }
		}
		QHBoxLayout *h = new QHBoxLayout();
		h->addWidget(l);
		h->addSpacing(100);
		if (improve_points <= 0) {
			can_improve = false;
		}
		if (l2 != NULL) {
			h->addWidget(l2);
			h->setAlignment(l2, Qt::AlignRight);
			if (can_improve) {
				QPushButton *imp = new QPushButton("+");
				imp->setMaximumWidth(30);
				connect(imp, SIGNAL(clicked()), this, SLOT(improve()));
				improve_map[imp] = *key_iter;
				h->addWidget(imp);
			}
		}
		layout.addLayout(h);
		key_iter++;
	}
}
