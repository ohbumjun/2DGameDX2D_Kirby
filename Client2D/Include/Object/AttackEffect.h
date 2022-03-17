#pragma once
#include "GameObject\GameObject.h"
class CAttackEffect :
    public CGameObject
{
protected:
	CAttackEffect();
	CAttackEffect(const CAttackEffect& obj);
	virtual ~CAttackEffect();
protected :
	Vector3 m_PrevPos;
	bool m_SideCollisionApplied;
protected:
	virtual bool CheckSideCollision();
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};

