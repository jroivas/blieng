#include "fightscreen.h"
#include <boost/foreach.hpp>

using ui::FightScreen;

static double zombieheight = 32;
static double zombiesize = 48;
static double safearea = 5;

FightScreen::FightScreen(QWidget *parent) : QWidget(parent), town(NULL), chars(NULL)
{
}

void FightScreen::setTown(blieng::Town *t)
{
	town = t;
	updateZombies(town->getZombies());
}

void FightScreen::updateZombies(std::vector<blieng::Character*> _zombies)
{
	while (!zombies.empty()) {
		ui::ZombieData *data = zombies.front();
		zombies.erase(zombies.begin());
		delete data;
	}
	BOOST_FOREACH(blieng::Character* zomb, _zombies) {
		ui::ZombieData *zomb_data = new ui::ZombieData(zomb);
		zombies.push_back(zomb_data);
	}
}

void FightScreen::initialize()
{
	unsigned int rowcnt = fellowship.size();
	unsigned int posx = 0;
	unsigned int posy = 0;
	
	BOOST_FOREACH(ui::ZombieData *zomb, zombies) {
		zomb->posx = posx;
		zomb->posy = posy;
		zomb->posx_inc = 0;
		zomb->posy_inc = 0;
		zomb->default_size = zombiesize;
		zomb->size = zombiesize;

		if (zomb->zombie->isValue("image")) {
			int imagenum = zomb->zombie->getIntValue("image") + 1;
			std::string imagefile = blieng::Data::getInstance()->findFile((boost::format("zomb00%d") % imagenum).str() + ".png");
			zomb->image = QImage(imagefile.c_str());
		}

		++posx;
		if (posx >= rowcnt) {
			posx = 0;
			++posy;
		}
	}
}

void FightScreen::setFellowship(std::vector<ui::CharacterData *> the_fellowship)
{
	fellowship = the_fellowship;
}

void FightScreen::setCharacterView(ui::CharacterView *charview)
{
	chars = charview;
}

void FightScreen::calucuateZombieDamage()
{
}

void FightScreen::calculateZombieSpeed()
{
	BOOST_FOREACH(ui::ZombieData* zomb, zombies) {
		double speed = 1;
		if (zomb->zombie->isValue("speed")) {
			speed = zomb->zombie->getDoubleValue("speed");
		}
		double healthrate = 0;
		double healthnum = 0;
		if (zomb->zombie->isValue("health") && zomb->zombie->isValue("health-max")) {
			double rate = zomb->zombie->getDoubleValue("health");
			double maxrate = zomb->zombie->getDoubleValue("health-max");
			if (maxrate != 0) rate /= maxrate;
			else rate /= 100.0;
			healthrate += rate * 1;
			healthnum += 1;
		}
		if (zomb->zombie->isValue("health-leg-left")) {
			double rate = zomb->zombie->getDoubleValue("health-leg-left");
			healthrate += rate * 10;
			healthnum += 10;
		}
		if (zomb->zombie->isValue("health-leg-right")) {
			double rate = zomb->zombie->getDoubleValue("health-leg-right");
			healthrate += rate * 10;
			healthnum += 10;
		}
		if (zomb->zombie->isValue("health-torso")) {
			double rate = zomb->zombie->getDoubleValue("health-torso");
			healthrate += rate * 2;
			healthnum += 2;
		}
		speed *= healthrate/healthnum;

		zomb->posy += speed;
		if (zomb->posy * zombieheight >= (height() - safearea - zombieheight)) {
			zomb->posy = (height() - safearea - zombieheight) / zombieheight;
		}

		zomb->size = zomb->default_size;
		zomb->posx_inc = 0;
		zomb->posy_inc = 0;
	}
}

bool FightScreen::collidingZombies(ui::ZombieData *a, ui::ZombieData *b)
{
	if (a->posx != b->posx) return false;

	double ydiff = a->posy - b->posy;
	if (ydiff < 0) ydiff *= -1;
	if (ydiff > 0.5) return false;

	return true;
}

class CollideData
{
public:
	CollideData() : posx(0), posy(0), count(0) {}
	double posx;
	double posy;
	unsigned int count;
};

void FightScreen::calculateCollidingZombies()
{
	std::vector<CollideData*> coll;
	BOOST_FOREACH(ui::ZombieData* zomb, zombies) {
		BOOST_FOREACH(ui::ZombieData* zomb2, zombies) {
			if (zomb == zomb2) continue;
			if (collidingZombies(zomb, zomb2)) {
				zomb->size = zomb->default_size/2;
				CollideData *collide_data = NULL;
				BOOST_FOREACH(CollideData *cdata, coll) {
					if (cdata->posx == zomb->posx && cdata->posy == round(zomb->posy)) {
						collide_data = cdata;
						break;
					}
				}
				if (collide_data == NULL) {
					collide_data = new CollideData();
					coll.push_back(collide_data);
				}
				collide_data->posx = zomb->posx;
				collide_data->posy = round(zomb->posy);
				collide_data->count++;
				if (collide_data->count == 2) {
					zomb->posx_inc = zomb->size;
				}
				else if (collide_data->count == 3) {
					zomb->posy_inc = zomb->size;
				}
				else if (collide_data->count == 4) {
					zomb->posx_inc = zomb->size;
					zomb->posy_inc = zomb->size;
				}
			}
		}
	}
	while (!coll.empty()) {
		CollideData *tmp = coll.back();
		coll.pop_back();
		delete tmp;
	}
}

bool FightScreen::endFight()
{
	return false;
}

void FightScreen::act()
{
	calucuateZombieDamage();
	calculateZombieSpeed();
	calculateCollidingZombies();
	if (endFight()) {
		emit fightEnded();
	} else {
		update();
	}
}

void FightScreen::paintEvent(QPaintEvent *event)
{
	QPainter paint(this);

	paint.setBrush(QColor(255,0,0));
	paint.drawRect(0, 0, width(), height());
	unsigned int rowcnt = fellowship.size();
	if (rowcnt == 0) {
		emit fightEnded();
		return;
	}

	double zombiewidth = width() * 1.0 / rowcnt - width()/10;
	double border = width()/20;

	BOOST_FOREACH(ui::ZombieData* zomb, zombies) {
		if (!zomb->image.isNull()) {
			paint.drawImage(QPointF(zomb->posx * zombiewidth + border + zomb->posx_inc, zomb->posy * zombieheight + zomb->posy_inc), zomb->image.scaled(zomb->size, zomb->size));
		}
	}
}
