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
        unsigned int max_zombies = 0;
        if (town->isValue("zombies")) {
                max_zombies = town->getUIntValue("zombies");
        }
        if (last_zombies==0) zomb_force++;
        unsigned int forced_zombies = 0;
        if (zomb_force>=2) forced_zombies = 1;
        unsigned int zombies = town->getRandomInt(0, max_zombies) + forced_zombies;

        last_zombies = zombies;
        total_population += population;

        unsigned int zombcnt = zombies;
        for (unsigned int index = 0; index < zombcnt; index++) {
                zomb::PlayerCharacter* unit = new zomb::PlayerCharacter();
                zomb::ZombieCharacter* zunit = new zomb::ZombieCharacter();
                zunit->fromPlayerCharacter(unit);
                town->addCharacter(zunit);
                delete unit;
        }
        for (unsigned int index = 0; index < population; index++) {
                zomb::PlayerCharacter* unit = new zomb::PlayerCharacter();
                town->addCharacter(unit);
        }
        town->setValue("population", population);
    }
}
