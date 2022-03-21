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
	float m_JumpVelocity;
	bool m_Jump;
	bool m_PhysicsSimulate;
	float m_FallTime;
	float m_FallStartY;
	float m_EffectMoveSpeed;
	Vector2 m_AttackDir;
	class CLifeObject* m_MonsterOwner;
	// class CKirbyState* m_KirbyOwner;
	CSharedPtr<class CSpriteComponent> m_MainSprite;
	CSharedPtr<class CColliderCircle> m_Collider;
public :
	CColliderCircle* GetColliderBody() const
{
		return m_Collider;
}
public :
	void SetEffectMoveSpeed(float Speed)
{
		m_EffectMoveSpeed = Speed;
}
	void SetMonsterOwner(class CLifeObject* Owner)
{
		m_MonsterOwner = Owner;
}
	void SetJumpVelocity(float Velocity)
{
		m_JumpVelocity = Velocity;
}
	void SetPhysicsSimulate(bool Enable)
{
		m_PhysicsSimulate = Enable;
		m_FallStartY = GetWorldPos().y;
}
	void JumpStart()
{
		m_Jump = true;
		m_FallTime = 0.f;
		m_FallStartY = GetWorldPos().y;
}
	void SetBottomCollisionEnable(bool Enable)
{
		m_BottomCollisionApplied = Enable;
}
protected :
	virtual void BottomCollisionSpecificAction();
	virtual void SideCollisionSpecificAction();
	virtual void SetRightAttackDir(float YDir = 0.f);
	virtual void SetLeftAttackDir(float YDir = 0.f);
	void SetAttackDirX(float XDir);
protected:
	virtual bool CheckSideCollision();
	virtual bool CheckBottomCollision();
	void UpdateGravityEffect(float DeltaTime);
public :
	void MonsterAttackCollisionCallback(const CollisionResult& Result);
	// void PlayerAttackCollisionCallback(const CollisionResult& Result);
protected:
	virtual void Start() override;
	virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};

