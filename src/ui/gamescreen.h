#ifndef __GAMESCREEN_H
#define __GAMESCREEN_H

#include "ui/gamelayout.h"
#include <QtGui>
#include "generate_character.h"
#include "zomb/player_character.h"
#include "blieng/town.h"
#include "mapscreen.h"
#include "characterview.h"
#include "fightscreen.h"

namespace ui
{

class GameScreen : public QWidget
{
	Q_OBJECT
public:
	GameScreen(QWidget *parent=0);

public slots:
	void newCharacter(zomb::PlayerCharacter *);
	void targetTown(blieng::Town *);
	void doWalk();
	void zombieCheck(blieng::Town *);
	void fightEnded();
	void killedCharacter(zomb::PlayerCharacter*);

signals:
	void fellowship(QPointF);
	void changeFellowship(std::vector<ui::CharacterData *>);
	void enterLocation(blieng::Town *);

private:
	void moveToHomeTown();
	void solveTargetPath();
	void removeCharacter(zomb::PlayerCharacter *chr);

	GenerateCharacter *chrgen;
	MapScreen *mapscreen;
	FightScreen *fightscreen;
	CharacterView *character_view;

	blieng::Town *current_location;
	blieng::Town *target_location;
	blieng::Path target_path;
	blieng::Point waypoint;
	double walk_progress;
	double walk_length;
	double walk_speed;

	ui::GameLayout layout;
	std::vector<zomb::PlayerCharacter *> characters;
	unsigned int character_count;

	QTimer *walker;
};

}

#endif
