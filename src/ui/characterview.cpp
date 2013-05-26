#include "characterview.h"
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "blieng/data.h"
#include "blieng/configure.h"

using ui::CharacterView;
using ui::CharacterData;

CharacterView::CharacterView(QWidget *parent) : QWidget(parent), fight(false)
{
    setLayout(&layout);
    act = new QPushButton("Act");
    act->setMinimumWidth(blieng::Configure::getInstance()->getUIntValue("act_button_width"));
    act->setMinimumHeight(blieng::Configure::getInstance()->getUIntValue("act_button_height"));
    connect(act, SIGNAL(clicked()), this, SIGNAL(done()));
}

void CharacterView::setCharacters(std::vector<zomb::PlayerCharacter* > _characters)
{
    characters = _characters;
    updateView();
}

void CharacterView::fightMode()
{
    fight = true;

    act->show();
    updateView();
}

void CharacterView::mapMode()
{
    fight = false;

    act->hide();
    updateView();
}

void CharacterView::clearData()
{
    layout.removeWidget(act);
    BOOST_FOREACH(CharacterData* chr, chrdata) {
        layout.removeItem(chr->box);
        chr->box->removeWidget(chr->widget);
        chr->box->removeWidget(chr->name_widget);
        delete chr->box;
        delete chr->widget;
        delete chr->name_widget;
        if (chr->fight != NULL) delete chr->fight;
        delete chr;
    }
    chrdata.clear();
}

std::vector<ui::CharacterData *> CharacterView::getCharacters()
{
    return chrdata;
}

void CharacterView::updateView()
{
    clearData();
    unsigned int chr_size = blieng::Configure::getInstance()->getUIntValue("chr_size");
    BOOST_FOREACH(zomb::PlayerCharacter* chr, characters) {
        if (chr->isValue("image")) {
            int imagenum = chr->getIntValue("image") + 1;
            CharacterData *data = new CharacterData();

            std::string fname = blieng::Configure::getInstance()->getStringValue("chr_images");
            fname = blieng::Data::getInstance()->formatString(fname, imagenum);

            std::string imagefile = blieng::Data::getInstance()->findFile(fname);
            data->character = chr;
            data->image = QImage(imagefile.c_str());
            chrdata.push_back(data);

            data->widget = new QLabel();
            data->widget->setPixmap(QPixmap::fromImage(data->image.scaled(chr_size, chr_size)));
            data->widget->setMinimumSize(chr_size, chr_size);

            data->name_widget = new QLabel();
            data->name_widget->setText(chr->getStringValue("name").c_str());

            data->box = new QVBoxLayout();
            data->box->addWidget(data->widget);
            data->box->addWidget(data->name_widget);
            data->active = true;

            if (fight) {
                data->fight = new QComboBox();
                // TODO Dynamic options
                data->fight->addItem("Gun");
                data->fight->addItem("Machine gun");
                data->fight->addItem("Kick");
                data->fight->addItem("Hit");
                data->fight->addItem("Loot");
                data->fight->addItem("Loot with care");
                data->fight->addItem("Run");
                data->box->addWidget(data->fight);
            }

            layout.addLayout(data->box);
        }
    }
    if (fight) {
            layout.addWidget(act);
    }
    emit updatedCharacters(chrdata);
    update();
}

double CharacterData::range()
{
    if (fight == NULL) return 0;
    //TODO: via item options
    int index = fight->currentIndex();
    if (index == 0) return 10;
    if (index == 1) return 10;
    if (index == 2) return 1;
    if (index == 3) return 1;
    return 0;
}

double CharacterData::damage()
{
    if (fight == NULL) return 0;
    //TODO: via item options
    int index = fight->currentIndex();
    if (index == 0) return 1;
    if (index == 1) return 3;
    if (index == 2) return 0.3;
    if (index == 3) return 0.1;
    return 0;
}

ui::CharacterData::LootingMode CharacterData::loot()
{
    if (fight == NULL) return LOOT_invalid;
    int index = fight->currentIndex();

    if (fight->itemText(index) == "Loot") {
        active = false;
        return ui::CharacterData::LOOT_normal;
    }
    else if (fight->itemText(index) == "Loot with care") {
        active = false;
        return ui::CharacterData::LOOT_care;
    }
    else if (fight->itemText(index) == "Run") {
        active = false;
        return ui::CharacterData::LOOT_run;
    }

    return LOOT_invalid;
}
