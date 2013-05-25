#ifndef __ZOMB_PLAYER_CHARACTER
#define __ZOMB_PLAYER_CHARACTER

#include <string>
#include <vector>
#include "blieng/character.h"
#include "blieng/data.h"

namespace zomb {

class PlayerCharacter : public blieng::Character
{
public:
	PlayerCharacter();
	virtual ~PlayerCharacter();
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
