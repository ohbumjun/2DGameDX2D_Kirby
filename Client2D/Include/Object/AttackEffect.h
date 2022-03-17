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
	bool m_BottomCollisionApplied;
public :
	void SetBottomCollisionEnable(bool Enable)
{
		m_BottomCollisionApplied = Enable;
}
protected:
	virtual bool CheckSideCollision();
	virtual bool CheckBottomCollision();
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};

