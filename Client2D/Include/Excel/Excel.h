#pragma once
#include "GameInfo.h"
#include "../Client.h"

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

class CExcel
{
private :
	CExcel();
	~CExcel();
public :
	void LoadExcel();
	void SaveExcel();
	bool Init();
    Monster_Stat* FindMonsterState(const std::wstring& MonsterName);
private :
    std::unordered_map<std::wstring, Monster_Stat*> m_MapMonsterStats;

private :
	static CExcel* m_Inst;
public :
	static CExcel* GetInst()
{
		if (!m_Inst)
			m_Inst = new CExcel;
		return m_Inst;
}
	static void DestroyInst()
{
		SAFE_DELETE(m_Inst);
}
};


