#include "CollisionManager.h"

CCollisionManager* CCollisionManager::m_Inst = nullptr;

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	auto iter = m_mapCollisionProfile.begin();
	auto iterEnd = m_mapCollisionProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CCollisionManager::Init()
{
	CreateProfile("Object", Collision_Channel::Object, true);
	CreateProfile("Player", Collision_Channel::Player, true);
	CreateProfile("PlayerAttack", Collision_Channel::PlayerAttack, true);
	CreateProfile("Monster", Collision_Channel::Monster, true);
	CreateProfile("MonsterAttack", Collision_Channel::MonsterAttack, true); 
	CreateProfile("PlayerEffect", Collision_Channel::PlayerEffect, true);
	CreateProfile("Block", Collision_Channel::Block, true);

	SetCollisionState("Player", Collision_Channel::Player, Collision_Interaction::Ignore);
	SetCollisionState("Player", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);

	SetCollisionState("PlayerAttack", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::Player, Collision_Interaction::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::PlayerEffect, Collision_Interaction::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::Block, Collision_Interaction::Ignore);

	SetCollisionState("PlayerEffect", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	SetCollisionState("PlayerEffect", Collision_Channel::Monster, Collision_Interaction::Ignore);
	SetCollisionState("PlayerEffect", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);

	SetCollisionState("Monster", Collision_Channel::Monster, Collision_Interaction::Ignore);
	SetCollisionState("Monster", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);
	SetCollisionState("Monster", Collision_Channel::PlayerEffect, Collision_Interaction::Ignore);
	SetCollisionState("Monster", Collision_Channel::Block, Collision_Interaction::Ignore);

	SetCollisionState("MonsterAttack", Collision_Channel::Monster, Collision_Interaction::Ignore);
	SetCollisionState("MonsterAttack", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);
	SetCollisionState("MonsterAttack", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	SetCollisionState("MonsterAttack", Collision_Channel::PlayerEffect, Collision_Interaction::Ignore);

	SetCollisionState("Block", Collision_Channel::PlayerAttack, Collision_Interaction::Ignore);
	SetCollisionState("Block", Collision_Channel::MonsterAttack, Collision_Interaction::Ignore);
	SetCollisionState("Block", Collision_Channel::Monster, Collision_Interaction::Ignore);

	return true;
}

void CCollisionManager::CreateProfile(const std::string& Name, Collision_Channel Channel, bool CollisionEnable,
	Collision_Interaction State)
{
	if (FindCollisionProfile(Name))
		return;

	Collision_Profile * Profile = new Collision_Profile;
	Profile->Name = Name;
	Profile->Channel = Channel;
	Profile->CollisionEnable = CollisionEnable;
	Profile->vecInteraction.resize(Collision_Channel::End);
	for (size_t i = 0; i <  Profile->vecInteraction.size(); i++)
	{
		Profile->vecInteraction[i] = State;
	}

	m_mapCollisionProfile.insert(std::make_pair(Name, Profile));
}

void CCollisionManager::SetCollisionState(const std::string& Name, Collision_Channel Channel, Collision_Interaction State)
{
	Collision_Profile* Profile = FindCollisionProfile(Name);
	if (!Profile)
		return;
	Profile->vecInteraction[(int)Channel] = State;
}

Collision_Profile* CCollisionManager::FindCollisionProfile(const std::string& Name)
{
	auto iter = m_mapCollisionProfile.find(Name);
	if (iter == m_mapCollisionProfile.end())
		return nullptr;
	return iter->second;
}
