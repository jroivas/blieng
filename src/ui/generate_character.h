#ifndef __UI_GENREATE_CHARACTER_H
#define __UI_GENREATE_CHARACTER_H

#include <QtGui>
#include <map>
#include "zomb/player_character.h"

namespace ui
{

class GenerateCharacter : public QWidget
{
	Q_OBJECT
public:
	GenerateCharacter(QWidget *parent=0);

public Q_SLOTS:
	void reroll();
	void improve();
	void done();

signals:
	void generated(zomb::PlayerCharacter *);

private:
	void clearLayout(QLayout *del_layout);
	void clear();
	void update();

	zomb::PlayerCharacter *character;
	QVBoxLayout layout;
	int improve_points;
	std::map<QPushButton*, std::string> improve_map;
};

}

#endif
