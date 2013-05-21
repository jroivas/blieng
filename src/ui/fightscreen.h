#ifndef __FIGHTSCREEN_H
#define __FIGHTSCREEN_H

#include <QtGui>
#include "blieng/town.h"
#include "blieng/character.h"
#include "ui/characterview.h"

namespace ui
{

class FightScreen : public QWidget
{
	Q_OBJECT
public:
	FightScreen(QWidget *parent=0);
	void setTown(blieng::Town *town);
	void paintEvent(QPaintEvent *event);
	void setCharacterView(ui::CharacterView *charview);

public slots:
	void setFellowship(std::vector<ui::CharacterData *> the_fellowship);
	void act();

private:
	blieng::Town *town;
	std::vector<ui::CharacterData *> fellowship;
	std::vector<blieng::Character *> zombies;
	ui::CharacterView *chars;
};

}

#endif
