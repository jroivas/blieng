#include "zombie_character.h"
#include "character.h"

using zomb::ZombieCharacter;

ZombieCharacter::ZombieCharacter() : zomb::PlayerCharacter()
{
	init();
}

void ZombieCharacter::init()
{
	int health = getRandomInt(30, 65);
	setValue("health", health);
	setValue("health-max", health);

	double radiation = getRandomDouble(0, 10);
	setValue("radiation", radiation);

	setValue("class", std::string("zombie"));

	setValue("dead", false);

	setValue("health-leg-left", 1.0);
	setValue("health-leg-right", 1.0);
	setValue("health-arm-left", 1.0);
	setValue("health-arm-right", 1.0);
	setValue("health-torso", 1.0);
	setValue("health-head", 1.0);

	static const int MAX_IMAGES = 1;
	setValue("image", getRandomInt(0, MAX_IMAGES));
}

void ZombieCharacter::fromPlayerCharacter(PlayerCharacter *chr)
{
	if (chr == NULL) return;

	// Transform a player character to a zombie
	assignObject(chr);
	chr->kill();

	init();
	/*setValue("dead", false);
	setValue("class", std::string("zombie"));
	*/
}
