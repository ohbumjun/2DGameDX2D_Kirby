#include "CCollisionManager.h"

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
	CreateProfile("Default", Collision_Channel::Default, true);
	CreateProfile("Player", Collision_Channel::Default, true);
	CreateProfile("PlayerAttack", Collision_Channel::Default, true);
	CreateProfile("Monster", Collision_Channel::Default, true);
	CreateProfile("MonsterAttack", Collision_Channel::Default, true); //

	SetCollisionState("Player", Collision_Channel::Player, Collision_State::Ignore);
	SetCollisionState("Player", Collision_Channel::PlayerAttack, Collision_State::Ignore);

	SetCollisionState("PlayerAttack", Collision_Channel::PlayerAttack, Collision_State::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::Player, Collision_State::Ignore);
	SetCollisionState("PlayerAttack", Collision_Channel::MonsterAttack, Collision_State::Ignore);

	SetCollisionState("Monster", Collision_Channel::Monster, Collision_State::Ignore);
	SetCollisionState("Monster", Collision_Channel::MonsterAttack, Collision_State::Ignore);

	SetCollisionState("MonsterAttack", Collision_Channel::Monster, Collision_State::Ignore);
	SetCollisionState("MonsterAttack", Collision_Channel::MonsterAttack, Collision_State::Ignore);
	SetCollisionState("MonsterAttack", Collision_Channel::PlayerAttack, Collision_State::Ignore);

	return true;
}

void CCollisionManager::CreateProfile(const std::string& Name, Collision_Channel Channel, bool CollisionEnable,
	Collision_State State)
{
	if (FindCollisionProfile(Name))
		return;

	Collision_Profile * Profile = new Collision_Profile;
	Profile->Name = Name;
	Profile->Channel = Channel;
	Profile->CollisionEnable = CollisionEnable;
	Profile->vecState.resize(Collision_Channel::End);
	for (size_t i = 0; Profile->vecState.size(); i++)
	{
		Profile->vecState[i] = State;
	}

	m_mapCollisionProfile.insert(std::make_pair(Name, Profile));
}

void CCollisionManager::SetCollisionState(const std::string& Name, Collision_Channel Channel, Collision_State State)
{
	Collision_Profile* Profile = FindCollisionProfile(Name);
	if (!Profile)
		return;
	Profile->vecState[(int)Channel] = State;
}

Collision_Profile* CCollisionManager::FindCollisionProfile(const std::string& Name)
{
	auto iter = m_mapCollisionProfile.find(Name);
	if (iter == m_mapCollisionProfile.end())
		return nullptr;
	return iter->second;
}
