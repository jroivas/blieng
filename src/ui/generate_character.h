#ifndef __GENREATE_CHARACTER_H
#define __GENREATE_CHARACTER_H

#include <QtGui>
#include <map>
#include "blieng/player_character.h"

class GenerateCharacter : public QWidget
{
	Q_OBJECT
public:
	GenerateCharacter();

public Q_SLOTS:
	void reroll();
	void improve();

private:
	void clearLayout(QLayout *del_layout);
	void clear();
	void update();

	blieng::PlayerCharacter *character;
	QVBoxLayout layout;
	int improve_points;
	std::map<QPushButton*, std::string> improve_map;
};

#endif
