#pragma once

#include "GameInfo.h"

struct LoadingMessage {
	bool Complete;
	float Percent;
};

enum class LoadingSceneType {
	Green1,
	Float1,
	Dyna1
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
	Sword,
	End
};

enum class KirbyAttackEffect_Type {
	Fire,
	Beam,
	BeamSpecial,
	Fight,
	FightSpecial,
	Bomb,
	BombSpecial,
	BeamSpark,
	FightFall,
	FireFall,
	BombFall,
	Sword,
	SwordFall
};

struct Monster_Stat {
	std::string m_Name;
	Monster_Type m_Type;
	// std::string m_Type;
	float m_HP;
	float m_AttackDist;
	float m_DashDist;
	float m_MoveVelocity;
	bool m_IsGroundObject;
	float m_AttackAbility;
	float m_Scale;
	float m_JumpVelocity;
	float m_CloseAttackDist;
	float m_FarAttackDist;
	Ability_State m_AbilityState;
	// std::string m_AbilityState;
};