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
	Bomb, 
	End
};

enum class KirbyAttackEffect_Type {
	Fire,
	Beam,
	Fight,
	Bomb,
	BeamSpark,
	FightFall,
	FireFall,
	BombFall
};

