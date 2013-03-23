#include "player_character.h"
#include <string>
#include <boost/random/uniform_int_distribution.hpp>

PlayerCharacter::PlayerCharacter()
{
	gen = new boost::random::random_device();
	roll();
}

void PlayerCharacter::roll()
{
	generateName();

	// Character level
	setValue("level", 0);
	setValue("level-max", 10);

	generateHealth();

	// Armor
	setValue("armor", 0);
	setValue("armor-max", 40);

	// Food
	setValue("food", 0);
	setValue("food-max", 350);

	// Water
	setValue("water", 0);
	setValue("water-max", 5);

	// Zombie prob
	setValue("zombie-prob", getRandomInt(0, 100));
}


void PlayerCharacter::generateHealth()
{
	// Health
	int health = getRandomInt(65, 100);
	setValue("health", health);
	setValue("health-max", health);
}

void PlayerCharacter::readNames()
{
	first_names = Data::getInstance()->readLinesFromFile("first_names");
	last_names = Data::getInstance()->readLinesFromFile("last_names");
}

int PlayerCharacter::getRandomInt(int limit_low, int limit_max)
{
	boost::random::uniform_int_distribution<> dist(limit_low, limit_max);
	return dist(*gen);
}

void PlayerCharacter::generateName()
{
	readNames();

	std::string name;
	name = first_names.at(getRandomInt(0, first_names.size()));
	name += " " + last_names.at(getRandomInt(0, last_names.size()));

	setValue("name", name);
}
