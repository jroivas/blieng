#include "gamescreen.h"

using ui::GameScreen;

GameScreen::GameScreen(QWidget *parent) : QWidget(parent)
{
	chrgen = new GenerateCharacter(this);
	mapscreen = new MapScreen("world1", this);

	layout.addWidget(chrgen, 0, 0, 0, Qt::AlignLeft);
	layout.addWidget(mapscreen, 0, 1, 1, Qt::AlignCenter);
	mapscreen->setMinimumWidth(600);
	mapscreen->setMinimumHeight(400);

	connect(chrgen, SIGNAL(generated(zomb::PlayerCharacter *)), this, SLOT(newCharacter(zomb::PlayerCharacter *)));

	setLayout(&layout);
}

void GameScreen::newCharacter(zomb::PlayerCharacter *chr)
{
	qDebug() << "New character" << chr;
	characters.push_back(chr);
}
