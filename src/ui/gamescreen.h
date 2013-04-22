#ifndef __GAMESCREEN_H
#define __GAMESCREEN_H

#include <QtGui>
#include "generate_character.h"
#include "zomb/player_character.h"
#include "mapscreen.h"
#include "characterview.h"

namespace ui
{

class GameLayout : public QLayout
{
public:
	enum Position { Top, Right, Bottom, Left, Center };
	GameLayout(QWidget *parent, int margin = 0, int spacing = 0);
	GameLayout(int spacing = 0);
	~GameLayout();

	void addItem(QLayoutItem *item);
	void addWidget(QWidget *widget, Position pos);
	void setGeometry(const QRect &rect);

	QSize sizeHint() const;
	QLayoutItem *itemAt(int index) const;
	QLayoutItem *takeAt(int index);
	int count() const;

private:
	QSize getSizeHint(bool minimum) const;

	class Item {
	public:
		Item(QLayoutItem *i, Position p) : item(i), pos(p) { }

		QLayoutItem *item;
		Position pos;
	};
	void add(QLayoutItem *item, Position pos);

	std::vector<Item* > items;
};

class GameScreen : public QWidget
{
	Q_OBJECT
public:
	GameScreen(QWidget *parent=0);

public slots:
	void newCharacter(zomb::PlayerCharacter *);

private:
	GenerateCharacter *chrgen;
	MapScreen *mapscreen;
	CharacterView *character_view;

	//QGridLayout layout;
	ui::GameLayout layout;
	std::vector<zomb::PlayerCharacter *> characters;
	unsigned int character_count;
};

}

#endif
