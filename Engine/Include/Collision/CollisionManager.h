#pragma once

#include "../GameInfo.h"

class CCollisionManager
{
private :
	std::unordered_map<std::string, Collision_Profile*> m_mapCollisionProfile;
public :
	bool Init();
	void CreateProfile(const std::string& Name, Collision_Channel Channel, bool CollisionEnable,
		Collision_State State = Collision_State::Ignore);
	void SetCollisionState(const std::string& Name, Collision_Channel Channel,
		Collision_State State);
	Collision_Profile* FindCollisionProfile(const std::string& Name);

	// Single Ton
private :
	CCollisionManager();
	~CCollisionManager();
private :
	static CCollisionManager* m_Inst;
public :
	static CCollisionManager* GetInst()
	{
			if (!m_Inst)
				m_Inst = new CCollisionManager;
			return m_Inst;
	}
	static void DestroyInst()
	{
			SAFE_DELETE(m_Inst);
	}
};

