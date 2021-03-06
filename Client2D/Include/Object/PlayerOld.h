#pragma once

#include "Component/SpriteComponent.h"
#include "GameObject/LifeObject.h"
#include "Component/WidgetComponent.h"
#include "Component/NavAgent.h"

class CPlayerOld :
    public CLifeObject
{
	friend class CScene;

protected:
	CPlayerOld();
	CPlayerOld(const CPlayerOld& obj);
	virtual ~CPlayerOld() override;

private:
	CSharedPtr<CSpriteComponent> m_KirbyState;
	CSharedPtr<CSpriteComponent> m_ChildLeftSprite;
	CSharedPtr<CSpriteComponent> m_ChildRightSprite;
	CSharedPtr<CSceneComponent>  m_ChildLeftMuzzle;
	CSharedPtr<CSceneComponent>  m_ChildRightMuzzle;
	CSharedPtr<CSceneComponent>  m_ChildRoot;
	CSharedPtr<CSceneComponent>  m_Muzzle;

	CSharedPtr<CSpriteComponent> m_Child1Sprite;
	CSharedPtr<CSpriteComponent> m_Child2Sprite;
	CSharedPtr<CSpriteComponent> m_Child3Sprite;
	CSharedPtr<CSpriteComponent> m_Child4Sprite;

	CSharedPtr<class CColliderBox2D> m_Body;
	CSharedPtr<class CColliderBox2D> m_PullRightCollider;
	CSharedPtr<class CColliderBox2D> m_PullLeftCollider;

	CSharedPtr<class CCameraComponent> m_Camera;

	CSharedPtr<CWidgetComponent> m_SimpleHUDWidget;

	CSharedPtr<CNavAgent> m_NavAgent;

	float m_DeltaTime;

	bool  m_SolW;
	float m_WDistance;
	float m_Opacity;
private:
	// 전체 움직임 속도 ( 레버 + 대쉬 )
	float m_MoveVelocity;
	float m_MoveVelocityMax;

	bool m_IsLeverMoving;
	float m_LeverMoveAccel;
	float m_LeverVelocity;
	float m_LeverMaxMoveVelocity;

	bool m_IsDashMoving;
	float m_DashMoveAccel;
	float m_DashVelocity;
	float m_DashMaxMoveVelocity;

	bool m_RightMove;
	float m_ToLeftWhenRightMove;
	bool m_RightMovePush;

	bool m_LeftMove;
	bool m_ToRightWhenLeftMove;
	bool m_LeftMovePush;

	bool m_TriangleJump;
	float m_TriangleJumpVelocityRatio;

	bool m_IsFlying;
	float m_FlySpeed;

	bool m_IsEatingMonster;

	bool m_IsPulling;
	float m_PullDistance;
	class CMonster* m_PullingMonster;
	class CMonster* m_EatenMonster;

public:
	CColliderBox2D* GetBodyCollider() const
	{
		return m_Body;
	}
public:
	void SetIsEatingMonster(bool Enable);
	void SetEatenMonster(class CMonster* Monster);
public:
	virtual bool       Init() override;
	virtual void		Start() override;
	virtual void       Update(float DeltaTime) override;
	virtual void       PostUpdate(float DeltaTime) override;
	virtual CPlayerOld* Clone() override;
	virtual void UpdateWhileOffGround(float DeltaTime) override;

private: // Move 
	void MoveUp(float DeltaTime);
	void MoveUpEnd(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveDashLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	void MoveDashRight(float DeltaTime);
	void LeftLeverMoveEnd(float DeltaTime);
	void RightLeverMoveEnd(float DeltaTime);
	void LeftDashMoveEnd(float DeltaTime);
	void RightDashMoveEnd(float DeltaTime);
	float CalculateLeverMoveSpeed(float DeltaTime);
	float CalculateDashMoveSpeed(float DeltaTime);
	float CalculateTotalMoveSpeed(float DeltaTime);
	void PlayerMoveUpdate(float DeltaTime);
	void ResetMoveInfo();
private: // Rotation
	void RotationZInv(float DeltaTime);
	void RotationZ(float DeltaTime);
private: // Fly & Jump
	void FlyAfterJump(float DeltaTime);
	void SimpleJump();
	void Jump(float DeltaTime);
	void TriangleJumpLeft(float DeltaTime);
	void TriangleJumpRight(float DeltaTime);
private: // Fall
	void FallFromCliff();
	void ChangeToIdleWhenReachGroundAfterFall();
	virtual void SetObjectLand() override;
	void FallDownAttack(const CollisionResult& Result);
protected: // Animation
	void ChangeAnimation(const std::string& AnimName);
	void ChangePlayerIdleAnimation();
	void ChangePlayerNormalIdleAnimation();
	void ChangePlayerEatIdleAnimation();
	void ChangePlayerWalkAnimation();
	void ChangePlayerNormalWalkAnimation();
	void ChangePlayerEatWalkAnimation();
	void ChangePlayerHitAnimation();
	void ChangePlayerRunAnimation();
	void ChangePlayerNormalRunAnimation();
	void ChangePlayerEatRunAnimation();
	void ChangePlayerDeathAnimation();
	void ChangePlayerAttackAnimation();
	void ChangePlayerFlyAnimation();
	void ChangePlayerJumpAnimation();
	void ChangePlayerFallAnimation();
private: // Spit Out & Pull
	void SpitOut(float DeltaTime);
	void PullRight(float DeltaTime);
	void PullRightEnd(float DeltaTime);
	void PullLeft(float DeltaTime);
	void PullLeftEnd(float DeltaTime);
	void PullRightCollisionBeginCallback(const CollisionResult& Result);
	void PullRightCollisionEndCallback(const CollisionResult& Result);
	void PullLeftCollisionBeginCallback(const CollisionResult& Result);
	void PullLeftCollisionEndCallback(const CollisionResult& Result);
private: // Special Change
	void SpecialChange(float DeltaTime);
private: // Special Action
	void Attack();
	void FallDownAttack();
private:
	void Attack(float DeltaTime);
	void Attack1(float DeltaTime);
	void Skill1(float DeltaTime);
	void MovePointDown(float DeltaTime);
};

