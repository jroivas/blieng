#include "zombie_character.h"

using zomb::ZombieCharacter;

ZombieCharacter::ZombieCharacter() : PlayerCharacter()
{
	int health = getRandomInt(30, 65);
	setValue("health", health);
	setValue("health-max", health);

	double radiation = getRandomDouble(0, 10);
	setValue("radiation", radiation);
}
