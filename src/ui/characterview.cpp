#include "characterview.h"
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "blieng/data.h"

using ui::CharacterView;
using ui::CharacterData;

CharacterView::CharacterView(QWidget *parent) : QWidget(parent)
{
	setLayout(&layout);
}

void CharacterView::setCharacters(std::vector<zomb::PlayerCharacter* > _characters)
{
	characters = _characters;
	updateView();
}

void CharacterView::clearData()
{
	BOOST_FOREACH(CharacterData* chr, chrdata) {
		layout.removeItem(chr->box);
		chr->box->removeWidget(chr->widget);
		chr->box->removeWidget(chr->name_widget);
		delete chr->box;
		delete chr->widget;
		delete chr->name_widget;
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
	BOOST_FOREACH(zomb::PlayerCharacter* chr, characters) {
		if (chr->isValue("image")) {
			int image = chr->getIntValue("image") + 1;
			CharacterData *data = new CharacterData();
			std::string imagefile = blieng::Data::getInstance()->findFile((boost::format("chr00%d") % image).str() + ".png");
			data->character = chr;
			data->image = QImage(imagefile.c_str());
			chrdata.push_back(data);

			data->widget = new QLabel();
			data->widget->setPixmap(QPixmap::fromImage(data->image.scaled(64, 64)));
			data->widget->setMinimumSize(64, 64);

			data->name_widget = new QLabel();
			data->name_widget->setText(chr->getStringValue("name").c_str());

			data->box = new QVBoxLayout();
			data->box->addWidget(data->widget);
			data->box->addWidget(data->name_widget);
			layout.addLayout(data->box);
		}
	}
	update();
}