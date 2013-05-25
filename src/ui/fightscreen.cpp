#include "fightscreen.h"
#include <boost/foreach.hpp>

using ui::FightScreen;

static double zombieheight = 32;
static const double zombiesize = 48;
static const double safearea = 5;
static const double zombiesteps = 20;

FightScreen::FightScreen(QWidget *parent) : QWidget(parent), town(NULL), chars(NULL)
{
}

void FightScreen::setTown(blieng::Town *t)
{
	town = t;
	updateZombies(town->getCharacterClass("zombie"));
}

void FightScreen::updateZombies(std::vector<blieng::Character*> _zombies)
{
	while (!zombies.empty()) {
		ui::ZombieData *data = zombies.front();
		zombies.erase(zombies.begin());
		delete data;
	}
	BOOST_FOREACH(blieng::Character* zomb, _zombies) {
		if (zomb->isAlive()) {
			ui::ZombieData *zomb_data = new ui::ZombieData(zomb);
			zombies.push_back(zomb_data);
		}
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

void FightScreen::updateZombiePositions()
{
	unsigned int rowcnt = fellowship.size();
	unsigned int posx = 0;
	unsigned int posy = 0;
	
	BOOST_FOREACH(ui::ZombieData *zomb, zombies) {
		zomb->posx = posx;
		if (posy > zomb->posy) zomb->posy = posy;
		zomb->posx_inc = 0;
		zomb->posy_inc = 0;
		zomb->default_size = zombiesize;
		zomb->size = zombiesize;

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
	updateZombiePositions();
}

void FightScreen::setCharacterView(ui::CharacterView *charview)
{
	chars = charview;
}

void FightScreen::zombieDamage(ui::CharacterData* chr, double range_data, double damage_data, unsigned int chrindex)
{
	BOOST_FOREACH(ui::ZombieData* zomb, zombies) {
		if (!zomb->zombie->isAlive()) continue;
		double distance = (height() - safearea - stepsize) - (zomb->posy * stepsize);
		if (distance <= (range_data * stepsize)) {
			double damage = damage_data * chr->character->getRandomDouble(0.0, 2.0);
			double dist = 1 - (distance / (range_data * stepsize));
			damage *= dist;
			if (zomb->posx != chrindex) damage /= 10;
			//qDebug() << "damage" << damage << dist;

			int pos = chr->character->getRandomInt(0, 6);
			double part_health = 0;
			switch (pos) {
				default:
				case 0: break;
				case 1: part_health = zomb->zombie->getDoubleValue("health-arm-left"); break;
				case 2: part_health = zomb->zombie->getDoubleValue("health-leg-left"); break;
				case 3: part_health = zomb->zombie->getDoubleValue("health-arm-right"); break;
				case 4: part_health = zomb->zombie->getDoubleValue("health-leg-right"); break;
				case 5: part_health = zomb->zombie->getDoubleValue("health-torso"); break;
				case 6: part_health = zomb->zombie->getDoubleValue("health-head"); break;
			}	

			part_health -= damage;
			if (part_health<0) part_health = 0;
			//qDebug() << pos << ": "<< part_health;

			switch (pos) {
				default:
				case 0: break;
				case 1: zomb->zombie->setValue("health-arm-left", part_health); break;
				case 2: zomb->zombie->setValue("health-leg-left", part_health); break;
				case 3: zomb->zombie->setValue("health-arm-right", part_health); break;
				case 4: zomb->zombie->setValue("health-leg-right", part_health); break;
				case 5: zomb->zombie->setValue("health-torso", part_health); break;
				case 6:	zomb->zombie->setValue("health-head", part_health);
					if (part_health<=0) {
						zomb->zombie->kill();
						qDebug() << "KILLED zomb";
					}
					break;
			}	
		}
	}
}

void FightScreen::calculateZombieDamage()
{
	fellowship = chars->getCharacters();
	double chrindex = 0;
	BOOST_FOREACH(ui::CharacterData* chr, fellowship) {
		double r = chr->range();
		double d = chr->damage();
		if (r>0 && d>0) {
			zombieDamage(chr, r, d, chrindex);
		}
		chrindex += 1;
	}
}

void FightScreen::calculateZombieSpeed()
{
	stepsize = height() / zombiesteps;
	BOOST_FOREACH(ui::ZombieData* zomb, zombies) {
		if (!zomb->zombie->isAlive()) continue;
		double speed = 1;
		if (zomb->zombie->isValue("speed")) {
			speed = zomb->zombie->getDoubleValue("speed");
		}
		double healthrate = 0;
		double healthnum = 1;
		double active_parts = 0;
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
			active_parts += rate;
		}
		if (zomb->zombie->isValue("health-leg-right")) {
			double rate = zomb->zombie->getDoubleValue("health-leg-right");
			healthrate += rate * 10;
			healthnum += 10;
			active_parts += rate;
		}
		if (zomb->zombie->isValue("health-arm-right")) {
			double rate = zomb->zombie->getDoubleValue("health-arm-right");
			healthrate += rate * 1;
			healthnum += 1;
			active_parts += rate;
		}
		if (zomb->zombie->isValue("health-arm-left")) {
			double rate = zomb->zombie->getDoubleValue("health-arm-left");
			healthrate += rate * 1;
			healthnum += 1;
			active_parts += rate;
		}
		if (zomb->zombie->isValue("health-torso")) {
			double rate = zomb->zombie->getDoubleValue("health-torso");
			healthrate += rate * 2;
			healthnum += 2;
			active_parts += rate;
		}
		if (active_parts > 0) {
			speed *= healthrate/healthnum;
		} else {
			speed = 0;
		}

		zomb->posy += speed;
		if (zomb->posy * stepsize >= (height() - safearea - stepsize)) {
			zomb->posy = (height() - safearea - stepsize) / stepsize;
		}

		zomb->size = zomb->default_size;
		zomb->posx_inc = 0;
		zomb->posy_inc = 0;
	}
}

void FightScreen::calculatePlayerDamage()
{
	std::vector<zomb::PlayerCharacter*> killed;
	fellowship = chars->getCharacters();
	BOOST_FOREACH(ui::ZombieData* zomb, zombies) {
		if (!zomb->zombie->isAlive()) continue;
		if (zomb->posy * stepsize >= (height() - safearea - stepsize)) {
			double chrpos = 0;
			BOOST_FOREACH(ui::CharacterData* chr, fellowship) {
				if (chr->character == NULL) continue;
				if (chr->character->isAlive()) {
					double damage = chr->character->getRandomDouble(0.0, 1.0); //FIXME

					if (zomb->zombie->isValue("radiation")) {
						double rad = zomb->zombie->getDoubleValue("radiation");
						damage *= rad;
					}

					// If not in line, reduce damage
					if (chrpos != zomb->posx) {
						damage /= 10;
					}

					double health = chr->character->getDoubleValue("health");
					health -= damage;
					chr->character->setValue("health", health);
					if (health < 0) {
						qDebug() << "DEAD: " << chr->character->getStringValue("name").c_str();
						chr->character->kill();
						killed.push_back(chr->character);
					}
				}

				chrpos += 1;
			}
		}
	}
	BOOST_FOREACH(zomb::PlayerCharacter* chr, killed) {
		emit killedCharacter(chr);
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
		if (!zomb->zombie->isAlive()) continue;
		BOOST_FOREACH(ui::ZombieData* zomb2, zombies) {
			if (zomb == zomb2) continue;
			if (!zomb2->zombie->isAlive()) continue;
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

bool FightScreen::zombiesKilled()
{
	unsigned int alive_zombies = 0;
	BOOST_FOREACH(ui::ZombieData* zomb, zombies) {
		if (!zomb->zombie->isAlive()) continue;
		alive_zombies++;
	}
	if (alive_zombies == 0) return true;

	return false;
}

bool FightScreen::playerKilled()
{
	fellowship = chars->getCharacters();
	if (fellowship.size()==0) return true;

	return false;
}

void FightScreen::lootWith(ui::CharacterData* chr, ui::CharacterData::LootingMode mode)
{
	BOOST_FOREACH(ui::ZombieData* zomb, zombies) {
		if (zomb->zombie->isAlive() && mode == ui::CharacterData::LOOT_normal) {
			int prob = chr->character->getRandomInt(0, 1000);
			if (prob % 2 == 1) {
				double damage = chr->character->getRandomDouble(0.0, 2.0);

				if (zomb->zombie->isValue("radiation")) {
					double rad = zomb->zombie->getDoubleValue("radiation");
					damage *= rad;
				}

				double health = chr->character->getDoubleValue("health");
				health -= damage;
				chr->character->setValue("health", health);
				if (health < 0) {
					qDebug() << "DEAD: " << chr->character->getStringValue("name").c_str();
					chr->character->kill();
				}
			}
		} else if (!zomb->zombie->isAlive()) {
			//TODO Loot items
		}
	}
}

bool FightScreen::loot()
{
	fellowship = chars->getCharacters();
	bool done = true;

	std::vector<zomb::PlayerCharacter*> killed;
	BOOST_FOREACH(ui::CharacterData* chr, fellowship) {
		ui::CharacterData::LootingMode loot = chr->loot();
		if (loot != ui::CharacterData::LOOT_invalid) {
			if (loot == ui::CharacterData::LOOT_run) {
				//TODO Quit the fight
			}
			else if (loot == ui::CharacterData::LOOT_normal) {
				lootWith(chr, loot);
			}
			else if (loot == ui::CharacterData::LOOT_care) {
				lootWith(chr, loot);
			}

			if (!chr->character->isAlive()) {
				killed.push_back(chr->character);
			}
		} else {
			done = false;
		}
	}

	BOOST_FOREACH(zomb::PlayerCharacter* chr, killed) {
		emit killedCharacter(chr);
	}

	return done;
}

void FightScreen::act()
{
	calculateZombieDamage();
	calculatePlayerDamage();
	calculateZombieSpeed();
	calculateCollidingZombies();
	if (loot()) {
		emit fightEnded();
	}

	update();
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
			paint.drawImage(QPointF(zomb->posx * zombiewidth + border + zomb->posx_inc, zomb->posy * stepsize + zomb->posy_inc), zomb->image.scaled(zomb->size, zomb->size));
		}
#if 0
		if (!zomb->zombie->isAlive()) {
			paint.setBrush(QColor(255,0,0));
			paint.drawRect(QRectF(QPointF(zomb->posx * zombiewidth + border + zomb->posx_inc, zomb->posy * zombieheight + zomb->posy_inc), QSizeF(10, 10)));
		}
#endif
	}
}
