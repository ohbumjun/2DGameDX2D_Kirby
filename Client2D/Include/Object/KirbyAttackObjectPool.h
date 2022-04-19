#pragma once
#include "GameInfo.h"
#include "KirbyAttackEffect.h"

class CKirbyAttackObjectPool : public CGameObject
{
	friend class CFightKirbyState;
	friend class CBeamKirbyState;
private:
	CKirbyAttackObjectPool();
	~CKirbyAttackObjectPool();
private :
	std::vector<CKirbyAttackEffect> m_vecAttackEffects;
	bool m_IsPoolBeingUsed;
	int m_UseIndex;
private :
	void SetPoolBeingUsed(bool Use)
{
		m_IsPoolBeingUsed = Use;
}
	void SetAttackType(KirbyAttackEffect_Type Type);
public :
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
};

