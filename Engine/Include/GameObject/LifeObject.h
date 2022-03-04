#pragma once
#include "GameObject.h"
class CLifeObject :
    public CGameObject
{
protected:
	CLifeObject();
	CLifeObject(const CLifeObject& obj);
	virtual ~CLifeObject() override = 0;
protected:
	Vector3		m_PrevPos;
	float m_MoveSpeed;
	float m_FallVelocityMax;
	float m_FallVelocity;
	bool m_PhysicsSimulate;
	float m_GravityAccel;
	bool m_IsGround;
	float m_FallTime;
	float m_FallStartY;
	float m_FloorCheck;
	float m_SideWallCheck;
	float m_PhysicApplyDelayTime;
protected:
	bool m_Jump;
	float m_JumpVelocity;
	float m_JumpAccel;
	float m_JumpAccelAccTime;
private :
	void UpdateWhileOffGround(float DeltaTime);
	void CheckBottomCollision();
	void CheckCeilingCollision();
	void CheckSideCollision();
public :
	bool IsGround () const
{
		return m_IsGround;
}
public :
	void SetIsGround (bool Enable)
{
		m_IsGround = Enable;
}
	void SetPhysicsSimulate(bool Physics)
	{
		m_PhysicsSimulate = Physics;
	}
public:
	virtual void         Start() override;
	virtual bool         Init() override;
	virtual void         Update(float DeltaTime) override;
	virtual void         PostUpdate(float DeltaTime) override;
	virtual void         PrevRender() override;
	virtual void         Render() override;
	virtual void         PostRender() override;
};

