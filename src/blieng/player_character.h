#ifndef __PLAYER_CHARACTER
#define __PLAYER_CHARACTER

#include <string>
#include <vector>
#include "character.h"
#include "data.h"

namespace blieng {

class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	void roll();

private:
	// Generators
	void generateName();
	void generateHealth();

	void readNames();

	static std::vector<std::string> first_names;
	static std::vector<std::string> last_names;
};

}

#endif
