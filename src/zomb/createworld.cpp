#include "createworld.h"
#include "zomb/player_character.h"
#include "zomb/zombie_character.h"
#include <boost/foreach.hpp>
#include <QDebug>

using zomb::CreateWorld;

CreateWorld::CreateWorld(blieng::Maps *maps) : maps(maps)
{
	initialize();
}

void CreateWorld::initialize()
{
	/* Initialize the world */
	unsigned int total_population = 0;
	unsigned int total_zombies = 0;
	unsigned int last_zombies = 0;
	unsigned int zomb_force = 0;
	BOOST_FOREACH(blieng::Town *town, maps->getTowns()) {
		double population = town->getDoubleValue("population-index");
		unsigned int min_pop = (population-1)*2;
		unsigned int max_pop = (population+1)*2;

		population = town->getRandomInt(min_pop, max_pop);
		unsigned int max_zombies = population/10;
		if (last_zombies==0) zomb_force++;
		unsigned int forced_zombies = 0;
		if (zomb_force>=2) forced_zombies = 1;
		unsigned int zombies = town->getRandomInt(0, max_zombies) + forced_zombies;
		if (town->isValue("start") && town->getBoolValue("start")) {
			zombies = 0;
		}
		last_zombies = zombies;
		total_population += population;
		unsigned int zombcnt = zombies;
		for (unsigned int index = 0; index < population; index++) {
			zomb::PlayerCharacter* unit = new zomb::PlayerCharacter();
			if (zombcnt>0) {
				zomb::ZombieCharacter* zunit = new zomb::ZombieCharacter();
				zunit->fromPlayerCharacter(unit);
				--zombcnt;
				town->addCharacter(zunit);
			} else {
				town->addCharacter(unit);
			}
		}
		town->setValue("population", population);
		//town->setValue("population", population);
		//qDebug() << town->getName().c_str() << " population: " << population << " zombies: " << zombies;
	}

	#if 0
	qDebug() << "total population: " << total_population;
	unsigned int max_zombies = total_population/100;
	if (max_zombies < 5) max_zombies = 5;
	unsigned int zombies = maps->getRandomInt(2, max_zombies);
	qDebug() << "initial zombies: " << zombies;

	// Convert to zombies
	BOOST_FOREACH(blieng::Town *town, maps->getTowns()) {
		if (zombies == 0) break;
		if (town->isValue("start") && !town->getBoolValue("start")) {
			town->getCharacters()
		}
	}
	#endif
}
