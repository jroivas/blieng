#include "player_character.h"
#include <string>
#include <boost/random/uniform_int_distribution.hpp>

using zomb::PlayerCharacter;

std::vector<std::string> PlayerCharacter::first_names;
std::vector<std::string> PlayerCharacter::last_names;

PlayerCharacter::PlayerCharacter() : blieng::Character()
{
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
	setValue("zombie-wound", (double)0);

	// Stats
	setValue("might", getRandomInt(0, 50));
	setValue("strength", getRandomInt(0, 50));
	setValue("intelligence", getRandomInt(0, 50));

	// Skills
	setValue("rifle", getRandomInt(0, 50));
	setValue("pistol", getRandomInt(0, 50));
	setValue("computer", getRandomInt(0, 50));

	setValue("dead", false);
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
	if (first_names.size() == 0) first_names = blieng::Data::getInstance()->readLinesFromFile("first_names");
	if (last_names.size() == 0) last_names = blieng::Data::getInstance()->readLinesFromFile("last_names");
}

void PlayerCharacter::generateName()
{
	readNames();

	std::string name;
	name = first_names.at(getRandomInt(0, first_names.size()));
	name += " " + last_names.at(getRandomInt(0, last_names.size()));

	setValue("name", name);
}
