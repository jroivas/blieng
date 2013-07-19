#ifndef __UI_CHARACTERVIEW_H
#define __UI_CHARACTERVIEW_H

#include <QtGui>
#include <QLabel>
#include <QVBoxLayout>
#include <vector>
#include "zomb/player_character.h"
#include "ui/zombpushbutton.h"
#include "ui/zombcombobox.h"

namespace ui
{

class CharacterData
{
public:
    typedef enum { LOOT_invalid, LOOT_fight, LOOT_normal, LOOT_care, LOOT_run } LootingMode;
    CharacterData() : widget(NULL), box(NULL), fight(NULL), group_action(NULL), character(NULL), active(true) {}
    QImage image;
    QLabel *widget;
    QVBoxLayout *box;
    QLabel *name_widget;
    ZombComboBox *fight;
    ZombComboBox *group_action;
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
    QSize sizeHint() const;

signals:
    void done();
    void updatedCharacters(std::vector<ui::CharacterData *>);

private:
    void clearData();
    void updateView();

    std::vector<zomb::PlayerCharacter *> characters;
    std::vector<CharacterData *> chrdata;
    QHBoxLayout layout;
    QVBoxLayout group_layout;
    bool fight;
    ZombComboBox *group_actions;
};

}

#endif
