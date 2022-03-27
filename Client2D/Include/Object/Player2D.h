#pragma once
#include "GameObject/LifeObject.h"
#include "Component/WidgetComponent.h"
#include "Component/NavAgent.h"
#include "../Component/FightKirbyState.h"
#include "../Component/KirbyState.h"
#include "../Component/BeamKirbyState.h"
#include "../Component/FireKirbyState.h"
#include "../Component/BombKirbyState.h"
#include "../Component/SwordKirbyState.h"
#include "../Component/NormalKirbyState.h"

class CPlayer2D :
	public CLifeObject
{
	friend class CScene;
	friend class CFireKirbyState;
	friend class CBeamKirbyState;
	friend class CFightKirbyState;
	friend class CBombKirbyState;
	friend class CSwordKirbyState;

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

	// 일반 움직임
	bool m_IsLeverMoving;
	float m_LeverMoveAccel;
	float m_LeverVelocity;
	float m_LeverMaxMoveVelocity;

	// 대쉬 움직임
	bool m_IsDashMoving;
	float m_DashMoveAccel;
	float m_DashVelocity;
	float m_DashMaxMoveVelocity;
	bool m_MoveDashEffectMade;
	float m_MoveDashEffectLimitTime;
	float m_MoveDashEffectLimitTimeMax;

	// 오른쪽 움직임
	bool m_RightMove;
	float m_ToLeftWhenRightMove;
	bool m_RightMovePush;

	// 왼쪽 움직임
	bool m_LeftMove;
	bool m_ToRightWhenLeftMove;
	bool m_LeftMovePush;

	// 삼각 점프
	bool m_TriangleJump;
	float m_TriangleJumpVelocityRatio;

	// 약한 점프
	bool m_IsWeakJump;
	float m_InitJumpVelocity;

	// MP
	float m_MP;
	float m_MPMax;

	// 하단 점프
	bool m_JumpDown;
	float m_JumpDownDist;

	// 수영
	bool m_IsSwimming;

	// 날기 
	bool m_IsFlying;
	float m_FlySpeed;
	bool m_SceneChange;

	// 사다리 타기
	bool m_IsLadderGoingUp;

	// 몬스터 먹기
	bool m_IsEatingMonster;
	bool m_IsPulling;
	float m_PullDistance;
	class CMonster* m_PullingMonster;
	class CEffectStar* m_PullingStar;
	CSharedPtr<class CMonster> m_EatenMonster;
	CSharedPtr<class CMonster> m_PrevAbilityEatenMonster;

	// Change 여부
	bool m_IsSpecialStateChanged;

	// 높은 곳에서 떨어지고 한번 더 바운스
	bool m_Bounced;

	// 초기 Player 위치 
	Vector3 m_InitPlayerPos;

	// Scene Change 이후, 해당 Door 로 다시 돌아올 것인지
	bool m_IsBackToSceneChangeDoorPos;

	// HIt
	bool m_IsBeingHit;
	float m_BeingHitTime;
	float m_BeingHitTimeMax;

	// Attack
	float m_AttackTimeLimit;
	float m_AttackTime;

	// Slide Attack
	bool m_SlideAttack;
	float m_SlideAttackTime;
	float m_SlideAttackTimeMax;

	// Change
	bool m_IsChanging;
	float m_ChangeTime;
	float m_ChangeTimeMax;
	CSharedPtr<class CEffectSceneChangeAlpha> m_ChangeBlackBackGround;
	std::function<void(const CollisionResult& Result)> m_SceneChangeCallback;
	

public :
	float GetMP() const
{
		return m_MP;
}
	float GetMPMax() const
{
		return m_MPMax;
}
   	bool IsBeingHit() const
{
		return m_IsBeingHit;
}
	bool IsSceneChanging() const
{
		return m_SceneChange;
}
	bool IsSlideAttacking() const
{
		return m_SlideAttack;
}
	bool IsSwimming() const
{
		return m_IsSwimming;
}
	bool IsAttacking() const
{
		return m_IsAttacking;
}
	Vector3 GetInitWorldPos() const
{
		return m_InitPlayerPos;
}
	CColliderBox2D* GetBodyCollider () const
{
		return m_Body;
}
	bool IsBackToSceneChangeDoorPos() const
{
		return m_IsBackToSceneChangeDoorPos;
}
public :
	void SetLadderState(bool Enable)
{
		m_IsLadderGoingUp = Enable;

		if (Enable)
		{
			ChangeAnimation("LadderGoUp");
			m_PhysicsSimulate = false;
		}
		else
		{
			ChangeAnimation("RightIdle");
			m_PhysicsSimulate = true;
			m_FallTime = 0.f;
			m_FallStartY = GetWorldPos().y;
		}
}
	void SetIsBackToSceneChangeDoorPos(bool Enable)
{
		m_IsBackToSceneChangeDoorPos = Enable;
}
public :
	void SetBeingHitDirection(float Dir);
	void SetIsBeingHit();
	void SetCameraFollowMaxTime(float Time);
	void SetCameraFollowBossMonster(class CGameObject* Boss);
private:
	void SetAttackEnable(bool Enable);
public:
	virtual bool       Init() override;
	virtual void		Start() override;
	virtual void       Update(float DeltaTime) override;
	virtual void       PostUpdate(float DeltaTime) override;
	virtual CPlayer2D* Clone() override;
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
	void StopPlayer();
private :
	// Swim
	void CheckIsSwimming();
	void UpdateSwimMoveDown(float DeltaTime);
private :
	// MP
	void UpdateMP(float DeltaTime);
public :
	// Hit
	void Damage(float Damage);
private :
	void UpdateBeingHit(float DeltaTime);
private :
	void CheckPlayerWaterCollision();
private :
	void UpdateAttackTime(float DeltaTime);
	void UpdateSlideAttackTime(float DeltaTime);
private :
	// Rotation
	void RotationZInv(float DeltaTime);
	void RotationZ(float DeltaTime);
private :
	// Fly & Jump
	void FlyAfterJump(float DeltaTime);
	void SimpleJump();
	void WeakJump();
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
	void FallDownAttackCallback(const CollisionResult& Result);
	virtual void CheckBottomCollision() override;
private:
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
	void ChangePlayerFallDownAttackAnimation();
	void ChangePlayerGoUpAttackAnimation();
	void ChangePlayerFlyAnimation();
	void ChangePlayerJumpAnimation();
	void ChangePlayerFallAnimation();
	void ChangePlayerSpitOutAnimation();
	void ChangePlayerSlideAttackAnimation();
	void ChangePlayerChangeAnimation();
	void ChangePlayerSwimAnimation();
	void ChangePlayerSpecialAttackAnimation(float DeltaTime);
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
public :
	void SetEatenMonsterAsPrevEatenMonster();
private :
	// Special Change
	void SpecialChangeStart(float DeltaTime);
	void SpecialChange();
	void SetBasicSettingToChangedState();
	void SpecialChangeEffect();
	void UpdateChangeState(float DeltaTime);
public :
	void SetIsEatingMonster(bool Enable);
	void SetEatenMonster(class CMonster* Monster);
private :
	// Special Action
	void Attack(float DeltaTime);
	void SlideAttack(float DeltaTime);
	void SpecialAttack();
	void PrepareSpecialAction(float PrepareTime);
	void UndoSpecialAction();
private :
	void PlayerAttackCollisionCallback(const CollisionResult& Result);
private :
	void MovePointDown(float DeltaTime);
public :
	void ResetPlayerCallback()
{
		m_SceneChangeCallback = nullptr;
}
	template<typename T>
	void SetSceneChangeCallback(T* Obj, void(T::*Func)(const CollisionResult&))
{
		m_SceneChangeCallback = std::bind(Func, Obj, std::placeholders::_1);
}
	void Skill1(float DeltaTime);
};



