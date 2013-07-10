#include "gamescreen.h"
#include <boost/foreach.hpp>
#include "blieng/bliobject.h"
#include "blieng/configure.h"
#include "zomb/zombie_character.h"

using ui::GameScreen;
using ui::GameLayout;

GameScreen::GameScreen(QString map, QWidget *parent) : QWidget(parent)
{
    chrgen = new GenerateCharacter(this);
    mapscreen = new MapScreen(map, this);
    character_view = new CharacterView(this);
    fightscreen = new FightScreen(this);
    fightscreen->setCharacterView(character_view);

    current_location = NULL;
    target_location = NULL;
    walker = new QTimer();
    walker->setInterval(blieng::Configure::getInstance()->getUIntValue("walker_interval"));

    clock = new zomb::WorldClock();

    layout.addWidget(chrgen, GameLayout::Left);
    layout.addWidget(mapscreen, GameLayout::Center);
    layout.addWidget(fightscreen, GameLayout::Center);
    layout.addWidget(character_view, GameLayout::Bottom);

    mapscreen->setMinimumWidth(blieng::Configure::getInstance()->getUIntValue("map_width_min"));
    mapscreen->setMinimumHeight(blieng::Configure::getInstance()->getUIntValue("map_height_min"));
    mapscreen->setEnabled(false);

    fightscreen->setMinimumWidth(blieng::Configure::getInstance()->getUIntValue("map_width_min"));
    fightscreen->setMinimumHeight(blieng::Configure::getInstance()->getUIntValue("map_height_min"));

    character_view->setMinimumHeight(blieng::Configure::getInstance()->getUIntValue("chr_height_min"));
    character_view->setMinimumWidth(blieng::Configure::getInstance()->getUIntValue("chr_width_min"));

    fightscreen->setVisible(false);

    character_count = blieng::Configure::getInstance()->getUIntValue("chr_count");

    connect(chrgen, SIGNAL(generated(zomb::PlayerCharacter *)), this, SLOT(newCharacter(zomb::PlayerCharacter *)));
    connect(mapscreen, SIGNAL(townSelected(blieng::Town *)), this, SLOT(targetTown(blieng::Town *)));
    connect(walker, SIGNAL(timeout()), this, SLOT(doWalk()));
    connect(this, SIGNAL(fellowship(QPointF)), mapscreen, SLOT(fellowship(QPointF)));
    //connect(this, SIGNAL(changeFellowship(std::vector<ui::CharacterData *>)), mapscreen, SLOT(changedFellowship(std::vector<ui::CharacterData *>)));
    //connect(this, SIGNAL(changeFellowship(std::vector<ui::CharacterData *>)), fightscreen, SLOT(setFellowship(std::vector<ui::CharacterData *>)));
    connect(character_view, SIGNAL(updatedCharacters(std::vector<ui::CharacterData *>)), mapscreen, SLOT(changedFellowship(std::vector<ui::CharacterData *>)));
    connect(character_view, SIGNAL(updatedCharacters(std::vector<ui::CharacterData *>)), fightscreen, SLOT(setFellowship(std::vector<ui::CharacterData *>)));

    connect(this, SIGNAL(enterLocation(blieng::Town *)), SLOT(zombieCheck(blieng::Town *)));
    connect(character_view, SIGNAL(done()), fightscreen, SLOT(act()));
    connect(fightscreen, SIGNAL(fightEnded()), this, SLOT(fightEnded()));
    connect(fightscreen, SIGNAL(killedCharacter(zomb::PlayerCharacter*)), this, SLOT(killedCharacter(zomb::PlayerCharacter*)));

    connect(clock, SIGNAL(randomTick()), this, SLOT(zombieProceed()));

    setLayout(&layout);

    moveToHomeTown();
}

void GameScreen::moveToHomeTown()
{
    blieng::Town *start_town = NULL;

    BOOST_FOREACH(blieng::Town *town, mapscreen->getMaps()->getTowns()) {
        /* Ensure we have a home town*/
        if (start_town == NULL) start_town = town;
        /* Try to check for the proper home town */
        if (town->isValue("start") && town->getBoolValue("start")) {
            start_town = town;
            break;
        }
    }

    if (start_town != NULL) {
            current_location = start_town;
            blieng::Point pos = start_town->getPosition();
            emit fellowship(QPointF(pos.x, pos.y));
    }
}

void GameScreen::newCharacter(zomb::PlayerCharacter *chr)
{
    qDebug() << "New character" << chr;
    characters.push_back(chr);
    if (characters.size() >= character_count) {
        chrgen->hide();
        mapscreen->setEnabled(true);
    }
    character_view->setCharacters(characters);
    //emit changeFellowship(character_view->getCharacters());
}

void GameScreen::solveTargetPath()
{
    target_path = blieng::Path();
    if (target_location == NULL) return;

    blieng::Point from = current_location->getPosition();
    blieng::Point to = target_location->getPosition();

    std::vector<blieng::Path> test_paths;

    unsigned int path_diff = blieng::Configure::getInstance()->getUIntValue("path_selection_resolution");

    BOOST_FOREACH(blieng::Path path, mapscreen->getMaps()->getPaths()) {
        blieng::Point start = path.getStart();
        if (start.length(from) < path_diff) {
            test_paths.push_back(path.copy());
        }
    }
    BOOST_FOREACH(blieng::Path path, mapscreen->getMaps()->getRevPaths()) {
        blieng::Point start = path.getStart();
        if (start.length(from) < path_diff) {
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
                    if (str.isValid() && end.length(str) < path_diff) {
                        append_test_paths.push_back(continue_path.combine(path));
                    }
                }
                BOOST_FOREACH(blieng::Path path, mapscreen->getMaps()->getRevPaths()) {
                    blieng::Point str = path.getStart();
                    if (str.isValid() && end.length(str) < path_diff) {
                        append_test_paths.push_back(continue_path.combine(path));
                    }
                }
            }
        }
        BOOST_FOREACH(blieng::Path continue_path, append_test_paths) {
            test_paths.push_back(continue_path);
        }
        BOOST_FOREACH(blieng::Path candi, test_paths) {
            if ((candi.getEnd().length(to) < path_diff) && (candi.getStart().length(from) < path_diff) && (candi.getEnd() != candi.getStart())) {
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
            if ((candi.getEnd().length(to) < path_diff) && (candi.getStart().length(from) < path_diff) && (candi.getEnd() != candi.getStart())) {
                target_path = candi;
                waypoint = NULL;
                min_len = len;
            }
        }
    }

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
    unsigned int zombs = town->getCharacterClassCnt("zombie");
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
    //emit changeFellowship(character_view->getCharacters());
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

void GameScreen::zombieProceed()
{
    BOOST_FOREACH(blieng::Town *town, mapscreen->getMaps()->getTowns()) {
        unsigned int townpopl = town->getPopulation();

        if (townpopl == 0) continue;

        unsigned int townzomb = town->getCharacterClassCnt("zombie");
        double zombie_rate = 0.0;
        zombie_rate = double(townzomb) / double(townpopl);
        if (blieng::Configure::getInstance()->getBoolValue("debug")) qDebug() << "rte" << townzomb << townpopl << zombie_rate;

        std::vector<blieng::Character *> killed;
        QString popula = "";
        BOOST_FOREACH(blieng::Character *chr, town->getCharacters()) {
            if (chr->isAlive() && (chr->isValue("class") && chr->getStringValue("class") != "zombie")) {
                double bite_prob = blieng::Configure::getInstance()->getDoubleValue("bite_prob");
                double bite_threshold = blieng::Configure::getInstance()->getDoubleValue("bite_threshold");
                double zombie_limiter = blieng::Configure::getInstance()->getDoubleValue("zombie_limiter");
                double bitten = blieng::BliObject::getRandomDouble(0.0, bite_prob);

                bitten += (zombie_rate/zombie_limiter);
                QString bb = QString::number(bitten);
                popula += (bitten>bite_threshold?"BITTEN ":bb.left(5) + " ");

                // Now bitten is 0..0.99 + (0..1/10), so probability get bitten is
                // higher when more zombies in the town
                if (bitten >= bite_threshold) {
                    zomb::PlayerCharacter *pchr = dynamic_cast<zomb::PlayerCharacter*>(chr);
                    if (pchr != NULL) {
                        //qDebug() << "killed " << chr->getStringValue("name").c_str();
                        zomb::ZombieCharacter *zomb = new zomb::ZombieCharacter();
                        zomb->fromPlayerCharacter(pchr);
                        town->addCharacter(zomb);
                        killed.push_back(chr);
                    }
                }
            }
        }
        if (blieng::Configure::getInstance()->getBoolValue("debug")) qDebug() << "   " << zombie_rate << popula;
        BOOST_FOREACH(blieng::Character *chr, killed) {
            town->removeCharacter(chr);
        }
    }
}
