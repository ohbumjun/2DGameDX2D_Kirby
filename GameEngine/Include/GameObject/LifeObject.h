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
	bool m_IsSideCollided;
	bool m_IsBottomCollided;
	bool m_IsCeilingCollided;
	bool m_ApplyBottomCollision;
	float m_PhysicApplyDelayTime;
protected:
	bool m_Jump;
	float m_JumpVelocity;
	float m_JumpAccel;
	float m_JumpAccelAccTime;
	float m_GroundOffSet;
	float m_CollisionOffset;
	bool m_IsFalling;
protected :
	bool m_CollisionDisabled;
	bool m_IsGroundObject;
protected:
	bool m_LineCollided;
protected :
	Vector3 m_ObjectMoveDir;
protected :
	class CTileEmpty* m_BottomCollidedTile;
protected :
	virtual void UpdateWhileOffGround(float DeltaTime);
	void CheckOutsideWorldResolution();
	virtual void CheckBelowWorldResolution();
	bool UpdateYPosIfLineCollided(float x);
public :
	virtual void CheckLineCollision(float DeltaTime);
	virtual void CheckCeilingCollision(float DeltaTime);
	virtual void CheckBottomCollision();
	virtual void CheckSideCollision();
public :
	virtual void SetObjectLand();
public :
	bool IsSideCollided() const
{
		return m_IsSideCollided;
}
	bool IsBottomCollided() const
	{
		return m_IsBottomCollided;
	}
	bool IsGround () const
{
		return m_IsGround;
}
	bool IsPhysicsSimulate() const
	{
		return m_PhysicsSimulate;
	}
public :
	void SetCollisionDisable(bool Enable)
{
		m_CollisionDisabled = Enable;
}
	void SetIsGround (bool Enable)
{
		m_IsGround = Enable;
}
	void SetPhysicsSimulate(bool Physics)
	{
		m_PhysicsSimulate = Physics;
	}
	void SetObjectMoveDir (Vector3 Dir)
{
		m_ObjectMoveDir = Dir;
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

