#pragma once
#include "Component/SpriteComponent.h"
#include "GameObject/LifeObject.h"
#include "Component/WidgetComponent.h"
#include "Component/NavAgent.h"
#include "../Component/FightKirbyState.h"
#include "../Component/KirbyState.h"
#include "../Component/BeamKirbyState.h"
#include "../Component/NormalKirbyState.h"

class CPlayer2D :
	public CLifeObject
{
	friend class CScene;

protected:
	CPlayer2D();
	CPlayer2D(const CPlayer2D& obj);
	virtual ~CPlayer2D() override;

private:
	CSharedPtr<CKirbyState> m_KirbyState;

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
private :
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

	bool m_JumpDown;
	float m_JumpDownDist;

	bool m_IsFlying;
	float m_FlySpeed;

	bool m_IsEatingMonster;

	bool m_IsSpecialStateChanged;
	bool m_IsPulling;
	float m_PullDistance;
	class CMonster* m_PullingMonster;
	class CMonster* m_EatenMonster;

	bool m_Bounced;

	Vector3 m_InitPlayerPos;

	bool m_MoveDashEffectMade;

public :
	Vector3 GetInitWorldPos() const
{
		return m_InitPlayerPos;
}
	CColliderBox2D* GetBodyCollider () const
{
		return m_Body;
}
public:
	virtual bool       Init() override;
	virtual void		Start() override;
	virtual void       Update(float DeltaTime) override;
	virtual void       PostUpdate(float DeltaTime) override;
	virtual CPlayer2D* Clone() override;
	virtual void UpdateWhileOffGround(float DeltaTime) override;

private:
	// Move 
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
	void ResetCameraInfoToPlayer();
	virtual void CheckBelowWorldResolution() override;
private :
	// Rotation
	void RotationZInv(float DeltaTime);
	void RotationZ(float DeltaTime);
private :
	// Fly & Jump
	void FlyAfterJump(float DeltaTime);
	void SimpleJump();
	void Jump(float DeltaTime);
	void TriangleJumpLeft(float DeltaTime);
	void TriangleJumpRight(float DeltaTime);
	void JumpDown(float DeltaTime);
	void JumpDownDistUpdate(float DeltaTime);
private :
	// Fall
	void FallFromCliff();
	void UpdateActionWhenReachGroundAfterFall();
	virtual void SetObjectLand() override;
	void FallDownAttack(const CollisionResult& Result);
	virtual void CheckBottomCollision() override;
protected:
	// Animation
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
	void ChangePlayerSpitOutAnimation();
private :
	// Spit Out & Pull
	void SpitOut(float DeltaTime);
	void PullRight(float DeltaTime);
	void PullRightEnd(float DeltaTime);
	void PullLeft(float DeltaTime);
	void PullLeftEnd(float DeltaTime);
	void PullRightCollisionBeginCallback(const CollisionResult& Result);
	void PullRightCollisionEndCallback(const CollisionResult& Result);
	void PullLeftCollisionBeginCallback(const CollisionResult& Result);
	void PullLeftCollisionEndCallback(const CollisionResult& Result);
	void ChangeToAfterWardsAnimationAfterSpitOut();
private :
	// Special Change
	void SpecialChange(float DeltaTime);
	void SetBasicSettingToChangedState();
public :
	void SetIsEatingMonster(bool Enable);
	void SetEatenMonster(class CMonster* Monster);
private :
	// Special Action
	void Attack();
	void FallDownAttack();
private :
	void MovePointDown(float DeltaTime);
};



