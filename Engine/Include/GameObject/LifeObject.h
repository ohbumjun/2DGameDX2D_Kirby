#pragma once
#include "GameObject.h"
class CLifeObject :
    public CGameObject
{
protected:
	CLifeObject();
	CLifeObject(const CLifeObject& obj);
	virtual ~CLifeObject() override;
protected:
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
protected:
	bool m_Jump;
	float m_JumpVelocity;
	float m_JumpAccel;
	float m_JumpAccelAccTime;
public:
	virtual void         Start() override;
	virtual bool         Init() override;
	virtual void         Update(float DeltaTime) override;
	virtual void         PostUpdate(float DeltaTime) override;
	virtual void         PrevRender() override;
	virtual void         Render() override;
	virtual void         PostRender() override;
	virtual CLifeObject* Clone() override;
};

