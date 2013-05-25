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
	typedef enum { LOOT_invalid, LOOT_normal, LOOT_care, LOOT_run } LootingMode;
	CharacterData() : widget(NULL), box(NULL), fight(NULL), character(NULL), active(true) {}
	QImage image;
	QLabel *widget;
	QVBoxLayout *box;
	QLabel *name_widget;
	QComboBox *fight;
	zomb::PlayerCharacter *character;

	double damage();
	double range();
	CharacterData::LootingMode loot();
	bool active;
};

class CharacterView : public QWidget
{
	Q_OBJECT
public:
	CharacterView(QWidget *parent=0);
	void setCharacters(std::vector<zomb::PlayerCharacter *> characters);
	std::vector<ui::CharacterData *> getCharacters();
	void fightMode();
	void mapMode();

signals:
	void done();
	void updatedCharacters(std::vector<ui::CharacterData *>);

private:
	void clearData();
	void updateView();

	std::vector<zomb::PlayerCharacter *> characters;
	std::vector<CharacterData *> chrdata;
	QHBoxLayout layout;
	bool fight;
	QPushButton *act;
};

}

#endif
