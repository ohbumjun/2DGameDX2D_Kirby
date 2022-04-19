#pragma once
#include "GameObject/LifeObject.h"
#include "Component/WidgetComponent.h"
#include "Component/NavAgent.h"
#include "../Component/KirbyState.h"
#include "../Client.h"

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
	// Object Pool
	std::unordered_map<Ability_State, CSharedPtr<class CKirbyAttackObjectPool>> m_mapObjectPool;

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
	bool m_IsSwimSoundOn;

	// 날기 
	bool m_IsFlying;
	float m_FlySpeed;
	bool m_SceneChange;
	float m_FlySoundTime;
	float m_FlySoundTimeMax;

	// 사다리 타기
	bool m_IsLadderGoingUp;

	// 몬스터 먹기
	bool m_IsEatingMonster;
	bool m_IsPulling;
	float m_PullColliderXMaxScale;
	float m_PullColliderXInitScale;
	float m_PullColliderYMaxScale;
	float m_PullColliderYInitScale;
	float m_PullDistance;
	class CMonster* m_PullingMonster;
	class CEffectStar* m_PullingStar;
	CSharedPtr<class CMonster> m_EatenMonster;
	CSharedPtr<class CMonster> m_PrevAbilityEatenMonster;
	float m_PullRandomStarMakeTime;
	float m_PullRandomStarMakeTimeMax;
	float m_PullSoundTime;
	float m_PullSoundTimeMax;

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

	// Ultimate Attack
	bool m_UltimateAttack;
	class CUltimateAttackWidget* m_UltimateAttackWindow;

	// Slide Attack
	bool m_SlideAttack;
	float m_SlideAttackSpeed;
	float m_SlideAttackSpeedMax;
	float m_SlideAttackTime;
	float m_SlideAttackTimeMax;
	float m_SlideAttackEffectToggleTime;
	float m_SlideAttackEffectCurTime;

	// Attack Ready
	bool m_AttackReady;

	// Change
	bool m_IsChanging;
	float m_ChangeTime;
	float m_ChangeTimeMax;
	CSharedPtr<class CEffectSceneChangeAlpha> m_ChangeBlackBackGround;
	std::function<void(const CollisionResult& Result)> m_SceneChangeCallback;
	Ability_State m_SpecialAbilityState;

public :
	CKirbyAttackObjectPool* GetObjectPool(Ability_State State) 
{
		return m_mapObjectPool[State];
}
	bool IsAttackReady() const
{
		return m_AttackReady;
}
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
	void SetAttackReady(bool Ready)
{
		m_AttackReady = Ready;
}
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
public:
	void SetAttackEnable(bool Enable);
public:
	virtual bool       Init() override;
	virtual void		Start() override;
	virtual void       Update(float DeltaTime) override;
	virtual void       PostUpdate(float DeltaTime) override;
	virtual CPlayer2D* Clone() override;
private :
	// Object Pool
	void InitObjectPoolSetting();
	void InitBeamKirbyObjectPool();
private :
	// Resolution
	void CheckWithinBossWorldResolution();
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
	void CheckIsSwimming(float DeltaTime);
	void UpdateSwimAction(float DeltaTime);
private :
	// MP
	void UpdateMP(float DeltaTime);
	void DecreaseMP(float MP);
public :
	// HP
	void AddHP(float HP);
 public:
	// Hit
	void Damage(float Damage);
private :
	void UpdateBeingHit(float DeltaTime);
private :
	void CheckPlayerWaterCollision();
private :
	// Attack Time
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
	void ChangePlayerUltimateAttackAnimation(float DeltaTime);
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
	void LoadChangeImagesInAdvance();
public :
	void SetIsEatingMonster(bool Enable);
	void SetEatenMonster(class CMonster* Monster);
private :
	// Special Action
	void Attack(float DeltaTime);
	void SpecialAttack();
	void UltimateAttack();
	void PrepareSpecialAction(float PrepareTime);
	void UndoSpecialAction();
	void PrepareUltimateAction(float BackGroundDestroyTime, float UIProceedTime);
	void UndoUltimateAction();
	void ChangeToIdleAfterAnimation(float DeltaTime);
private:
	void SlideAttack(float DeltaTime);
	void MakePlayerCloneEffect();
private :
	// UI Setting
	void SetUIAccordingToKirbyState();
	// Kirby State Size
	void SetKirbyStateSizeAccordingToKirbyState();
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



