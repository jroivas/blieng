#include "gamescreen.h"
#include <boost/foreach.hpp>

using ui::GameScreen;
using ui::GameLayout;

GameScreen::GameScreen(QWidget *parent) : QWidget(parent)
{
	chrgen = new GenerateCharacter(this);
	mapscreen = new MapScreen("world1", this);
	character_view = new CharacterView(this);
	fightscreen = new FightScreen(this);
	fightscreen->setCharacterView(character_view);

	current_location = NULL;
	target_location = NULL;
	walker = new QTimer();
	walker->setInterval(100);

#if 0
	layout.addWidget(chrgen, 0, 0, 0, Qt::AlignLeft);
	layout.addWidget(mapscreen, 0, 1, 1, Qt::AlignHCenter);
	layout.addWidget(character_view, 1, 1, 0, Qt::AlignHCenter | Qt::AlignBottom);
	layout.addWidget(chrgen, 0, 0, Qt::AlignLeft);
	layout.addWidget(mapscreen, 0, 1, Qt::AlignCenter);
	layout.addWidget(character_view, 3, 1, Qt::AlignHCenter | Qt::AlignBottom);
#else
	layout.addWidget(chrgen, GameLayout::Left);
	layout.addWidget(mapscreen, GameLayout::Center);
	layout.addWidget(fightscreen, GameLayout::Center);
	layout.addWidget(character_view, GameLayout::Bottom);
#endif

	//mapscreen->setMaximumWidth(1000);
	mapscreen->setMinimumWidth(400);
	mapscreen->setMinimumHeight(300);
	fightscreen->setMinimumWidth(400);
	fightscreen->setMinimumHeight(300);
	//mapscreen->setMaximumHeight(700);
	character_view->setMinimumHeight(64);
	character_view->setMinimumWidth(400);
	//character_view->setMaximumHeight(64);

	fightscreen->setVisible(false);

	character_count = 3;

	connect(chrgen, SIGNAL(generated(zomb::PlayerCharacter *)), this, SLOT(newCharacter(zomb::PlayerCharacter *)));
	connect(mapscreen, SIGNAL(townSelected(blieng::Town *)), this, SLOT(targetTown(blieng::Town *)));
	connect(walker, SIGNAL(timeout()), this, SLOT(doWalk()));
	connect(this, SIGNAL(fellowship(QPointF)), mapscreen, SLOT(fellowship(QPointF)));
	connect(this, SIGNAL(changeFellowship(std::vector<ui::CharacterData *>)), mapscreen, SLOT(changedFellowship(std::vector<ui::CharacterData *>)));
	connect(this, SIGNAL(changeFellowship(std::vector<ui::CharacterData *>)), fightscreen, SLOT(setFellowship(std::vector<ui::CharacterData *>)));

	connect(this, SIGNAL(enterLocation(blieng::Town *)), SLOT(zombieCheck(blieng::Town *)));
	connect(character_view, SIGNAL(done()), fightscreen, SLOT(act()));
	connect(fightscreen, SIGNAL(fightEnded()), this, SLOT(fightEnded()));
	connect(fightscreen, SIGNAL(killedCharacter(zomb::PlayerCharacter*)), this, SLOT(killedCharacter(zomb::PlayerCharacter*)));

	setLayout(&layout);

	moveToHomeTown();
}

void GameScreen::moveToHomeTown()
{
	BOOST_FOREACH(blieng::Town *town, mapscreen->getMaps()->getTowns()) {
		if (town->isValue("start") && town->getBoolValue("start")) {
			current_location = town;
			blieng::Point pos = town->getPosition();
			emit fellowship(QPointF(pos.x, pos.y));
			return;
		}
	}
}

void GameScreen::newCharacter(zomb::PlayerCharacter *chr)
{
	qDebug() << "New character" << chr;
	characters.push_back(chr);
	if (characters.size() >= character_count) {
		chrgen->hide();
	}
	character_view->setCharacters(characters);
	emit changeFellowship(character_view->getCharacters());
}

void GameScreen::solveTargetPath()
{
	target_path = blieng::Path();
	if (target_location == NULL) return;

	blieng::Point from = current_location->getPosition();
	blieng::Point to = target_location->getPosition();

	std::vector<blieng::Path> test_paths;

	BOOST_FOREACH(blieng::Path path, mapscreen->getMaps()->getPaths()) {
		blieng::Point start = path.getStart();
		if (start == from) {
			test_paths.push_back(path.copy());
		}
	}
	BOOST_FOREACH(blieng::Path path, mapscreen->getMaps()->getRevPaths()) {
		blieng::Point start = path.getStart();
		if (start == from) {
			test_paths.push_back(path.copy());
		}
	}
	
	bool ok = false;
	unsigned int tries = 0;
	do {
		std::vector<blieng::Path> append_test_paths;
		BOOST_FOREACH(blieng::Path continue_path, test_paths) {
			if (!continue_path.isValid()) continue;
			blieng::Point end = continue_path.getEnd();
			if (end.isValid()) {
				BOOST_FOREACH(blieng::Path path, mapscreen->getMaps()->getPaths()) {
					blieng::Point str = path.getStart();
					if (str.isValid() && end == str) {
						append_test_paths.push_back(continue_path.combine(path));
					}
				}
				BOOST_FOREACH(blieng::Path path, mapscreen->getMaps()->getRevPaths()) {
					blieng::Point str = path.getStart();
					if (str.isValid() && end == str) {
						append_test_paths.push_back(continue_path.combine(path));
					}
				}
			}
		}
		BOOST_FOREACH(blieng::Path continue_path, append_test_paths) {
			test_paths.push_back(continue_path);
		}
		BOOST_FOREACH(blieng::Path candi, test_paths) {
			if (candi.getEnd() == to && candi.getStart() == from && (candi.getEnd() != candi.getStart())) {
				ok = true;
				break;
			}
		}
		tries++;
	} while (!ok && tries < 42); //FIXME Maximum supported combined path lenght is 42, ie. maximum 42 combined paths

	double min_len = 9999999999; // FIXME
	BOOST_FOREACH(blieng::Path candi, test_paths) {
		double len = candi.length();
		if (len < min_len) {
			if (candi.getEnd() == to && candi.getStart() == from && (candi.getEnd() != candi.getStart())) {
				target_path = candi;
				waypoint = NULL;
				min_len = len;
			}
		}
	}
	//qDebug() << "THE PATH" << min_len << target_path->toString().c_str();
/*
	BOOST_FOREACH(blieng::Path candi, test_paths) {
		if (candi != target_path) delete candi;
	}
	*/

	test_paths.clear();
	
	walk_progress = 0;
	//walk_speed = 10; //TODO Solve from fellowship speed by slowest character
	walk_speed = 20; //TODO Solve from fellowship speed by slowest character
	walker->start();
}

void GameScreen::doWalk()
{
	if (target_location == NULL) return;
	if (!target_path.isValid()) return;

	if (!waypoint.isValid()) {
		waypoint = target_path.takeFirst();
	}
	if (!waypoint.isValid()) {
		current_location = target_location;
		walker->stop();
		emit enterLocation(current_location);
		return;
	}
	blieng::Point to_point;
	do {
		to_point = target_path.getStart();
		if (!to_point.isValid()) {
			current_location = target_location;
			walker->stop();
			emit enterLocation(current_location);
			return;
		}

		if (to_point == waypoint) {
			waypoint = target_path.takeFirst();
			walk_progress = walk_speed;
		}
		walk_length = waypoint.length(to_point);
		if (walk_length == 0) waypoint = to_point;
	} while (walk_length == 0 || waypoint == to_point);
	
	walk_progress += walk_speed;
	blieng::Point pos = waypoint.traverse(to_point, walk_progress, walk_length);
	if (!pos.isValid()) {
		//Finished
		current_location = target_location;
		walker->stop();
		emit enterLocation(current_location);
	} else if (pos == to_point) {
		waypoint = to_point;
		walk_progress = 0;
		emit fellowship(QPointF(pos.x, pos.y));
	} else {
		//qDebug() << "Walking to " << pos->x << ", " << pos->y;
		emit fellowship(QPointF(pos.x, pos.y));
	}

}

void GameScreen::targetTown(blieng::Town *town)
{
	target_location = town;
	solveTargetPath();
}

void GameScreen::zombieCheck(blieng::Town *town)
{
	unsigned int zombs = town->getZombiesCnt();
	qDebug() << "Zombies here: " << zombs;
	if (zombs>0) {
		mapscreen->setVisible(false);
		character_view->fightMode();
		fightscreen->setTown(town);
		fightscreen->initialize();
		fightscreen->setVisible(true);
		update();
	}
}

void GameScreen::fightEnded()
{
	fightscreen->setVisible(false);
	character_view->mapMode();
	mapscreen->setVisible(true);
	update();
}

void GameScreen::killedCharacter(zomb::PlayerCharacter *chr)
{
	removeCharacter(chr);
	character_view->setCharacters(characters);
	emit changeFellowship(character_view->getCharacters());
	//emit changeFellowship(characters);
}

void GameScreen::removeCharacter(zomb::PlayerCharacter *chr)
{
	if (chr == NULL) return;

	std::vector<zomb::PlayerCharacter *>::iterator it = characters.begin();
	while (it != characters.end()) {
		if (*it == chr) {
			characters.erase(it);
			break;
		}
		++it;
	}
}

