#ifndef __FIGHTSCREEN_H
#define __FIGHTSCREEN_H

#include <QtGui>
#include "blieng/town.h"
#include "blieng/character.h"
#include "ui/characterview.h"

namespace ui
{

class ZombieData
{
public:
	ZombieData(blieng::Character *zombie) : zombie(zombie), posx(0), posy(0) {}
	~ZombieData() { zombie = NULL; }
	blieng::Character *zombie;

	double posx;
	double posy;

	double posx_inc;
	double posy_inc;

	double size;
	double default_size;
	QImage image;
};

class FightScreen : public QWidget
{
	Q_OBJECT
public:
	FightScreen(QWidget *parent=0);
	void setTown(blieng::Town *town);
	void paintEvent(QPaintEvent *event);
	void setCharacterView(ui::CharacterView *charview);
	void initialize();

public slots:
	void setFellowship(std::vector<ui::CharacterData *> the_fellowship);
	void act();

signals:
	bool fightEnded();

private:
	void updateZombies(std::vector<blieng::Character*> zombies);
	void calucuateZombieDamage();
	void calculateZombieSpeed();
	void calculateCollidingZombies();
	bool collidingZombies(ui::ZombieData *a, ui::ZombieData *b);
	bool endFight();

	blieng::Town *town;
	std::vector<ui::CharacterData *> fellowship;
	std::vector<ZombieData *> zombies;
	ui::CharacterView *chars;
};

}

#endif
