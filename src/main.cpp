#include <QtCore>
#include <QtGui>

#include <iostream>
#include "ui/generate_character.h"
#include "ui/startscreen.h"
#include "ui/loadingscreen.h"
#include "ui/mapscreen.h"
#include "ui/gamescreen.h"
#include "blieng/item.h"
#include "blieng/maps.h"
#include "blieng/wallclock.h"
#include "blieng/configure.h"
#include "zomb/zombie_character.h"
#include "zomb/worldclock.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	#if 0
	blieng::Item *nail = new blieng::Item("nail");
	blieng::Item *iron = new blieng::Item("iron");
	iron->setUsable();
	std::cout << nail->toString() << "\n";
	std::cout << iron->toString() << "\n";
	nail->consume(iron);
	std::cout << iron->toString() << "\n";
	
	blieng::Item *new_nail = nail->produce();
	if (new_nail != NULL) {
		std::cout << new_nail->toString() << "\n";
	} else {
		std::cout << "Can't produce nail\n";
	}

	new_nail = nail->produce();
	if (new_nail != NULL) {
		std::cout << new_nail->toString() << "\n";
	} else {
		std::cout << "Can't produce nail\n";
	}
	#endif

	#if 0
	for (int j=0; j<10; j++) {
		zomb::ZombieCharacter *z = new zomb::ZombieCharacter();
		std::cout << z->toString() << "\n";
	}
	#endif
	#if 0
	zomb::ZombieCharacter *z1 = new zomb::ZombieCharacter();
	zomb::ZombieCharacter *z2 = new zomb::ZombieCharacter();
	std::cout << "a1\n";
	z1->assignObject(z2);
	std::cout << "a2\n";
	z1->assignObject(nail);
	#endif


	#if 0
	blieng::Wallclock *timer = new blieng::Wallclock(new blieng::Item("time"));
	timer->addProducer(new blieng::Item("ironmine"));
	timer->addProducer(new blieng::Item("worker"));
	timer->addProducer(new blieng::Item("worker"));
	timer->addProducer(new blieng::Item("worker"));
	timer->addProducer(new blieng::Item("worker"));
	timer->addProducer(new blieng::Item("iron"));
	timer->addProducer(new blieng::Item("nail"));
	timer->addProducer(new blieng::Item("wood"));
	timer->addProducer(new blieng::Item("wood"));
	timer->addProducer(new blieng::Item("box"));
	timer->forward();
	timer->forward();
	timer->forward();
	timer->forward();
	timer->forward();
	/*
	*/
	#endif

	#if 0
	ui::LoadingScreen *ss = new ui::LoadingScreen();
	ss->setMinimumSize(800, 600);
	ss->show();
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	return app.exec();
	#endif

	#if 0
	ui::StartScreen *ss = new ui::StartScreen();
	ss->setMinimumSize(800, 600);
	ss->show();
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	return app.exec();
	#endif

	//blieng::Maps *map1 = new blieng::Maps("world1");
	#if 0
	ui::MapScreen *game = new ui::MapScreen("world1");
	game->show();
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	
	return app.exec();
	#endif
	blieng::Configure *conf = blieng::Configure::getInstance();

	conf->addKey("clock", blieng::Configure::KeyUInt);
	conf->addKey("clock_background", blieng::Configure::KeyUInt);

	conf->addKey("walker_interval", blieng::Configure::KeyUInt);

	conf->addKey("map_width_min", blieng::Configure::KeyUInt);
	conf->addKey("map_height_min", blieng::Configure::KeyUInt);

	conf->addKey("act_button_width", blieng::Configure::KeyUInt);
	conf->addKey("act_button_height", blieng::Configure::KeyUInt);

	conf->addKey("chr_width_min", blieng::Configure::KeyUInt);
	conf->addKey("chr_height_min", blieng::Configure::KeyUInt);
	conf->addKey("chr_count", blieng::Configure::KeyUInt);
	conf->addKey("chr_size", blieng::Configure::KeyUInt);
	conf->addKey("chr_map_size", blieng::Configure::KeyUInt);

	//conf->addKey("", blieng::Configure::KeyUInt);
	conf->addKey("default_zoom_level", blieng::Configure::KeyUInt);
	conf->addKey("path_width", blieng::Configure::KeyUInt);
	conf->addKey("townname_max_width", blieng::Configure::KeyUInt);
	conf->addKey("town_sensitivity_distance", blieng::Configure::KeyUInt);
	conf->addKey("mouse_wheel_sensitivity", blieng::Configure::KeyUInt);

	conf->addKey("zombie_size", blieng::Configure::KeyUInt);
	conf->addKey("zombie_steps", blieng::Configure::KeyUInt);
	conf->addKey("fight_safe_area", blieng::Configure::KeyUInt);

	conf->addKey("random_freq", blieng::Configure::KeyUInt);
	conf->addKey("random_prob", blieng::Configure::KeyUInt);

	conf->addKey("bite_prob", blieng::Configure::KeyDouble);
	conf->addKey("bite_threshold", blieng::Configure::KeyDouble);
	conf->addKey("zombie_limiter", blieng::Configure::KeyDouble);

	conf->addKey("debug", blieng::Configure::KeyBool);

	conf->load("zomb.json");
	if (!conf->validate()) {
		qDebug() << "Invalid config";
		return 1;
	}

	ui::GameScreen *game = new ui::GameScreen();
	game->show();
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	
	return app.exec();

	#if 0
	ui::GenerateCharacter *cg = new ui::GenerateCharacter();
	cg->show();
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	
	return app.exec();
	#else
	return 0;
	#endif
}
