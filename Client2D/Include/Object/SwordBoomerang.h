#pragma once
#include "GameObject\GameObject.h"

class CSwordBoomerang :
    public CGameObject
{
	friend class CScene;
	friend class CSwordKirbyState;
protected:
	CSwordBoomerang();
	virtual ~CSwordBoomerang();
protected:
	float m_ThrowDirDist;
	float m_ThrowDirDistMax;
	bool m_Return;
	float m_SpeedAccel;
	float m_InitSpeed;
	float m_CurrentSpeed;
	float m_AttackDamage;
	CSharedPtr<class CSpriteComponent> m_MainSprite;
	CSharedPtr<class CColliderCircle> m_Collider;
	float m_AttackDir;
protected:
	void SetAttackDamage(float Damage);
	void SetAttackDir(float DirX);
protected:
	void UpdateBoomerangMovement(float DeltaTime);
	void CollisionCallback(const CollisionResult& Result);
protected:
	virtual void Start() override;
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
};

