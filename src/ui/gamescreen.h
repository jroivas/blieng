#ifndef __GAMESCREEN_H
#define __GAMESCREEN_H

#include <QtGui>
#include "generate_character.h"
#include "zomb/player_character.h"
#include "mapscreen.h"

namespace ui
{

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

	QGridLayout layout;
	std::vector<zomb::PlayerCharacter *> characters;
};

}

#endif
