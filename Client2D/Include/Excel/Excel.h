#pragma once
#include "GameInfo.h"
#include "../Client.h"

class CExcel
{
private :
	CExcel();
	~CExcel();
private :
	const int m_ContentStartRow;
	const int m_ContetnEndCol;

	enum class ColType {
		Name = 0,
		Type = 1,
		HP = 2,
		AttackDist = 3,
		DashDist = 4,
		MoveVelocity = 5,
		IsGroundObject = 6,
		AttackAbility = 7,
		Scale = 8,
		JumpVelocity = 9,
		CloseAttackDist = 10,
		FarAttackDist = 11,
		AbilityState = 12
	};
public :
	void LoadExcel();
	void EditExcel();
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


