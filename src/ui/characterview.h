#ifndef __CHARACTER_VIEW_H
#define __CHARACTER_VIEW_H

#include <QtGui>
#include <vector>
#include "zomb/player_character.h"

namespace ui
{

class CharacterData
{
public:
	CharacterData() : widget(NULL), box(NULL), character(NULL) {}
	QImage image;
	QLabel *widget;
	QLabel *name_widget;
	QVBoxLayout *box;
	zomb::PlayerCharacter *character;
};

class CharacterView : public QWidget
{
	Q_OBJECT
public:
	CharacterView(QWidget *parent=0);
	void setCharacters(std::vector<zomb::PlayerCharacter *> characters);
	std::vector<ui::CharacterData *> getCharacters();

private:
	void clearData();
	void updateView();

	std::vector<zomb::PlayerCharacter *> characters;
	std::vector<CharacterData *> chrdata;
	QHBoxLayout layout;
};

}

#endif
