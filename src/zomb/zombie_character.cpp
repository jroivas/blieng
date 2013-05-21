#include "zombie_character.h"
#include "character.h"

using zomb::ZombieCharacter;

ZombieCharacter::ZombieCharacter() : zomb::PlayerCharacter()
{
	int health = getRandomInt(30, 65);
	setValue("health", health);
	setValue("health-max", health);

	double radiation = getRandomDouble(0, 10);
	setValue("radiation", radiation);

	setValue("class", std::string("zombie"));
}

void ZombieCharacter::fromPlayerCharacter(PlayerCharacter *chr)
{
	if (chr == NULL) return;

	// Transform a player character to a zombie
	assignObject(chr);
	chr->kill();
	setValue("dead", false);
	setValue("class", std::string("zombie"));

	static const int MAX_IMAGES = 1;
	setValue("image", getRandomInt(0, MAX_IMAGES));
}
