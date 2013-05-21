#include "fightscreen.h"
#include <boost/foreach.hpp>

using ui::FightScreen;

FightScreen::FightScreen(QWidget *parent) : QWidget(parent), town(NULL), chars(NULL)
{
}

void FightScreen::setTown(blieng::Town *t)
{
	town = t;
	zombies = town->getZombies();
}

void FightScreen::setFellowship(std::vector<ui::CharacterData *> the_fellowship)
{
	fellowship = the_fellowship;
}

void FightScreen::setCharacterView(ui::CharacterView *charview)
{
	chars = charview;
}

void FightScreen::act()
{
}

void FightScreen::paintEvent(QPaintEvent *event)
{
	QPainter paint(this);

	/*double posx = 80;
	double posy = 80;
	*/
	unsigned int wid = fellowship.size();
	unsigned int posx = 0;
	unsigned int posy = 0;
	BOOST_FOREACH(blieng::Character *chr, zombies) {
		if (chr->isValue("image")) {
			int imagenum = chr->getIntValue("image") + 1;
			std::string imagefile = blieng::Data::getInstance()->findFile((boost::format("zomb00%d") % imagenum).str() + ".png");
			QImage image = QImage(imagefile.c_str());

			//QPixmap zomb = QPixmap::fromImage(image.scaled(64, 64));
			paint.drawImage(QPointF(posx * 80, posy * 80), image.scaled(64, 64));
			++posx;
			if (posx >= wid) {
				posx = 0;
				++posy;
			}
		}
	}
}
