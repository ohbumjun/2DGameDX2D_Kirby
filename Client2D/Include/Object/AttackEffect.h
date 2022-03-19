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
public :
	void SetJumpVelocity(float Velocity)
{
		m_JumpVelocity = Velocity;
}
	void SetPhysicsSimulate(bool Enable)
{
		m_PhysicsSimulate = Enable;
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
protected:
	virtual bool CheckSideCollision();
	virtual bool CheckBottomCollision();
	void UpdateGravityEffect(float DeltaTime);
protected:
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};

