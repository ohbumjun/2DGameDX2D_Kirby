#pragma once

#include "GameInfo.h"

struct LoadingMessage {
	bool Complete;
	float Percent;
};

enum class Monster_AI {
	Idle,
	Walk,
	Trace,
	Hit,
	Attack,
	Death
};

enum class Ability_State {
	Fight,
	Beam,
	Fire,
	End
};

enum class KirbyNormalAttack_Type {
	Fire,
	Beam,
	Fight,
	FightFall
};