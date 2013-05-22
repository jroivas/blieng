#ifndef __ZOMBIE_CHARACTER_H
#define __ZOMBIE_CHARACTER_H

#include "player_character.h"

namespace zomb {

class ZombieCharacter : public zomb::PlayerCharacter
{
public:
	ZombieCharacter();
	void fromPlayerCharacter(PlayerCharacter *chr);
	void filterItems();

private:
	void init();
};

}
#endif
