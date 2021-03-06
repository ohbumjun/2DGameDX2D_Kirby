// Engine
#include "Input.h"
#include "../ClientManager.h"
// UI
#include "../UI/SimpleHUD.h"
#include "../UI/EmptyObjectHUD.h"
#include "../UI/PlayerHUD.h"
#include "Ui/UIDamageFont.h"
#include "../UI/UltimateAttackWidget.h"
// Scene
#include "Scene/Scene.h"
#include "Scene/ViewPort.h"
#include <Scene/CameraManager.h>
#include <Scene/SceneManager.h>
#include "Scene/NavigationManager.h"
// Component
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/CameraComponent.h"
#include "Component/TileEmptyComponent.h"
// Object
#include "BulletCamera.h"
#include "Bullet.h"
#include "BulletTornaido.h"
#include "Player2D.h"
#include "PlayerClone.h"
#include "AbilityMonster.h"
#include "EffectDash.h"
#include "EffectSpitOut.h"
#include "EffectJumpAir.h"
#include "EffectRandomStar.h"
#include "SpecialChangeParticle.h"
#include "EffectStar.h"
#include "BubbleParticle.h"
#include "EffectSceneChangeAlpha.h"
#include "EffectUltimateAttackUI.h"
#include "../Object/Monster.h"
#include "EffectWaterAttack.h"
#include "EffectWaterBlast.h"
#include "KirbyAttackObjectPool.h"
// KirbyState
#include "../Component/FightKirbyState.h"
#include "../Component/BeamKirbyState.h"
#include "../Component/FireKirbyState.h"
#include "../Component/BombKirbyState.h"
#include "../Component/SwordKirbyState.h"
#include "../Component/NormalKirbyState.h"


CPlayer2D::CPlayer2D() :
	m_MoveVelocity(0.f),
	m_LeverMoveAccel(1.6f),
	m_LeverVelocity(0.f),
	m_LeverMaxMoveVelocity(300.f),
	m_SlideAttackTimeMax(0.8f),
	m_DashMoveAccel(2.0f),
	m_DashVelocity(0.f), 
	m_DashMaxMoveVelocity(200.f),
	m_SlideAttackSpeedMax(1000.f),
	m_AttackTimeLimit(4.f),
	m_TriangleJumpVelocityRatio(0.8f),
	m_SlideAttackEffectToggleTime(0.1f),
	m_PullSoundTimeMax(0.3f),
	m_MPMax(100.f),
	m_MP(100.f),
	m_RightMove(false),
	m_AttackReady(false),
	m_FlySoundTimeMax(0.5f),
	m_IsBackToSceneChangeDoorPos(false),
	m_IsSwimSoundOn(false),
	m_UltimateAttack(false),
	m_Bounced(false),
	m_IsSwimming(false),
	m_ToLeftWhenRightMove(false),
	m_RightMovePush(false),
	m_IsWeakJump(false),
	m_SlideAttack(false),
	m_LeftMove(false),
	m_ToRightWhenLeftMove(false),
	m_SpecialAbilityState(Ability_State::End),
	m_SceneChange(false),
	m_IsChanging(false),
	m_IsSpecialStateChanged(false),
	m_LeftMovePush(false),
	m_JumpDown(false),
	m_MoveDashEffectMade(false),
	m_IsLeverMoving(false),
	m_IsDashMoving(false),
	m_TriangleJump(false),
	m_ChangeTimeMax(0.8f),
	m_IsEatingMonster(false),
	m_IsLadderGoingUp(false),
	m_DeltaTime(0.f),
	m_IsFlying(false),
	m_FlySpeed(300.f),
	m_PullDistance(100.f),
	m_MoveDashEffectLimitTimeMax(0.2f),
	m_MoveDashEffectLimitTime(0.2f),
	m_ChangeTime(0.f),
	m_SceneChangeCallback(nullptr),
	m_IsBeingHit(false),
	m_BeingHitTime(0.f),
	m_BeingHitTimeMax(0.5f),
	m_PullRandomStarMakeTimeMax(0.1f)
{
	SetTypeID<CPlayer2D>();
	m_SolW      = false;
	m_WDistance = 0.f;
	m_Opacity   = 1.f;

	m_HP = 1200.f;
	m_HPMax = 1200.f;
	m_AttackAbility = 50.f;

	m_ApplyLimitPosResolution = true;

	m_InitJumpVelocity = m_JumpVelocity;

	m_MoveVelocityMax = m_LeverMaxMoveVelocity + m_DashMaxMoveVelocity;
}

// todo : 이거 제대로 
CPlayer2D::CPlayer2D(const CPlayer2D& obj) :
	CLifeObject(obj)
{
	m_KirbyState           = dynamic_cast<CKirbyState*>(FindComponent("PlayerSprite"));
	m_Body = (CColliderBox2D*)FindComponent("Body");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_SimpleHUDWidget = (CWidgetComponent*)FindComponent("SimpleHUD");
	m_NavAgent = dynamic_cast<CNavAgent*>(FindComponent("NavAgent"));

}

CPlayer2D::~CPlayer2D()
{
}

bool CPlayer2D::Init()
{
	m_KirbyState  = CreateComponent<CNormalKirbyState>("PlayerSprite");
	
	// Collider 
	m_Body = CreateComponent<CColliderBox2D>("Body");
	m_Body->SetCollisionProfile("Player");

	// Widget
	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");
	m_SimpleHUDWidget->CreateUIWindow<CEmptyObjectHUD>("SimpleHUDWidget");

	// Camera
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Camera->OnViewportCenter(); // Player 중심 세팅

	// NavAgent
	m_NavAgent = CreateComponent<CNavAgent>("NavAgent");

	SetRootComponent(m_KirbyState);
	m_KirbyState->AddChild(m_Body);
	m_KirbyState->AddChild(m_Camera);
	m_KirbyState->AddChild(m_SimpleHUDWidget);

	// 반드시 이 위치에서 ( AddChild 이후에 위치 좌표 세팅 )
	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	m_KirbyState->SetTransparency(true);

	// Pivot 값이 없다면, 원래의 pos 인 왼쪽 하단 pos 를 중심으로
	// Center 가 형성되게 될 것이다. 
	m_KirbyState->SetRelativeScale(100.f, 100.f, 1.f);
	m_KirbyState->SetRelativePos(100.f, 50.f, 0.f);
	m_KirbyState->SetPivot(0.5f, 0.5f, 0.f);

	m_Scene->SetPlayerTypeID(GetTypeID());

	return true;
}

void CPlayer2D::Start()
{
	CLifeObject::Start();

	SetPhysicsSimulate(true);

	m_Body = (CColliderBox2D*)FindComponent("Body");
	m_Body->SetCollisionProfile("Player");
	m_Body->SetExtend(GetWorldScale().x * 0.4f, GetWorldScale().y * 0.4f);

	m_Body->EmptyCollisionCallback(Collision_State::Begin);
	m_Body->AddCollisionCallback(Collision_State::Begin, this, &CPlayer2D::FallDownAttackCallback);
	m_Body->AddCollisionCallback(Collision_State::Begin, this, &CPlayer2D::PlayerAttackCollisionCallback);

	// todo : 이게 왜 안되는 거지 ? 원래 가리키고 있는 녀석은 NormalKirbyState 가 맞을 텐데 ?
	// 한편, 기존 Kirby State 정보는 유지하기 위해서, Static Player가 없을 때만
	// 즉, Scene Change가 일어나지 않을 때만 Kirby State 를 다시 세팅해준다. 
	if (!CSceneManager::GetStaticPlayerInfo())
	{
		m_KirbyState = (CNormalKirbyState*)FindComponent("PlayerSprite");
		// m_KirbyState = dynamic_cast<CNormalKirbyState*>(FindComponent("PlayerSprite"));
		// m_KirbyState = static_cast<CNormalKirbyState*>(FindComponent("PlayerSprite"));

		// PullRightCollider
		m_PullRightCollider = CreateComponent<CColliderBox2D>("PullRightCollider");
		m_PullRightCollider->SetCollisionProfile("Player");

		m_PullRightCollider->SetExtend(100.f, 100.f);
		m_KirbyState->AddChild(m_PullRightCollider);

		// PullLeftCollider
		m_PullLeftCollider = CreateComponent<CColliderBox2D>("PullLeftCollider");
		m_PullLeftCollider->SetCollisionProfile("Player");
		m_PullLeftCollider->SetExtend(100.f, 100.f);
		m_KirbyState->AddChild(m_PullLeftCollider);

		m_PullColliderXInitScale = 100.f;
		m_PullColliderYInitScale = 100.f;

		m_PullColliderXMaxScale = (m_PullDistance + m_Body->GetWorldScale().x * 3.f) * 0.8f;
		m_PullColliderYMaxScale = (m_PullDistance + m_Body->GetWorldScale().y * 3.f) * 0.8f;
	}

	m_KirbyState->Start();

	SetRootComponent(m_KirbyState);

	m_InitPlayerPos = m_KirbyState->GetWorldPos();

	m_PullRightCollider = (CColliderBox2D*)FindComponent("PullRightCollider");
	m_PullRightCollider->AddCollisionCallback(Collision_State::Begin, this, &CPlayer2D::PullRightCollisionBeginCallback);
	m_PullRightCollider->AddCollisionCallback(Collision_State::End, this, &CPlayer2D::PullRightCollisionEndCallback);
	m_PullRightCollider->Start(); // SceneCollision 의 Collider List 에 추가하기
	m_PullRightCollider->Enable(false);
	m_PullRightCollider->SetRelativePos(m_Body->GetWorldScale().x * 1.5f * 0.5f, 0.f, 1.f);

	m_PullLeftCollider = (CColliderBox2D*)FindComponent("PullLeftCollider");
	m_PullLeftCollider->AddCollisionCallback(Collision_State::Begin, this, &CPlayer2D::PullLeftCollisionBeginCallback);
	m_PullLeftCollider->AddCollisionCallback(Collision_State::End, this, &CPlayer2D::PullLeftCollisionEndCallback);
	m_PullLeftCollider->Start(); // SceneCollision 의 Collider List 에 추가하기
	m_PullLeftCollider->Enable(false);
	m_PullLeftCollider->SetRelativePos(m_Body->GetWorldScale().x * -1.5f * 0.5f, 0.f, 1.f);
	
	m_NavAgent = dynamic_cast<CNavAgent*>(FindComponent("NavAgent"));

	// Widget Component의 Widget 생성
	
	m_SimpleHUDWidget = (CWidgetComponent*)(FindComponent("SimpleHUD"));

	if (!m_SimpleHUDWidget)
	{
		m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");
		m_KirbyState->AddChild(m_SimpleHUDWidget);
	}

	// 처음 Player가 만들어지는 것이라면
	/*
	if (!CSceneManager::GetStaticPlayerInfo())
	{
		m_SimpleHUDWidget->CreateUIWindow<CEmptyObjectHUD>("SimpleHUDWidget");
		m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);
	}
	*/

	// 매번 WidgetComponent의 Window는 그냥 새로 만들어준다
	// 어차피 Widget Component의 Window는 Shared Ptr 이기 때문이다.
	m_SimpleHUDWidget->CreateUIWindow<CEmptyObjectHUD>("SimpleHUDWidget");
	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);
	m_SimpleHUDWidget->GetWidgetWindow()->SetScene(m_Scene);
	m_SimpleHUDWidget->GetWidgetWindow()->GetViewPort()->SetScene(m_Scene);
	

	m_Camera = (CCameraComponent*)FindComponent("Camera");

	/*
	// todo : 왜 여기서 새롭게 만들어주면 작동을 안하게 되는 것일까 ?
	if (!m_Camera)
	{
		m_Camera = CreateComponent<CCameraComponent>("Camera");
		m_Camera->OnViewportCenter();
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
		m_KirbyState->AddChild(m_Camera);
	}
	*/

	// Key Input 세팅 
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveUp", 
		KeyState_Push, this, &CPlayer2D::MoveUp);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveUp",
		KeyState_Up, this, &CPlayer2D::MoveUpEnd);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDown", 
		KeyState_Push, this, &CPlayer2D::MoveDown);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveLeft", 
		KeyState_Push, this, &CPlayer2D::MoveLeft);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveLeft",
		KeyState_Up, this, &CPlayer2D::LeftLeverMoveEnd);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDashLeft", 
		KeyState_Push, this, &CPlayer2D::MoveDashLeft);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDashLeft",
		KeyState_Up, this, &CPlayer2D::LeftDashMoveEnd);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveRight", 
		KeyState_Push, this, &CPlayer2D::MoveRight);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveRight",
		KeyState_Up, this, &CPlayer2D::RightLeverMoveEnd);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDashRight", 
		KeyState_Push, this, &CPlayer2D::MoveDashRight);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDashRight",
		KeyState_Up, this, &CPlayer2D::RightDashMoveEnd);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("PullRight",
		KeyState_Push, this, &CPlayer2D::PullRight);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("PullRight",
		KeyState_Up, this, &CPlayer2D::PullRightEnd);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("PullLeft",
		KeyState_Push, this, &CPlayer2D::PullLeft);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("PullLeft",
		KeyState_Up, this, &CPlayer2D::PullLeftEnd);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("SpecialChange",
		KeyState_Down, this, &CPlayer2D::SpecialChangeStart);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Attack", 
		KeyState_Down, this, &CPlayer2D::Attack);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("SlideAttack", 
		KeyState_Down, this, &CPlayer2D::SlideAttack);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("SpecialAttack",
		KeyState_Down, this, &CPlayer2D::ChangePlayerSpecialAttackAnimation);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("UltimateAttack",
		KeyState_Down, this, &CPlayer2D::ChangePlayerUltimateAttackAnimation);

	/*
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Skill1", 
		KeyState_Down, this, &CPlayer2D::Skill1);
	*/

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MovePoint", 
		KeyState_Down, this, &CPlayer2D::MovePointDown);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Jump", 
		KeyState_Down, this, &CPlayer2D::Jump);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("JumpWhileDash",
		KeyState_Down, this, &CPlayer2D::Jump);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("JumpDown",
		KeyState_Down, this, &CPlayer2D::JumpDown);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("SpitOut",
		KeyState_Push, this, &CPlayer2D::SpitOut);

	// Jump 가 다 끝난 이후에는, Animation 을 Fall 상태로 바꿔라
	
	SetBasicSettingToChangedState();

	m_KirbyState->GetAnimationInstance()->SetCurrentAnimation("RightJump");

	// UI Setting
	SetUIAccordingToKirbyState();

	SetKirbyStateSizeAccordingToKirbyState();

	// Player를 한번 멈춘다.
	StopPlayer();

	m_SceneChange = false;

	LoadChangeImagesInAdvance();

	m_AttackAbility = 150.f;
}

void CPlayer2D::Update(float DeltaTime)
{
	CLifeObject::Update(DeltaTime);

	PlayerMoveUpdate(DeltaTime);

	FallFromCliff();

	CheckIsSwimming(DeltaTime);

	CheckWithinBossWorldResolution();

	UpdateBeingHit(DeltaTime);

	UpdateAttackTime(DeltaTime);

	UpdateChangeState(DeltaTime);

	UpdateSlideAttackTime(DeltaTime);

	UpdateSwimAction(DeltaTime);

	UpdateActionWhenReachGroundAfterFall();

	UpdateMP(DeltaTime);
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CLifeObject::PostUpdate(DeltaTime);

	CheckOutsideWorldResolution();

	JumpDownDistUpdate(DeltaTime);

	m_DeltaTime = 0.f;
}

CPlayer2D* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::SetIsEatingMonster(bool Enable)
{
	m_IsEatingMonster = Enable;

	if (Enable)
	{
		// Animation Change
		ChangePlayerIdleAnimation();
	}
}

void CPlayer2D::SetEatenMonster(CMonster* Monster)
{
	m_EatenMonster = Monster;
}

void CPlayer2D::SetBeingHitDirection(float Dir)
{
	if (Dir < 0)
		Dir = -1.f;
	if (Dir >= 0)
		Dir = 1.f;

	m_ObjectMoveDir.x = Dir;
}

void CPlayer2D::SetIsBeingHit()
{
	m_IsBeingHit = true;

	m_BeingHitTime = m_BeingHitTimeMax;

	ResetMoveInfo();

	m_LeverVelocity = 0.f;

	m_DashVelocity = 0.f;
}

void CPlayer2D::SetCameraFollowMaxTime(float Time)
{
	m_Camera->SetFollowTargetCameraTime(Time);
}

void CPlayer2D::SetAttackEnable(bool Enable)
{
	m_IsAttacking = Enable;
}

void CPlayer2D::InitObjectPoolSetting()
{
	if (m_SpecialAbilityState == Ability_State::Beam)
	{
		InitBeamKirbyObjectPool();

		m_mapObjectPool[Ability_State::Beam]->SetPoolInitializeFunction(this, &CPlayer2D::InitBeamKirbyObjectPool);
	}

	if (m_SpecialAbilityState == Ability_State::Fight)
	{
		InitFightKirbyObjectPool();

		m_mapObjectPool[Ability_State::Fight]->SetPoolInitializeFunction(this, &CPlayer2D::InitFightKirbyObjectPool);
	}
}

void CPlayer2D::InitBeamKirbyObjectPool()
{
	const std::vector<CSharedPtr<CKirbyAttackEffect>> vecAttackEffects = m_mapObjectPool[m_SpecialAbilityState]->GetVecKirbyAttackEffects();

	m_mapObjectPool[m_SpecialAbilityState]->ExtendPool(60);

	size_t TotSize = vecAttackEffects.size();

	for (size_t i = 0; i < TotSize; ++i)
	{
		CKirbyAttackEffect* AttackEffect = vecAttackEffects[i];

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BeamSpecial);
		AttackEffect->SetPoolOwner(m_mapObjectPool[m_SpecialAbilityState]);
		AttackEffect->SetKirbyOwner(m_KirbyState);
		AttackEffect->SetAttackDamage(m_KirbyState->GetExtraAttackAbility() + m_AttackAbility * 4);
	}
}

void CPlayer2D::InitFightKirbyObjectPool()
{
	const std::vector<CSharedPtr<CKirbyAttackEffect>> vecAttackEffects = m_mapObjectPool[m_SpecialAbilityState]->GetVecKirbyAttackEffects();

	m_mapObjectPool[m_SpecialAbilityState]->ExtendPool(100);

	size_t TotSize = vecAttackEffects.size();

	for (size_t i = 0; i < TotSize; ++i)
	{
		const float NumFrom0To1 = CClientManager::GetInst()->GenerateRandomNumberFrom0To1();

		const float NumFrom0To2 = CClientManager::GetInst()->GenerateRandomNumberFrom0To1();

		CKirbyAttackEffect* AttackEffect = vecAttackEffects[i];

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);

		AttackEffect->SetPoolOwner(m_mapObjectPool[m_SpecialAbilityState]);

		AttackEffect->SetKirbyOwner(m_KirbyState);

		AttackEffect->SetAttackDamage(m_KirbyState->GetExtraAttackAbility() + m_AttackAbility * 4);

		Vector3 CameraWorldPos = m_Scene->GetCameraManager()->GetCurrentCamera()->GetWorldPos();

		AttackEffect->SetWorldPos(
			CameraWorldPos.x + (NumFrom0To1 * 1200.f),
			CameraWorldPos.y + (300.f * NumFrom0To2) + (500.f * NumFrom0To1),
			0.f);

		AttackEffect->SetAttackObjectSpeed(1800 + (1000.f * NumFrom0To1));

		AttackEffect->SetAttackObjectMaxLimit(1000.f);

		AttackEffect->SetBottomCollisionEnable(false);

		AttackEffect->SetSideCollisionEnable(false);

		// 왼쪽을 보고 있었다면
		if (GetObjectMoveDir().x < 0.f)
			AttackEffect->SetLeftAttackDir(-1.f);
		// 오른쪽으로 보고 있었다면
		else
			AttackEffect->SetRightAttackDir(-1.f);

		AttackEffect->SetAttackDirX(0.f);
	}
}

void CPlayer2D::CheckWithinBossWorldResolution()
{
	if (m_Scene->GetBossWorldResolution().x > 0.f)
	{
		float RightPosX = GetWorldPos().x + GetWorldScale().x * GetPivot().x;

		if (RightPosX >= m_Scene->GetBossWorldResolution().x)
		{
			SetWorldPos(m_Scene->GetBossWorldResolution().x - GetWorldScale().x * GetPivot().x - 0.001f,
				GetWorldPos().y, GetWorldPos().z);
		}

		float LeftPosX = GetWorldPos().x - GetWorldScale().x * GetPivot().x;

		if (LeftPosX <= 0.f)
		{
			SetWorldPos(GetWorldScale().x * GetPivot().x + 0.001f, GetWorldPos().y, GetWorldPos().z);
		}
	}
}

void CPlayer2D::MoveUp(float DeltaTime)
{
	if (m_GamePlayDelayTime > 0.f)
		return;

	if (m_IsBeingHit)
		return;

	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	// 혹시나 미세한 차이로 인해 갑자기 Swim 아닌 상태로 바뀔 가능성이 있다.
	// 따라서 MoveUp 하기 전에 한번 더 검사해주는 것이다.
	CheckIsSwimming(DeltaTime);

	// if (m_SceneChangeCallback && m_FallTime < 0.1f && !m_IsFlying)
	// if (m_SceneChangeCallback && m_FallTime < 0.1f)
	if (m_SceneChangeCallback)
	{
		CollisionResult Result;

		m_SceneChangeCallback(Result);

		m_SceneChangeCallback = nullptr;

		m_SceneChange = true;

		StopPlayer();

		m_PhysicsSimulate = false;

		m_Scene->GetResource()->SoundPlay("PlayerChangeSceneStart");
	}
	else
	{
		if (m_SceneChange)
			return;

		if (m_IsSwimming)
		{
			ChangeAnimation("SwimUp");

			AddWorldPos(Vector3(0.f, 1.f, 0.f) * DeltaTime * 300.f);
		}
		else
		{
			if (m_IsLadderGoingUp)
			{
				ChangeAnimation("LadderUp");

				AddWorldPos(Vector3(0.f, 1.f, 0.f) * DeltaTime * 300.f);
			}
			else
			{
				FlyAfterJump(DeltaTime); 
			}
		}
	}
}

void CPlayer2D::MoveUpEnd(float DeltaTime)
{
	if (m_IsFlying)
	{
		// 땋에 닿은 상태라면, m_IsFlying을 false 로 해서
		// 다시 날려면, Jump 이후에 날 수 있게 세팅한다
		// if (m_IsGround)
		m_IsFlying = false;

		// Air Effect를 하나 만들어낸다.
		// CEffectSpitOut* EffectSpitOut = m_Scene->CreateGameObject<CEffectSpitOut>("DashEffect");
		CEffectJumpAir* EffectSpitOut = m_Scene->CreateGameObject<CEffectJumpAir>("DashEffect");

		EffectSpitOut->SetPlayerAttackCollider();

		Vector3 EffectPos = {};

		if (m_ObjectMoveDir.x > 0) // 오른쪽 
		{
			EffectPos = Vector3(GetWorldPos().x + (GetWorldScale().x * GetPivot().x * 1.3f),GetWorldPos().y, GetWorldPos().z);

			EffectSpitOut->SetRightEffect();
		}
		else // 왼쪽
		{
			EffectPos = Vector3(GetWorldPos().x - (GetWorldScale().x * GetPivot().x * 1.5f),GetWorldPos().y, GetWorldPos().z);

			EffectSpitOut->SetLeftEffect();
		}

		EffectSpitOut->SetMoveVelocity(500.f);

		EffectSpitOut->SetLifeTime(0.3f);
		EffectSpitOut->SetWorldPos(EffectPos);

		ChangePlayerSpitOutAnimation();

		m_Scene->GetResource()->SoundPlay("PlayerSpitOut");
		// ChangePlayerFallAnimation();
	}

	// Fall StartY 를 여기서 다시 세팅한다.
	m_FallTime = 0.f;
	m_FallStartY = GetWorldPos().y;

	m_IsFalling = true;
}

void CPlayer2D::MoveDown(float DeltaTime)
{
	if (m_IsSwimming)
	{
		m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_Y) * 100.f * DeltaTime * -1.f);
	}
	else if (m_IsLadderGoingUp)
	{
		ChangeAnimation("LadderDown");

		AddWorldPos(Vector3(0.f, 1.f, 0.f) * DeltaTime * 300.f * -1.f);
	}
}

void CPlayer2D::MoveLeft(float DeltaTime) //
{
	if (m_GamePlayDelayTime > 0.f)
		return;

	// 오른쪽 버튼을 누른 상태라면 X
	if (m_RightMovePush)
		return;

	if (m_IsBeingHit)
		return;

	if (m_IsChanging)
		return;

	if (m_IsAttacking)
		return;

	// 범위 제한 하기
	float WorldPosLeftX = GetWorldPos().x - GetPivot().x * GetWorldScale().x;
	float WorldPosRightX = GetWorldPos().x + GetPivot().x * GetWorldScale().x;

	// 현재는 Pull 하고 있는 상태가 아니므로 false 로 세팅한다.
	m_IsPulling = false;

	// 삼각 점프 상태였다면 계속 이동 시킨다
	if (m_TriangleJump)
	{
		if (m_RightMove)
		{
			if (WorldPosRightX >= m_Scene->GetWorldResolution().x - 0.1f)
				return;
			m_KirbyState->AddWorldPos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * m_TriangleJumpVelocityRatio);
		}
		else
		{
			if (WorldPosLeftX <= 0.1f)
				return;
			m_KirbyState->AddWorldPos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f * m_TriangleJumpVelocityRatio);
		}

		return;
	}

	// 레버 이동 표시
	m_IsLeverMoving = true;

	// 왼쪽 버튼 누르는 중 표시
	m_LeftMovePush = true;

	// 방향 왼쪽 표시
	m_ObjectMoveDir.x = 1.f * -1;

	// 이동 관련 상태 표시
	if (m_RightMove)
	{
		m_LeftMove = false;
		m_ToLeftWhenRightMove = true;
	}
	else
	{
		m_LeftMove = true;
		m_ToLeftWhenRightMove = false;
	}

	// 일반 이동 속도 계산
	CalculateLeverMoveSpeed(DeltaTime);

	// 여기로 들어온 것은 Dash Move 중은 아니라는 것이므로
	if (m_DashVelocity > 0.f)
	{
		m_DashVelocity -= m_DashMoveAccel * 0.3f;

		if (m_DashVelocity < 0.f)
			m_DashVelocity = 0.f;
	}

	// 총 이동 속도를 계산한다
	CalculateTotalMoveSpeed(DeltaTime);

	// 실제 이동 처리를 한다
	if (m_RightMove)
	{
		if (WorldPosRightX >= m_Scene->GetWorldResolution().x - 0.1f)
			return;
		m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
	}
	else
	{
		if (WorldPosLeftX <= 0.1f)
			return;
		m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);
	}

	// Animation 전환
		// Animation 전환
	// 현재 날다가, 방금 Air를 내뱉은 상태라면 X
	if (m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName() != "RightSpitOut" &&
		m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName() != "LeftSpitOut")
	{
		if (m_IsSwimming)
		{
			ChangePlayerSwimAnimation();
		}
		else if (m_IsLadderGoingUp)
		{
			ChangeAnimation("LadderUp");
		}
		else
		{
			ChangePlayerWalkAnimation();
		}
	}
}

void CPlayer2D::MoveDashLeft(float DeltaTime)
{
	// 오른쪽 버튼을 누르고 있다면
	if (m_RightMovePush)
		return;

	if (m_IsBeingHit)
		return;

	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	if (m_IsLadderGoingUp)
		return;

	// 범위 제한 하기
	float WorldPosLeftX = GetWorldPos().x - GetPivot().x * GetWorldScale().x;
	float WorldPosRightX = GetWorldPos().x + GetPivot().x * GetWorldScale().x;

	if (m_TriangleJump)
	{
		if (m_RightMove)
		{
			if (WorldPosRightX >= m_Scene->GetWorldResolution().x - 0.1f)
				return;
			m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * m_TriangleJumpVelocityRatio);
		}
		else
		{
			if (WorldPosLeftX <= 0.1f)
				return;
			m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f * m_TriangleJumpVelocityRatio);
		}
		return;
	}

	// 이동 상태 True + 버튼 이동 상태 True
	m_IsLeverMoving = true;
	m_IsDashMoving = true;

	m_LeftMovePush = true;

	// 방향 왼쪽 표시
	m_ObjectMoveDir.x = 1.f * -1;

	if (m_RightMove)
	{
		m_LeftMove = false;
		m_ToLeftWhenRightMove = true;
	}
	else
	{
		m_LeftMove = true;
		m_ToLeftWhenRightMove = false;
	}

	CalculateLeverMoveSpeed(DeltaTime);

	CalculateDashMoveSpeed(DeltaTime);

	CalculateTotalMoveSpeed(DeltaTime);

	if (m_RightMove)
	{
		if (WorldPosRightX >= m_Scene->GetWorldResolution().x - 0.1f)
			return;
		m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
	}
	else
	{
		if (WorldPosLeftX <= 0.1f)
			return;
		m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);
	}

	// Animation 전환
	if (m_IsSwimming)
	{
		ChangePlayerSwimAnimation();
	}
	else
	{
		ChangePlayerRunAnimation();

		// Effect 생성
		if (!m_MoveDashEffectMade && !m_Jump && m_MoveDashEffectLimitTime >= m_MoveDashEffectLimitTimeMax)
		{
			CEffectDash* EffectDash = m_Scene->CreateGameObject<CEffectDash>("DashEffect");

			Vector3 EffectPos = Vector3(GetWorldPos().x,
				GetWorldPos().y - GetWorldScale().y * GetPivot().y,
				GetWorldPos().z);

			EffectDash->SetWorldPos(EffectPos);

			EffectDash->SetDirGoRight(true);

			m_MoveDashEffectMade = true;

			m_MoveDashEffectLimitTime -= m_MoveDashEffectLimitTimeMax;

			m_Scene->GetResource()->SoundPlay("PlayerDash");
		}
	}
}

void CPlayer2D::MoveRight(float DeltaTime)
{
	if (m_GamePlayDelayTime > 0.f)
		return;

	if (m_IsBeingHit)
		return;

	if (m_IsChanging)
		return;

	if (m_IsAttacking)
		return;
	
	// 왼쪽 키를 누르고 있었다면 안먹게 한다
	if (m_LeftMovePush)
		return;

	float WorldPosLeftX = GetWorldPos().x - GetPivot().x * GetWorldScale().x;
	float WorldPosRightX = GetWorldPos().x + GetPivot().x * GetWorldScale().x;

	// 현재는 Pull 하고 있는 상태가 아니므로 false 로 세팅한다.
	m_IsPulling = false;

	// Triangle Jump 중이라면 이동은 계속 하되 + 키보드는 안먹게 한다
	if (m_TriangleJump)
	{
		if (m_RightMove)
		{
			if (WorldPosRightX >= m_Scene->GetWorldResolution().x - 0.1f)
				return;
			m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * m_TriangleJumpVelocityRatio);
		}
		else
		{
			if (WorldPosLeftX <= 0.1f)
				return;
			m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f * m_TriangleJumpVelocityRatio);
		}

		return;
	}


	// 일반 이동 표시
	m_IsLeverMoving = true;

	// 오른쪽 이동 표시
	m_RightMovePush = true;

	// 방향 왼쪽 표시
	m_ObjectMoveDir.x = 1.f;

	// 이동 상태 Update
	if (m_LeftMove)
	{
		m_RightMove = false;
		m_ToRightWhenLeftMove = true;
	}
	else
	{
		m_RightMove = true;
		m_ToRightWhenLeftMove = false;
	}

	// 이동 속도 계산
	CalculateLeverMoveSpeed(DeltaTime);

	// 대쉬 속도 Update
	if (m_DashVelocity > 0.f)
	{
		m_DashVelocity -= m_DashMoveAccel * 0.3f;

		if (m_DashVelocity < 0.f)
			m_DashVelocity = 0.f;
	}

	// 총 이동 속도 계산
	CalculateTotalMoveSpeed(DeltaTime);

	// 실제 이동 수행
	if (m_RightMove)
	{
		if (WorldPosRightX >= m_Scene->GetWorldResolution().x - 0.1f)
			return;
		m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
	}
	else
	{
		if (WorldPosLeftX <= 0.1f)
			return;
		m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);
	}

	// Animation 전환
	// 현재 날다가, 방금 Air를 내뱉은 상태라면 X
	if (m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName() != "RightSpitOut" &&
		m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName() != "LeftSpitOut")
	{
		if (m_IsSwimming)
		{
			ChangePlayerSwimAnimation();
		}
		else if (m_IsLadderGoingUp)
		{
			ChangeAnimation("LadderUp");
		}
		else
		{
			ChangePlayerWalkAnimation();
		}
	}
}

void CPlayer2D::MoveDashRight(float DeltaTime)
{
	if (m_LeftMovePush)
		return;

	if (m_IsBeingHit)
		return;

	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	if (m_IsLadderGoingUp)
		return;

	float WorldPosLeftX = GetWorldPos().x - GetPivot().x * GetWorldScale().x;
	float WorldPosRightX = GetWorldPos().x + GetPivot().x * GetWorldScale().x;

	if (m_TriangleJump)
	{
		if (m_RightMove)
		{
			if (WorldPosRightX >= m_Scene->GetWorldResolution().x - 0.1f)
				return;
			m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * m_TriangleJumpVelocityRatio);
		}
		else
		{
			if (WorldPosLeftX <= 0.1f)
				return;
			m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f * m_TriangleJumpVelocityRatio);
		}
		return;
	}

	m_IsLeverMoving = true;

	m_IsDashMoving  = true;

	m_RightMovePush = true;

	m_ObjectMoveDir.x = 1.f;

	if (m_LeftMove)
	{
		m_RightMove = false;
		m_ToRightWhenLeftMove = true;
	}
	else
	{
		m_RightMove = true;
		m_ToRightWhenLeftMove = false;
	}

	CalculateLeverMoveSpeed(DeltaTime);

	CalculateDashMoveSpeed(DeltaTime);

	CalculateTotalMoveSpeed(DeltaTime);

	if (m_RightMove)
	{
		if (WorldPosRightX >= m_Scene->GetWorldResolution().x - 0.1f)
			return;
		m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
	}
	else
	{
		if (WorldPosLeftX <= 0.1f)
			return;
		m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);
	}

	// Animation 전환
	if (m_IsSwimming)
	{
		ChangePlayerSwimAnimation();
	}
	else
	{
		ChangePlayerRunAnimation();

		// Effect 생성
		if (!m_MoveDashEffectMade && !m_Jump && m_MoveDashEffectLimitTime >= m_MoveDashEffectLimitTimeMax)
		{
			CEffectDash* EffectDash = m_Scene->CreateGameObject<CEffectDash>("DashEffect");

			Vector3 EffectPos = Vector3(GetWorldPos().x - (GetWorldScale().x * GetPivot().x) * 2,
				GetWorldPos().y - GetWorldScale().y * GetPivot().y,
				GetWorldPos().z);

			EffectDash->SetWorldPos(EffectPos);

			EffectDash->SetDirGoRight(false);

			m_MoveDashEffectMade = true;

			m_MoveDashEffectLimitTime -= m_MoveDashEffectLimitTimeMax;

			m_Scene->GetResource()->SoundPlay("PlayerDash");
		}
	}
}

void CPlayer2D::LeftLeverMoveEnd(float DeltaTime)
{
	m_LeftMovePush = false;

	ResetMoveInfo();

	ChangePlayerIdleAnimation();

	PullLeftEnd(DeltaTime);

	m_MoveDashEffectMade = false;
}

void CPlayer2D::RightLeverMoveEnd(float DeltaTime)
{
	m_RightMovePush = false;

	ResetMoveInfo();

	ChangePlayerIdleAnimation();

	PullRightEnd(DeltaTime);

	m_MoveDashEffectMade = false;
}

void CPlayer2D::LeftDashMoveEnd(float DeltaTime)
{
	ChangePlayerWalkAnimation();

	m_LeftMovePush = false;

	ResetMoveInfo();

	PullLeftEnd(DeltaTime);

	m_MoveDashEffectMade = false;
}

void CPlayer2D::RightDashMoveEnd(float DeltaTime)
{
	ChangePlayerWalkAnimation();

	m_RightMovePush = false;

	ResetMoveInfo();

	PullRightEnd(DeltaTime);

	m_MoveDashEffectMade = false;
}

void CPlayer2D::SpitOut(float DeltaTime)
{
	if (!m_IsEatingMonster)
		return;

	if (m_IsBeingHit)
		return;

	if (m_IsChanging)
		return;

	if (m_IsAttacking)
		return;

	if (m_IsSwimming)
		return;

	if (m_IsLadderGoingUp)
		return;

	CEffectStar* SpitOutStar = m_Scene->CreateGameObject<CEffectStar>("SpitOutStar");

	SpitOutStar->SetWorldPos(GetWorldPos());

	SpitOutStar->SetIsRotate(true);

	if (m_ObjectMoveDir.x > 0)
		SpitOutStar->SetRightAttackDir();
	else
		SpitOutStar->SetLeftAttackDir();

	m_IsEatingMonster = false;

	// 특별 몬스터를 내뱉는 것이라면, 
	if (m_EatenMonster->GetMonsterType() == Monster_Type::Ability)
	{
		CAbilityMonster* AbilityMonster = (CAbilityMonster*)m_EatenMonster.Get();

		// Star 세팅 추가
		SpitOutStar->SetIsSpecialKirbyStar(true);
		SpitOutStar->SetJumpVelocity(100.f);
		SpitOutStar->SetPhysicsSimulate(true);
		SpitOutStar->JumpStart();
		SpitOutStar->SetLifeTime(10.f);
		SpitOutStar->SetNormalBottomCollision(true);
		SpitOutStar->SetIsPulledAgainStar(true);
		SpitOutStar->SetAbilityState(AbilityMonster->GetAbilityState());

		// Prev Eaten Monster에 정보세팅
		m_PrevAbilityEatenMonster = m_EatenMonster;
	}
	else
	{
		SpitOutStar->SetAttackDamage(m_AttackAbility);
	}

	// todo : 이거는 필요할 수도 있다.
	// m_EatenMonster->Destroy();

	/*
	m_EatenMonster->Enable(true);

	m_EatenMonster->SetBeingSpitOut(true);

	m_EatenMonster->SetWorldPos(GetWorldPos());

	m_EatenMonster->SetAIState(Monster_AI::Hit);

	if (m_ObjectMoveDir.x > 0)
	{
		m_EatenMonster->SetObjectMoveDir(Vector3(1.f, 0.f, 0.f));
	}
	else
	{
		m_EatenMonster->SetObjectMoveDir(Vector3(1.f * -1, 0.f, 0.f));
	}
	*/

	m_EatenMonster = nullptr;

	ChangePlayerSpitOutAnimation();

	// Special State 였다면 다시 원래 상태로 돌아가기
	if (m_IsSpecialStateChanged)
	{
		m_SpecialAbilityState = Ability_State::End;

		Vector3 OriginalPos = GetWorldPos();

		m_KirbyState = CreateComponent<CNormalKirbyState>("NormalKirbyState");

		SetRootComponent(m_KirbyState);

		m_KirbyState->SetWorldPos(OriginalPos);

		m_KirbyState->AddChild(m_Camera);
		m_KirbyState->AddChild(m_SimpleHUDWidget);
		m_KirbyState->AddChild(m_Body);
		m_KirbyState->AddChild(m_PullRightCollider);
		m_KirbyState->AddChild(m_PullLeftCollider);

		m_KirbyState->SetWorldScale(80.f, 80.f, 1.f);

		m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

		m_KirbyState->SetTransparency(true);

		m_KirbyState->SetPivot(0.5f, 0.5f, 0.f);

		m_IsSpecialStateChanged = false;

		SetBasicSettingToChangedState();

		CPlayerHUD* PlayerHUD = (CPlayerHUD*)m_Scene->GetPlayerHUD();

		PlayerHUD->GetNameText()->SetText(TEXT("KIRBY"));

		PlayerHUD->GetIconImage()->SetTexture("NormalKirbyIconImage", TEXT("Project/UI/Icon_NormalKirby.png"));

		PlayerHUD->GetIconImage()->SetSize(50.f, 50.f);

		ChangePlayerNormalIdleAnimation();
	}

	m_Scene->GetResource()->SoundPlay("PlayerSpitOut");
}

float CPlayer2D::CalculateLeverMoveSpeed(float DeltaTime)
{
	// 오른쪽으로 이동중이라면
	if (m_RightMove)
	{
		// 계속 오른쪽 이동 중이라면
		if (!m_ToLeftWhenRightMove)
		{
			m_LeverVelocity += m_LeverMoveAccel;
		}
		// 그게 아니라, 오른쪽으로 이동 중이다가, 왼쪽으로 감속하는 것이라면
		else
		{
			m_LeverVelocity -= m_LeverMoveAccel * 2.f;
		}
	}

	// 만약 왼쪽 이동중이라면
	else if (m_LeftMove)
	{
		// 계속 왼쪽 이동 중이라면
		if (!m_ToRightWhenLeftMove)
		{
			m_LeverVelocity += m_LeverMoveAccel;
		}
		// 왼쪽으로 이동 중이다가, 오른쪽으로 감속하는 것이라면
		else
		{
			m_LeverVelocity -= m_LeverMoveAccel * 2.f;
		}
	}

	// 최대 속도 조절
	if (m_LeverVelocity >= m_LeverMaxMoveVelocity)
	{
		m_LeverVelocity = m_LeverMaxMoveVelocity;
	}

	// 최소 속도 조절
	if (m_LeverVelocity < 0.f)
		m_LeverVelocity = 0.f;

	return m_LeverVelocity;
}

float CPlayer2D::CalculateDashMoveSpeed(float DeltaTime)
{
	// 공중에 떠있다면 Dash Move 는 진행하지 않는다
	if (!m_IsGround)
		return m_LeverVelocity;

	// 오른쪽으로 이동 중이라면
	if (m_RightMove)
	{
		// 계속 오른쪽으로 이동 중이라면
		if (!m_ToLeftWhenRightMove)
		{
			m_DashVelocity += m_DashMoveAccel;
		}
		else
		{
			m_DashVelocity -= m_DashMoveAccel * 2.f;
		}
	}
	// 왼쪽으로 이동 중이라면
	else if (m_LeftMove)
	{
		// 계속 왼쪽으로 이동 중이라면
		if (!m_ToRightWhenLeftMove)
		{
			m_DashVelocity += m_DashMoveAccel;
		}
		else
		{
			m_DashVelocity -= m_DashMoveAccel * 2.f;
		}
	}

	// 최대 치 조절
	if (m_DashVelocity >= m_DashMaxMoveVelocity)
	{
		m_DashVelocity = m_DashMaxMoveVelocity;
	}

	// 최소 치 조절
	if (m_DashVelocity < 0.f)
		m_DashVelocity = 0.f;

	return m_DashVelocity;
}

float CPlayer2D::CalculateTotalMoveSpeed(float DeltaTime)
{
	m_MoveVelocity = m_LeverVelocity + m_DashVelocity;

	// 최대 치 조절
	if (m_MoveVelocity > m_MoveVelocityMax)
		m_MoveVelocity = m_MoveVelocityMax;
	
	// 최소 치 조절
	if (m_MoveVelocity <= 0.1f)
	{
		// 기존 오른쪽, 왼쪽 이동 표시 상태를 해제해준다.
		if (m_RightMove)
		{
			m_RightMove = false;
			m_ToRightWhenLeftMove = false;
		}
		if (m_LeftMove)
		{
			m_LeftMove = false;
			m_ToLeftWhenRightMove = false;
		}

		m_MoveVelocity = 0.f;
		m_LeverVelocity = 0.f;
		m_DashVelocity = 0.f;
	}

	return m_MoveVelocity;
}

void CPlayer2D::PlayerMoveUpdate(float DeltaTime)
{
	// 레버 움직임 --> 현재 움직이지 않고 있는 상황이라면, 자연 감속을 시켜줘야 한다.
	// 최대 움직임을 제한해줘야 한다.
	if (m_MoveVelocity >= m_MoveVelocityMax)
	{
		m_MoveVelocity = m_MoveVelocityMax;
	}

	// 레버를 누르고 있지 않다면, 레버 속도를 감속 시킨다.
	if (!m_IsLeverMoving)
	{
		m_LeverVelocity -= m_LeverMoveAccel * 0.3f ;

		if (m_LeverVelocity <= 0.f)
			m_LeverVelocity = 0.f;
	}

	// 대쉬를 진행하고 있지 않다면, 대쉬 속도도 감속 시킨다.
	if (!m_IsDashMoving)
	{
		m_DashVelocity -= m_DashMoveAccel * 0.3f;

		if (m_DashVelocity <= 0.f)
			m_DashVelocity = 0.f;
	}

	// 전체 속도 감속을 진행한다.
	if (!m_IsLeverMoving && !m_IsDashMoving && m_MoveVelocity > 0.f)
	{
		// 감속
		m_MoveVelocity = m_LeverVelocity + m_DashVelocity;

		// 이동을 멈추면 -> 오른쪽 , 왼쪽 이동 표시를 취소해준다.
		if (m_MoveVelocity <= 0.1f)
		{
			if (m_RightMove)
			{
				m_RightMove = false;
				m_ToRightWhenLeftMove = false;
			}
			if (m_LeftMove)
			{
				m_LeftMove = false;
				m_ToLeftWhenRightMove = false;
			}

			if (!m_IsSwimming && !m_IsLadderGoingUp)
			{
				ChangePlayerIdleAnimation();
			}

			m_MoveVelocity = 0.f;
		}

		// Animation 세팅
		// ChangePlayerWalkAnimation();

		// 범위 제한 하기
		float WorldPosLeftX = GetWorldPos().x - GetPivot().x * GetWorldScale().x;
		float WorldPosRightX = GetWorldPos().x + GetPivot().x * GetWorldScale().x;

		// 감속 중임에도 이동은 시켜줘야 한다.
		if (m_RightMove)
		{
			if (WorldPosRightX >= m_Scene->GetWorldResolution().x - 0.1f)
				return;
			m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
		}
		else
		{
			if (WorldPosLeftX < 0.1f)
				return;
			m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);
		}
	}

	// 대시 Effect Limit Time을 Update 시킨다.
	if (m_MoveDashEffectLimitTime < m_MoveDashEffectLimitTimeMax)
	{
		m_MoveDashEffectLimitTime += DeltaTime;
	}
}

void CPlayer2D::ResetMoveInfo()
{
	m_IsDashMoving = false;
	m_IsLeverMoving = false;
	m_ToRightWhenLeftMove = false;
	m_ToLeftWhenRightMove = false;

	if (m_IsPulling)
	{
		m_IsPulling = false;
	}
}

void CPlayer2D::ResetCameraInfoToPlayer()
{
	m_KirbyState->AddChild(m_Camera);

	// m_PhysicsSimulate = true;
}

void CPlayer2D::CheckBelowWorldResolution()
{
	if (m_GamePlayDelayTime > 0)
		return;

	if (m_IsSwimming)
	{
		SetWorldPos(GetWorldPos().x, GetWorldScale().y * GetPivot().y, GetWorldPos().z);
	}
	else
	{
		Vector3 WorldPos = GetWorldPos();

		m_Camera->SetFollowPlayer(true);

		m_Camera->SetInheritParentWorldPosChange(false);

		// 이것이 true 면, While Off Ground 처리로 인해 X Pos가 계속 현재로 유지되어 버린다.
		m_PhysicsSimulate = false;

		SetWorldPos(m_InitPlayerPos);
		// SetWorldPos(Vector3(WorldPos.x - 300.f, WorldPos.y + 500.f, WorldPos.z));

		m_FallTime = 0.f;

		m_FallStartY = m_InitPlayerPos.y;
	}
}

void CPlayer2D::SetCameraFollowBossMonster(class CGameObject* Boss)
{
	if (m_GamePlayDelayTime > 0)
		return;

	m_Camera->SetFollowTarget(true);

	m_Camera->SetFollowTargetObject(Boss);

	m_Camera->SetInheritParentWorldPosChange(false);
}

void CPlayer2D::StopPlayer()
{
	m_LeverVelocity = 0.f;
	m_DashVelocity = 0.f;
	m_MoveVelocity = 0.f;
}

void CPlayer2D::CheckIsSwimming(float DeltaTime)
{
	CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

	if (!TileMap)
		return;

	if (m_PrevPos.y == GetWorldPos().y)
		return;

	float ResultLeft = -1, ResultRight = -1, ResultTop = -1, ResultBottom = -1;
	int StartX = -1, EndX = -1, TopY = -1, BottomY = -1;

	float PrevLeft = m_PrevPos.x - GetWorldScale().x * GetPivot().x;
	float PrevRight = m_PrevPos.x + GetWorldScale().x * GetPivot().x;
	float PrevBottom = m_PrevPos.y - GetWorldScale().y * GetPivot().y;
	float PrevTop = m_PrevPos.y + GetWorldScale().y * GetPivot().y;

	float CurLeft = GetWorldPos().x - GetWorldScale().x * GetPivot().x;
	float CurRight = GetWorldPos().x + GetWorldScale().x * GetPivot().x;
	float CurBottom = GetWorldPos().y - GetWorldScale().x * GetPivot().x;
	float CurTop = GetWorldPos().y + GetWorldScale().y * GetPivot().y;

	if (m_PrevPos.y > GetWorldPos().y)
	{
		// 내려갈 때
		// 이 경우는, 물 이 아닌 곳에서 내려오는 상황 밖에 없다.
		// 따라서 현재 수영 중이라면 검사하지 않는다.
		if (m_IsSwimming)
			return;

		// 나는 중이 아닐 때 --> 그냥 확 떨어질 때만 검사한다.
		// if (!m_IsFlying)
		//	return;

		ResultLeft = CurLeft < PrevLeft ? CurLeft : PrevLeft;
		ResultRight = CurRight > PrevRight ? CurRight : PrevRight;
		ResultBottom = CurBottom < PrevBottom ? CurBottom : PrevBottom;
		ResultTop = CurBottom > PrevBottom ? CurBottom : PrevBottom;

		StartX = TileMap->GetTileEmptyIndexX(ResultLeft);
		EndX = TileMap->GetTileEmptyIndexX(ResultRight);

		TopY = TileMap->GetTileEmptyIndexY(ResultTop);
		BottomY = TileMap->GetTileEmptyIndexY(ResultBottom);

		if (StartX == -1 || EndX == -1 || TopY == -1 || BottomY == -1)
			return;

		// 아래에서 위로 검사한다.
		for (int row = BottomY; row <= TopY; row++)
		{
			for (int col = StartX; col <= EndX; col++)
			{
				if (TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Water)
				{
					m_IsSwimming = true;

					if (!m_IsSwimSoundOn)
					{
						m_Scene->GetResource()->SoundPlay("PlayerEnterWater");
						m_IsSwimSoundOn = true;
					}

					ChangePlayerSwimAnimation();

					m_PhysicsSimulate = false;

					CEffectWaterBlast* BlastEffect = m_Scene->CreateGameObject<CEffectWaterBlast>("WaterBlast");

					BlastEffect->SetWorldPos(GetWorldPos().x,
						GetWorldPos().y - GetWorldScale().y * GetPivot().y,
						GetWorldPos().z);

					return;
				}
			}
		}
	}
	else if (m_PrevPos.y < GetWorldPos().y)
	{
		// 올라갈 때 확인
		// 이 경우는 수영 중 , 물 밖으로 나가는 경우를 조사하는 것이므로
		// 수영중이 아닐 때는 검사하지 않는다.
		if (!m_IsSwimming)
			return;

		ResultLeft = CurLeft < PrevLeft ? CurLeft : PrevLeft;
		ResultRight = CurRight < PrevRight ? CurRight : PrevRight;

		// 대신 검사 기준은 Bottom을 기준으로 한다.
		// 물에서 완전히 나왔을 때를 조사하기 위해서이다.
		ResultBottom = CurBottom < PrevBottom ? CurBottom : PrevBottom;
		ResultTop = CurBottom > PrevBottom ? CurBottom : PrevBottom;

		StartX = TileMap->GetTileEmptyIndexX(ResultLeft);
		EndX = TileMap->GetTileEmptyIndexX(ResultRight);

		TopY = TileMap->GetTileEmptyIndexY(ResultTop);
		BottomY = TileMap->GetTileEmptyIndexY(ResultBottom);

		if (StartX == -1 || EndX == -1 || TopY == -1 || BottomY == -1)
			return;

		// 위에서 아래로 검사한다.
		for (int row = TopY; row >= BottomY; row--)
		{
			for (int col = StartX; col <= EndX; col++)
			{
				if (TileMap->GetTileEmpty(col, row)->GetTileType() != Tile_Type::Water)
				{
					m_IsSwimming = false;

					if (m_IsSwimSoundOn)
					{
						m_Scene->GetResource()->SoundStop("PlayerEnterWater");
						m_IsSwimSoundOn = false;
					}

					m_PhysicsSimulate = true;

					CEffectWaterBlast* BlastEffect = m_Scene->CreateGameObject<CEffectWaterBlast>("WaterBlast");

					BlastEffect->SetWorldPos(GetWorldPos().x,
						GetWorldPos().y - GetWorldScale().y * GetPivot().y,
						GetWorldPos().z);

					m_Scene->GetResource()->SoundPlay("PlayerGetOutOfWater");

					return;
				}
			}
		}
	}

	if (!m_IsSwimming)
	{
		if (m_IsSwimSoundOn)
		{
			m_Scene->GetResource()->SoundStop("PlayerEnterWater");
			m_IsSwimSoundOn = false;
		}
	}
}

void CPlayer2D::UpdateSwimAction(float DeltaTime)
{
	if (!m_IsSwimming)
		return;

	AddWorldPos(Vector3(0.f, -1.f, 0.f) * DeltaTime * 100.f);
}

void CPlayer2D::UpdateMP(float DeltaTime)
{
	if (m_MP < m_MPMax)
	{
		m_MP += DeltaTime * 20.f;

		CPlayerHUD* HUD = dynamic_cast<CPlayerHUD*>(m_Scene->GetPlayerHUD());

		if (!HUD)
			return;

		HUD->GetMPProgressBar()->SetPercent(m_MP / m_MPMax);
	}
}

void CPlayer2D::DecreaseMP(float MP)
{
	m_MP -= MP;

	if (m_MP < 0.f)
	{
		m_MP = 0.f;
	}

	CPlayerHUD* HUD = dynamic_cast<CPlayerHUD*>(m_Scene->GetPlayerHUD());

	if (!HUD)
		HUD->GetMPProgressBar()->SetPercent(m_MP / m_MPMax);

	m_Scene->GetResource()->SoundPlay("PlayerItemGet");
}

void CPlayer2D::AddHP(float HP)
{
	m_HP += HP;

	if (m_HP >= m_HPMax)
	{
		m_HP = m_HPMax;
	}

	CPlayerHUD* HUD = dynamic_cast<CPlayerHUD*>(m_Scene->GetPlayerHUD());

	if (HUD)
	{
		HUD->GetHPProgressBar()->SetPercent(m_HP / m_HPMax);
	}

	// Random Star Effect
	for (int i = 0; i < 3; i++)
	{
		CEffectRandomStar* RandomStar = m_Scene->CreateGameObject<CEffectRandomStar>("RandomStar");
		RandomStar->SetWorldPos(Vector3(GetWorldPos().x - (GetWorldScale().x * GetPivot().x) * 2.f, GetWorldPos().y, GetWorldPos().z));
	}

	m_Scene->GetResource()->SoundPlay("PlayerItemGet");
}

void CPlayer2D::Damage(float Damage)
{
	// 실제 데미지
	m_HP -= Damage;

	if (m_HP < 0.f)
	{
		m_HP = 1.f;
		
		return;
	}

	// Shake Effect 적용하기 
	m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect();


	// Widget HP Bar 조정하기
	// CSimpleHUD* HUD = dynamic_cast<CSimpleHUD*>(m_SimpleHUDWidget->GetWidgetWindow());
	// HUD->GetProgressBar()->SetPercent(m_HP / m_HPMax);

	/*
	CUIWindow * Window = m_Scene->GetPlayerHUD();

	if (!Window)
		return;

	CPlayerHUD* HUD = dynamic_cast<CPlayerHUD*>(Window);

	if (!Window)
		return;
	*/

	CPlayerHUD* HUD = dynamic_cast<CPlayerHUD*>(m_Scene->GetPlayerHUD());

	if (HUD)
	{
		HUD->GetHPProgressBar()->SetPercent(m_HP / m_HPMax);
	}

	m_Scene->GetResource()->SoundPlay("PlayerDamaged");
}

void CPlayer2D::UpdateBeingHit(float DeltaTime)
{
	if (!m_IsBeingHit)
		return;

	if (m_BeingHitTime > 0.f)
	{
		m_BeingHitTime -= DeltaTime;

		m_KirbyState->SetOpacity(0.7f);

		AddWorldPos(Vector3(m_ObjectMoveDir.x, 0.f, 0.f) * DeltaTime * 450.f);

		if (GetWorldPos().x - GetWorldScale().x * GetPivot().x < 0.f)
		{
			SetWorldPos(GetWorldScale().x * GetPivot().x + 0.001f, GetWorldPos().y, 0.f);
		}
		if (GetWorldPos().x + GetWorldScale().x * GetPivot().x >= m_Scene->GetWorldResolution().x)
		{
			SetWorldPos(m_Scene->GetWorldResolution().x - GetWorldScale().x * GetPivot().x - 0.001f, GetWorldPos().y, 0.f);
		}

		ChangePlayerHitAnimation();
			
		if (m_BeingHitTime < 0.f)
		{
			m_IsBeingHit = false;

			m_KirbyState->SetOpacity(1.f);
		}
	}
}

void CPlayer2D::CheckPlayerWaterCollision()
{
	CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

	if (!TileMap)
		return;

	float PrevLeft = m_PrevPos.x - GetWorldScale().x - GetPivot().x;
	float PrevRight = m_PrevPos.x + GetWorldScale().x - GetPivot().x;
	float PrevTop = m_PrevPos.y + GetWorldScale().y - GetPivot().y;
	float PrevBottom = m_PrevPos.x - GetWorldScale().x - GetPivot().x;

	float CurLeft= m_PrevPos.x - GetWorldScale().x - GetPivot().x;
	float CurRight= m_PrevPos.x - GetWorldScale().x - GetPivot().x;
	float CurTop = m_PrevPos.x - GetWorldScale().x - GetPivot().x;
	float CurBottom = m_PrevPos.x - GetWorldScale().x - GetPivot().x;
}

void CPlayer2D::UpdateAttackTime(float DeltaTime)
{
	/*
	if (m_IsAttacking)
	{
		if (m_AttackTime < m_AttackTimeLimit)
		{
			m_AttackTime += DeltaTime;

			if (m_AttackTime > m_AttackTimeLimit)
			{
				m_AttackTime = 0.f;

				m_IsAttacking = false;
			}
		}
	}
	*/
}

void CPlayer2D::UpdateSlideAttackTime(float DeltaTime)
{
	if (m_SlideAttack)
	{
		if (m_SlideAttackTime > 0.f)
		{
			m_SlideAttackTime -= DeltaTime;

			m_SlideAttackSpeed -= m_DashMoveAccel;

			// 왼쪽 
			if (m_ObjectMoveDir.x < 0.f)
			{
				AddWorldPos(Vector3(1.f * -1.f, 0.f, 0.f) * DeltaTime * m_SlideAttackSpeed);
			}
			// 오른쪽
			else
			{
				AddWorldPos(Vector3(1.f, 0.f, 0.f) * DeltaTime * m_SlideAttackSpeed);
			}

			if (GetWorldPos().x - GetWorldScale().x * GetPivot().x < 0.f)
			{
				SetWorldPos(GetWorldScale().x * GetPivot().x + 0.001f, GetWorldPos().y, 0.f);
			}
			if (GetWorldPos().x + GetWorldScale().x * GetPivot().x >= m_Scene->GetWorldResolution().x)
			{
				SetWorldPos(m_Scene->GetWorldResolution().x - GetWorldScale().x * GetPivot().x - 0.001f, GetWorldPos().y, 0.f);
			}

			m_Body->SetCollisionProfile("PlayerAttack");
		}

		m_SlideAttackEffectCurTime += DeltaTime;

		if (m_SlideAttackEffectCurTime >= m_SlideAttackEffectToggleTime)
		{
			MakePlayerCloneEffect();

			m_SlideAttackEffectCurTime -= m_SlideAttackEffectToggleTime;
		}

		if (m_SlideAttackTime <= 0.f)
		{
			m_SlideAttackTime = -1.f;

			m_Body->SetCollisionProfile("Player");

			m_SlideAttack = false;

			m_IsAttacking = false;
		}
	}
}

void CPlayer2D::RotationZInv(float DeltaTime) //
{
	m_KirbyState->AddRelativeRotationZ(180.f * DeltaTime);
}

void CPlayer2D::RotationZ(float DeltaTime)
{
	m_KirbyState->AddRelativeRotationZ(-180.f * DeltaTime);
}

void CPlayer2D:: FlyAfterJump(float DeltaTime)
{
	if (m_GamePlayDelayTime > 0.f)
		return;

	if (m_IsBeingHit)
		return;

	if (m_IsAttacking)
		return;

	// 일단 한번 뛴 상태에서 날아야 한다.
	/*
	if (m_Jump)
	{
		m_IsFlying = true;

		// 중력 적용 방지
		m_Jump = false;
		m_IsGround = true;
	}
	*/

	m_IsFlying = true;

	// 삼각 점프 해제
	m_TriangleJump = false;

	// 중력 적용 방지
	m_Jump = false;
	m_IsGround = true;

	// 날고 나서 다시 바닥에 착지할 때, Bounce 될 수있도록 한다.
	m_Bounced = false;

	// Line 위에서 올라갈 수 있게 , JumpStart를 true로 세팅한ㄷ
	m_JumpStart = true;

	// 나는 중이라면, 계속 날게 세팅한다
	if (m_IsFlying)
	{
		m_KirbyState->AddWorldPos(Vector3(0.f, 1.f, 0.f) * m_FlySpeed * DeltaTime);

		// todo : Animation Change
		ChangePlayerFlyAnimation();

		// 중력 적용 방지
		m_Jump = false;
		m_IsGround = true;
	}

	m_FlySoundTime += DeltaTime;

	if (m_FlySoundTime >= m_FlySoundTimeMax)
	{
		m_FlySoundTime = 0.f;
		m_Scene->GetResource()->SoundPlay("PlayerFly");
	}
}

void CPlayer2D::SimpleJump()
{
	if (m_IsBeingHit)
		return;

	if (!m_Jump)
	{
  		m_Jump = true;
		m_IsGround = false;

		m_FallTime = 0.f;
		m_FallStartY = GetWorldPos().y;

		m_IsBottomCollided = false;

		m_JumpStart = true;

		// Animation Change
		if (m_IsEatingMonster && !m_IsSpecialStateChanged)
			ChangePlayerWalkAnimation();
		else
			ChangePlayerJumpAnimation();

		m_Scene->GetResource()->SoundPlay("PlayerJump");
	}
}

void CPlayer2D::WeakJump()
{
	if (!m_Jump)
	{
		m_Jump = true;

		m_IsWeakJump = true;

		m_IsGround = false;

		m_FallTime = 0.f;

		m_FallStartY = GetWorldPos().y;

		m_IsBottomCollided = false;

		m_JumpStart = true;

		m_JumpVelocity = 30.f;

		m_Scene->GetResource()->SoundPlay("PlayerJump");
	}
}

void CPlayer2D::Jump(float DeltaTime)
{
	if (m_GamePlayDelayTime > 0.f)
		return;

	if (m_IsBeingHit)
		return;

	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	if (m_IsSwimming)
		return;

	if (m_IsLadderGoingUp)
		return;

	// 삼각 충돌 적용하기
	bool SideCollision = false;

	CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

	Vector3 WorldPos    = GetWorldPos();
	Vector3 WorldScale = GetWorldScale();
	Vector3 Pivot			 = GetPivot();

	Vector3 LB = WorldPos - Pivot * WorldScale;
	Vector3 RT = LB + WorldScale;

	// 오른쪽이면, 오른쪽 한칸 타일
	if (m_RightMove)
	{
		// LBIndexX = TileMap->GetTileEmptyIndexX(Vector3(ResultLB.x, ResultLB.y, m_Pos.z));
		int IndexX = TileMap->GetTileEmptyIndexX(RT.x);

		int TopIndexY = TileMap->GetTileEmptyIndexY(RT.y);
		int BottomIndexY = TileMap->GetTileEmptyIndexY(LB.y);

		int IndexXRight = IndexX + 1;
		int IndexXLeft = IndexX - 1;

		if (IndexX >= TileMap->GetTileCountX() - 1)
			IndexX = TileMap->GetTileCountX() - 1;
		if (IndexXRight >= TileMap->GetTileCountX() - 1)
			IndexXRight = TileMap->GetTileCountX() - 1;
		if (IndexXLeft < 0)
			IndexXLeft = 0;

		for (int row = BottomIndexY; row <= TopIndexY; row++)
		{
			for (int col = IndexX; col <= IndexXRight; col++)
			{
				int IndexFinal = row * TileMap->GetTileCountX() + col;

				if (TileMap->GetTileEmpty(IndexFinal)->GetTileType() != Tile_Type::Wall)
					continue;

				Vector3 TilePos   = TileMap->GetTileEmpty(IndexFinal)->GetWorldPos();
				Vector3 TileSize = TileMap->GetTileEmpty(IndexFinal)->GetSize();

				// 현재 땅에 딱 붙어있다면 무시한다
				if (TilePos.y + TileSize.y - m_GroundOffSet <= LB.y &&
					LB.y <= TilePos.y + TileSize.y + m_GroundOffSet)
					continue;

				// 현재 위치 Tile 도 Wall 이고, 왼쪽도 Wall 이라면
				// 그것은 삼각 점프가 아니게 된다. 그냥 Tile이 연속적으로 있는 곳에 대해서는 삼각 점프 X
				if (col == IndexX)
				{
					if (TileMap->GetTileEmpty(row * TileMap->GetTileCountX() + IndexXLeft)->GetTileType() == Tile_Type::Wall)
						continue;
				}
				if (col == IndexXRight)
				{
					if (TileMap->GetTileEmpty(row * TileMap->GetTileCountX() + IndexX)->GetTileType() == Tile_Type::Wall)
						continue;
				}

				SideCollision = true;
				break;
			}
			if (SideCollision)
				break;
		}
	}

	// 왼쪽으로 이동하는 것이라면
	else if (m_LeftMove)
	{
		int IndexX = TileMap->GetTileEmptyIndexX(LB.x);
		int TopIndexY = TileMap->GetTileEmptyIndexY(RT.y);
		int BottomIndexY = TileMap->GetTileEmptyIndexY(LB.y);

		int IndexXLeft = IndexX - 1;
		int IndexXRight = IndexX + 1;

		if (IndexX < 0)
			IndexX = 0;
		if (IndexXLeft < 0)
			IndexXLeft = 0;
		if (IndexXRight >= TileMap->GetTileCountX())
			IndexXRight = TileMap->GetTileCountX() - 1;

		// 왼쪽 2개의 타일을 조사한다.
		for (int row = BottomIndexY; row <= TopIndexY; row++)
		{
			for (int col = IndexX; col >= IndexXLeft; col--)
			{
				int IndexFinal = row * TileMap->GetTileCountX() + col;

				if (TileMap->GetTileEmpty(IndexFinal)->GetTileType() != Tile_Type::Wall)
					continue;

				Vector3 TilePos = TileMap->GetTileEmpty(IndexFinal)->GetWorldPos();
				Vector3 TileSize = TileMap->GetTileEmpty(IndexFinal)->GetSize();

				// 현재 화면에 딱 붙어있는 경우에는 무시한다.
				if (TilePos.y + TileSize.y - m_GroundOffSet <= LB.y &&
					TilePos.y + TileSize.y + m_GroundOffSet)
					continue;

				// 현재 위치 Tile도 Wall 이고, 오른쪽도 Wall 이라면
				// 연속적으로 Tile이 놓여있는 곳
				// 그러한 곳에 대해서는 삼각 점프 X
				if (col == IndexX)
				{
					if (TileMap->GetTileEmpty(row * TileMap->GetTileCountX() + IndexXRight)->GetTileType()
						== Tile_Type::Wall)
						continue;
				}
				if (col == IndexXLeft)
				{
					if (TileMap->GetTileEmpty(row * TileMap->GetTileCountX() + IndexX)->GetTileType()
						== Tile_Type::Wall)
						continue;
				}

				SideCollision = true;

				break;
			}
			if (SideCollision)
				break;
		}
	}

	if (SideCollision)
	{
		// 현재 움직이는 방향 반대로 움직여야 한다
		// 오른쪽
		if (m_RightMove)
		{
			TriangleJumpLeft(DeltaTime);
		}
		else if (m_LeftMove)
		{
			TriangleJumpRight(DeltaTime);
		}

		// Animation Change
		if (m_IsEatingMonster)
			ChangePlayerWalkAnimation();
		else
			ChangePlayerJumpAnimation();
	}
	else
	{
		if (!m_IsFalling)
		{
			SimpleJump();   //
		}
	}

}

void CPlayer2D::TriangleJumpLeft(float DeltaTime)
{
	m_Jump = true;
	m_IsGround = false;

	// 점프 시작 높이 세팅
	m_FallStartY = GetWorldPos().y;

	// Fall Time 초기화
	m_FallTime = 0.f;

	m_TriangleJump = true;

	m_LeftMove = true;
	m_RightMove = false;

	m_ToLeftWhenRightMove = false;
	m_ToRightWhenLeftMove = false;

	m_Scene->GetResource()->SoundPlay("PlayerJump");
}

void CPlayer2D::TriangleJumpRight(float DeltaTime)
{
	m_Jump = true;
	m_IsGround = false;

	// 점프 시작 높이 세팅
	m_FallStartY = GetWorldPos().y;

	// Fall Time 초기화
	m_FallTime = 0.f;

	m_TriangleJump = true;

	m_RightMove = true;
	m_LeftMove = false;

	m_ToLeftWhenRightMove = false;
	m_ToRightWhenLeftMove = false;

	m_Scene->GetResource()->SoundPlay("PlayerJump");
}

void CPlayer2D::JumpDown(float DeltaTime)
{
	if (m_IsBeingHit)
		return;

	if (m_IsGround)
	{
		if (m_BottomCollidedTile && m_BottomCollidedTile->GetTileType() == Tile_Type::Block)
		{
			// 떨어지게 세팅
			m_IsGround = false;

			m_IsFalling = false;

			m_FallTime = 0.f;

			m_FallStartY = GetWorldPos().y;

			m_JumpDown = true;

			m_Scene->GetResource()->SoundPlay("PlayerJump");
		}
	}
}

void CPlayer2D::JumpDownDistUpdate(float DeltaTime)
{

	if (m_JumpDown)
	{
		m_JumpDownDist = m_FallStartY - GetWorldPos().y;

		if (m_JumpDownDist > GetWorldScale().y * 0.6f)
		{
			bool IsBlockExist = false;

			// Block이 아니라면
			CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

			float Top = GetWorldPos().y + GetWorldScale().y * GetPivot().y;
			float Bottom = GetWorldPos().y - GetWorldScale().y * GetPivot().y;
			float Left = GetWorldPos().x - GetWorldScale().x * GetPivot().x;
			float Right = GetWorldPos().x + GetWorldScale().x * GetPivot().x;

			Vector3 LT = Vector3(Left, Top, 0.f);
			Vector3 RB = Vector3(Right, Bottom, 0.f);

			int StartX = TileMap->GetTileEmptyIndexX(LT);
			int StartY = TileMap->GetTileEmptyIndexY(LT);

			int EndX = TileMap->GetTileEmptyIndexX(RB);
			int EndY = TileMap->GetTileEmptyIndexY(RB);

			for (int row = StartY; row <= EndY; row++)
			{
				for (int col = StartX; col <= EndX; col++)
				{
					if (TileMap->GetTileEmpty(col, row)->GetTileType() == Tile_Type::Block)
					{
						IsBlockExist = true;
						break;
					}
				}
				if (IsBlockExist)
					break;
			}

			if (IsBlockExist)
				return;

			m_JumpDown = false;
		}
	}
}

void CPlayer2D::FallFromCliff()
{
	if (m_IsSwimming)
		return;

	if (m_IsLadderGoingUp)
		return;

	// 절벽에서 막 떨어졌을 때
	if (m_FallStartY - GetWorldPos().y > 5.f && !m_IsBottomCollided && !m_IsFlying)
	{
		std::string CurAnimName = m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName();

		if (CurAnimName == "RightSpitOut" || CurAnimName == "LeftSpitOut")
			return;

		ChangePlayerFallAnimation();
	}
}

void CPlayer2D::UpdateActionWhenReachGroundAfterFall()
{
	if (m_IsSwimming)
		return;

	if (m_IsLadderGoingUp)
		return;

	if (m_IsBottomCollided && !m_IsChanging)
	{
		if (!m_Bounced && m_FallStartY > GetWorldPos().y + GetWorldScale().y * 1.5f)
		{
			SimpleJump();

			m_Bounced = true;

			CEffectRandomStar* RandomStar = m_Scene->CreateGameObject<CEffectRandomStar>("RandomStar");
			RandomStar->SetWorldPos(Vector3(GetWorldPos().x - (GetWorldScale().x * GetPivot().x) * 2.f, GetWorldPos().y, GetWorldPos().z));
		}

		else
		{
			// 충돌하는 순간 Animation 이 Fall 이라면, Animation을 Idle로
			std::string CurAnimName = m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName();

			// 1. 떨어지고 있었거나
			// 2. 점프중이었거나
			if (CurAnimName == "RightFall" || CurAnimName == "LeftFall" ||
				CurAnimName == "RightJump" || CurAnimName == "LeftJump")
			{
				if (m_IsEatingMonster && !m_IsSpecialStateChanged)
					ChangePlayerEatIdleAnimation();
				else
					ChangePlayerNormalIdleAnimation();

				m_Scene->GetResource()->SoundPlay("PlayerJump");

				CEffectRandomStar* RandomStar = m_Scene->CreateGameObject<CEffectRandomStar>("RandomStar");
				RandomStar->SetWorldPos(Vector3(GetWorldPos().x - (GetWorldScale().x * GetPivot().x) * 2.f, GetWorldPos().y, GetWorldPos().z));
			}
		}

		m_IsFalling = false;

		// Sword Kirby의 Normal Attack시 살짝 점프하여 공격할 것이다.
		if (m_IsWeakJump)
		{
			m_IsWeakJump = false;
			m_JumpVelocity = m_InitJumpVelocity;
		}

		// 혹시나, Player Change 이후, Black 배경이 사라지지 않았다면, 사라지게 한다.
	}
}

void CPlayer2D::ChangePlayerIdleAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_IsLadderGoingUp)
		return;

	if (m_IsChanging)
		return;

	if (!m_Jump && !m_IsFlying)
	{
		std::string CurAnimName = m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName();

		if (CurAnimName == "RightFall" || CurAnimName == "LeftFall")
			return;

		if (m_IsSwimming)
		{
			ChangePlayerSwimAnimation();
		}
		else
		{
			if (m_IsEatingMonster && !m_IsSpecialStateChanged)
				ChangePlayerEatIdleAnimation();
			else
				ChangePlayerNormalIdleAnimation();
		}
	}
}

void CPlayer2D::ChangePlayerNormalIdleAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftIdle");
	else
		ChangeAnimation("RightIdle");
}

void CPlayer2D::ChangePlayerWalkAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	if (!m_Jump && !m_IsFlying)
	{
		std::string CurAnimName = m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName();

		if (CurAnimName == "RightFall" || CurAnimName == "LeftFall")
			return;

		if (m_IsEatingMonster && !m_IsSpecialStateChanged)
		{
			ChangePlayerEatWalkAnimation();
		}
		else
		{
			ChangePlayerNormalWalkAnimation();
		}
	}
}

void CPlayer2D::ChangePlayerNormalWalkAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftWalk");
	else
		ChangeAnimation("RightWalk");
}

void CPlayer2D::ChangePlayerHitAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftHit");
	else
		ChangeAnimation("RightHit");
}

void CPlayer2D::ChangePlayerRunAnimation()
{
	if (m_IsAttacking)
		return;

	if (!m_Jump && !m_IsFlying)
	{
		std::string CurAnimName = m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName();

		if (CurAnimName == "RightFall" || CurAnimName == "LeftFall")
			return;

		if (m_IsEatingMonster && !m_IsSpecialStateChanged)
		{
			ChangePlayerEatRunAnimation();
		}
		else
		{
			ChangePlayerNormalRunAnimation();
		}
	}
}

void CPlayer2D::ChangePlayerNormalRunAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftRun");
	else
		ChangeAnimation("RightRun");
}

void CPlayer2D::ChangePlayerEatRunAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftEatWalk");
	else
		ChangeAnimation("RightEatWalk");
}

void CPlayer2D::ChangePlayerDeathAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftDeath");
	else
		ChangeAnimation("RightDeath");
}

void CPlayer2D::ChangePlayerAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftAttack");
	else
		ChangeAnimation("RightAttack");
}

void CPlayer2D::ChangePlayerFallDownAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftFallAttack");
	else
		ChangeAnimation("RightFallAttack");
}

void CPlayer2D::ChangePlayerGoUpAttackAnimation()
{

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftUpAttack");
	else
		ChangeAnimation("RightUpAttack");
}

void CPlayer2D::ChangePlayerFlyAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftFly");
	else
		ChangeAnimation("RightFly");
}

void CPlayer2D::ChangePlayerJumpAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftJump");
	else
 		ChangeAnimation("RightJump");
}

void CPlayer2D::ChangePlayerFallAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftFall");
	else
		ChangeAnimation("RightFall");
}

void CPlayer2D::ChangePlayerSpitOutAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftSpitOut");
	else
		ChangeAnimation("RightSpitOut");

	// Effect 만들어내기 
}

void CPlayer2D::ChangePlayerSlideAttackAnimation()
{
	if (m_IsChanging)
		return;

	if (m_IsFlying)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftSlide");
	else
		ChangeAnimation("RightSlide");
}

void CPlayer2D::ChangePlayerChangeAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftChange");
	else
		ChangeAnimation("RightChange");
}

void CPlayer2D::ChangePlayerSwimAnimation()
{
	if (!m_IsSwimming)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftSwim");
	else
		ChangeAnimation("RightSwim");
}

void CPlayer2D::ChangePlayerSpecialAttackAnimation(float DeltaTime)
{
	if (!m_KirbyState)
		return;

	if (m_IsAttacking)
		return;

	if (m_IsLadderGoingUp)
		return;

	if (m_IsSwimming)
		return;

	if (m_IsFlying)
		return;

	if (!m_IsSpecialStateChanged)
		return;

	if (m_MP < 25.f)
		return;

	m_IsChanging = true;

	m_IsAttacking = true;

	StopPlayer();

	PrepareSpecialAction(1.f);

	CAnimationSequence2DData* Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightSpecialAttack");

	if (Data)
	{
		Data->SetEndFunction(this, &CPlayer2D::SpecialAttack);
	}

	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftSpecialAttack");

	if (Data)
	{
		Data->SetEndFunction(this, &CPlayer2D::SpecialAttack);
	}

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftSpecialAttack");
	else
		ChangeAnimation("RightSpecialAttack");

	DecreaseMP(25.f);

	// Sword 의 경우, 크기를 잠시 키운다.
	if (m_SpecialAbilityState == Ability_State::Sword)
	{
		m_KirbyState->SetWorldScale(140.f, 140.f, 1.f);
	}
}

void CPlayer2D::ChangePlayerUltimateAttackAnimation(float DeltaTime)
{
	if (!m_KirbyState)
		return;

	if (m_IsAttacking)
		return;

	if (m_IsLadderGoingUp)
		return;

	if (m_IsSwimming)
		return;

	if (m_IsFlying)
		return;

	if (!m_IsSpecialStateChanged)
		return;

	if (m_MP < 99.f)
		return;

	// m_IsChanging = true;

	m_IsAttacking = true;

	WeakJump();

	PrepareUltimateAction(4.f, 1.f);

	CAnimationSequence2DData* Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightSpecialAttack");

	if (Data)
		Data->SetEndFunction(this, &CPlayer2D::UltimateAttack);

	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftSpecialAttack");

	if (Data)
		Data->SetEndFunction(this, &CPlayer2D::UltimateAttack);

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftSpecialAttack");
	else
		ChangeAnimation("RightSpecialAttack");

	DecreaseMP(m_MPMax);

	// Sword 의 경우, 크기를 잠시 키운다.
	if (m_SpecialAbilityState == Ability_State::Sword)
	{
		m_KirbyState->SetWorldScale(140.f, 140.f, 1.f);
	}

	m_PhysicsSimulate = false;

	StopPlayer();

	m_Scene->GetResource()->SoundPlay("PlayerUltimateAttack");
}

void CPlayer2D::ChangePlayerEatIdleAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftEatIdle");
	else
		ChangeAnimation("RightEatIdle");
}

void CPlayer2D::ChangePlayerEatWalkAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_IsChanging)
		return;

	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftEatWalk");
	else
		ChangeAnimation("RightEatWalk");
}

void CPlayer2D::SetObjectLand()
{
	CLifeObject::SetObjectLand();

	m_TriangleJump = false;

	m_IsFlying = false;

	m_MoveDashEffectMade = false;
}

void CPlayer2D::FallDownAttackCallback(const CollisionResult& Result)
{
	Vector3 CurrentPos = GetWorldPos();

	if (m_FallStartY > CurrentPos.y && !m_IsGround && !m_IsSwimming)
	{
		CColliderComponent* DestCollider = Result.Dest;

		CGameObject* OwnerObject = DestCollider->GetGameObject();

		CMonster* OwnerMonster = dynamic_cast<CMonster*>(OwnerObject);

		if (!OwnerMonster)
			return;

		// 2) Sound

		// 3) Damage Font 작성
		OwnerMonster->Damage(m_AttackAbility);

		CWidgetComponent*  ObjectWindow = OwnerMonster->FindComponentByType<CWidgetComponent>();

		if (ObjectWindow)
		{
			CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");

			DamageFont->SetDamage((int)m_AttackAbility);
		}

		// 4) Hit 상태로 변경
		OwnerMonster->SetBeingHit(true);

		OwnerMonster->SetAIState(Monster_AI::Hit);

		// 현재 Player 방향으로 나아가게 하기
		if (m_ObjectMoveDir.x < 0)
		{
			OwnerMonster->SetObjectMoveDir(Vector3(-1.f, 0.f, 0.f));
		}
		else
		{
			OwnerMonster->SetObjectMoveDir(Vector3(1.f, 0.f, 0.f));
		}

		m_Scene->GetResource()->SoundPlay("PlayerDamageMonster");
	}
}

void CPlayer2D::CheckBottomCollision()
{
	if (m_JumpDown)
		return;

	CLifeObject::CheckBottomCollision();
}

void CPlayer2D::PullRight(float DeltaTime)
{
	if (m_IsEatingMonster)
		return;

	if (m_IsBeingHit)
		return;

	if (m_IsSwimming)
		return;

	if (m_IsLadderGoingUp)
		return;

	StopPlayer();

	m_KirbyState->GetAnimationInstance()->ChangeAnimation("RightPull");

	// 오른쪽 범위 안에 있는지 확인하기
	float PullDist = 0.f;

	m_PullRightCollider->Enable(true);

	float XScale = m_PullRightCollider->GetInfo().Length.x;
	float YScale = m_PullRightCollider->GetInfo().Length.y;

	if (XScale < m_PullColliderXMaxScale)
	{
		XScale += DeltaTime * 300.f;
	}
	if (YScale < m_PullColliderYMaxScale)
	{
		YScale += DeltaTime * 300.f;
	}

	m_PullRightCollider->SetExtend(XScale, YScale);

	m_IsPulling = true;

	m_ObjectMoveDir.x = 1.f;

	m_PullRandomStarMakeTime += DeltaTime;

	Vector3 WorldPos = GetWorldPos();
	float LeftXPos = GetWorldPos().x - GetWorldScale().x * GetPivot().x;
	float RightXPos = GetWorldPos().x + GetWorldScale().x * GetPivot().x;

	if (m_PullRandomStarMakeTime >= m_PullRandomStarMakeTimeMax)
	{
		m_PullRandomStarMakeTime = 0.f;

		CEffectRandomStar* PullStar = m_Scene->CreateGameObject<CEffectRandomStar>("Star");

		PullStar->SetAttackXDir(1.f);

		PullStar->SetWorldPos(GetWorldPos().x + GetWorldScale().x * GetPivot().x, GetWorldPos().y, 0.f);
	}

	m_PullSoundTime += DeltaTime;

	if (m_PullSoundTime >= m_PullSoundTimeMax)
	{
		m_PullSoundTime = 0.f;
		m_Scene->GetResource()->SoundPlay("PlayerPull");
	}
}

void CPlayer2D::PullRightCollisionBeginCallback(const CollisionResult& Result)
{
	CColliderComponent* CollisionDest = Result.Dest;

	if (m_PullingMonster)
		return;

	if (m_IsEatingMonster)
		return;

	if (!m_IsPulling)
		return;

	CMonster* DestMonster = dynamic_cast<CMonster*>(CollisionDest->GetGameObject());

	// 현재 끌어당기는 대상이 Monster 라면
	if (DestMonster && DestMonster->GetMonsterType() != Monster_Type::Boss)
	{
		m_PullingMonster = DestMonster;

		if (!DestMonster->IsBeingPulled())
		{
			DestMonster->SetIsBeingPulled(true);
			DestMonster->SetBeingHit(true);
			DestMonster->SetAIState(Monster_AI::Hit);
			DestMonster->SetCollisionDisable(true);
			DestMonster->SetPulledDestPos(GetWorldPos());
		}
	}
	// 그게 아니라면 EffectStart 여야 한다. 자기가 내뱉은 Start
	else 
	{
		CEffectStar* EffectStar = dynamic_cast<CEffectStar*>(CollisionDest->GetGameObject());

		if (EffectStar)
		{
			// Ability Monster를 내뱉은 Star가 아니라면
			if (EffectStar->IsSpecialKirbyStar() == false)
				return;

			EffectStar->SetIsBeingPulled(true);

			m_PullingStar = EffectStar;
		}
	}
}

void CPlayer2D::PullRightCollisionEndCallback(const CollisionResult& Result)
{
	if (m_PullingMonster)
	{
		m_PullingMonster->SetIsBeingPulled(false);
		m_PullingMonster->ResetPulledInfo();
		m_PullingMonster->SetBeingHit(false);
		m_PullingMonster->SetCollisionDisable(false);
		m_PullingMonster = nullptr;
	}
	else if (m_PullingStar)
	{
		m_PullingStar->SetIsBeingPulled(false);
		m_PullingStar = nullptr;
	}
}

void CPlayer2D::PullLeftCollisionEndCallback(const CollisionResult& Result)
{
	if (m_PullingMonster)
	{
		m_PullingMonster->SetIsBeingPulled(false);
		m_PullingMonster->ResetPulledInfo();
		m_PullingMonster->SetCollisionDisable(false);
		m_PullingMonster->SetBeingHit(false);
		m_PullingMonster = nullptr;
	}
	else if (m_PullingStar)
	{
		m_PullingStar->SetIsBeingPulled(false);
		m_PullingStar = nullptr;
	}
}

void CPlayer2D::ChangeToAfterWardsAnimationAfterSpitOut()
{
	if (m_FallStartY - GetWorldPos().y > 0.f || m_IsFalling)
	{
		ChangePlayerFallAnimation();
	}
	else
	{
		ChangePlayerIdleAnimation();
	}
}

void CPlayer2D::SetEatenMonsterAsPrevEatenMonster()
{
	m_EatenMonster = m_PrevAbilityEatenMonster;

	m_IsEatingMonster = true;

	// Animation Change
	ChangePlayerIdleAnimation();
}

void CPlayer2D::SpecialChange()
{
	CAbilityMonster* AbilityMonster = (CAbilityMonster*)m_EatenMonster.Get();

	Ability_State State = AbilityMonster->GetAbilityState();

	Vector3 BeforeChangePos = GetWorldPos();

	// Set Kirby State
	switch (AbilityMonster->GetAbilityState())
	{
	case Ability_State::Beam:
	{
		m_KirbyState = nullptr;

		m_KirbyState = CreateComponent<CBeamKirbyState>("BeamKirbyState");

		m_KirbyState->SetWorldScale(85.f, 95.f, 1.f);

		m_KirbyState->SetInitWorldScale(Vector3(85.f, 95.f, 1.f));

		m_SpecialAbilityState = Ability_State::Beam;
	}
	break;
	case Ability_State::Fire:
	{
		m_KirbyState = nullptr;

		m_KirbyState = CreateComponent<CFireKirbyState>("FireKirbyState");

		m_KirbyState->SetWorldScale(85.f, 100.f, 1.f);

		m_SpecialAbilityState = Ability_State::Fire;
	}
	break;
	case Ability_State::Fight:
	{
		m_KirbyState = nullptr;

		m_KirbyState = CreateComponent<CFightKirbyState>("FightKirbyState");

		m_KirbyState->SetWorldScale(90.f, 90.f, 1.f);

		m_SpecialAbilityState = Ability_State::Fight;
	}
	break;
	case Ability_State::Bomb:
	{
		m_KirbyState = nullptr;

		m_KirbyState = CreateComponent<CBombKirbyState>("BombKirbyState");

		m_KirbyState->SetWorldScale(77.f, 77.f, 1.f);

		m_SpecialAbilityState = Ability_State::Bomb;
	}
	break;
	case Ability_State::Sword:
	{
		m_KirbyState = nullptr;

		m_KirbyState = CreateComponent<CSwordKirbyState>("SwordKirbyState");

		m_KirbyState->SetWorldScale(80.f, 100.f, 1.f);

		m_SpecialAbilityState = Ability_State::Sword;
	}
	break;
	}

	m_IsSpecialStateChanged = true;

	// Object Pool 만들기
	auto iter = m_mapObjectPool.find(m_SpecialAbilityState);

	if (iter == m_mapObjectPool.end())
	{
		CKirbyAttackObjectPool* ObjectPool = m_Scene->CreateGameObject<CKirbyAttackObjectPool>("AttackObjectPool");
		ObjectPool->m_PlayerOwner = this;
		ObjectPool->SetPoolAbilityState(State);
		m_mapObjectPool.insert(std::make_pair(m_SpecialAbilityState, ObjectPool));
	}

	InitObjectPoolSetting();

	// Set UI Info
	SetUIAccordingToKirbyState();

	// Set Kirby State Scale
	SetKirbyStateSizeAccordingToKirbyState();

	// 아래 코드는 그대로 둔다. --> 변화 이후에도, 현재 먹고 있던 Star 는 다시 내뱉을 수 있어야 하기 때문이다.
	// m_IsEatingMonster = false;

	m_KirbyState->SetPlayer(this);

	SpecialChangeEffect();

	SetRootComponent(m_KirbyState);

	SetBasicSettingToChangedState();

	m_KirbyState->SetScene(m_Scene);

	m_KirbyState->SetWorldPos(BeforeChangePos);

	m_KirbyState->AddChild(m_Camera);
	m_KirbyState->AddChild(m_SimpleHUDWidget);
	m_KirbyState->AddChild(m_Body);
	m_KirbyState->AddChild(m_PullRightCollider);
	m_KirbyState->AddChild(m_PullLeftCollider);

	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	m_KirbyState->SetTransparency(true);

	m_KirbyState->SetPivot(0.5f, 0.5f, 0.f);

	m_IsChanging = false;

	// Player 외 모든 Object 들을 멈춘다.
	UndoSpecialAction();

	// 다시 중력 적용
	m_PhysicsSimulate = true;

	ChangePlayerIdleAnimation();

	WeakJump();

	m_Scene->GetResource()->SoundPlay("PlayerChange");
}

void CPlayer2D::SpecialChangeStart(float DeltaTime)
{
	// 먹고 있는 녀석이 없으면 X
	if (!m_IsEatingMonster)
		return;

	if (m_IsBeingHit)
		return;

	// 능력 몬스터가 아니라면 Return
	if (m_EatenMonster->GetMonsterType() != Monster_Type::Ability)
		return;

	if (m_IsSpecialStateChanged)
		return;

	m_IsChanging = true;

	SpecialChangeEffect();

	ChangePlayerChangeAnimation();

	StopPlayer();

	PrepareSpecialAction(m_ChangeTimeMax);

	m_Scene->GetResource()->SoundPlay("PlayerChange");

	// 원인은 모르지만, UltimateAttack 으로 맨 처음 Animation을 Change 한 이후에
	// m_IsGround가 false로 세팅되고, 계속 내려가게 된다.
	// 이를 방지하기 위해, UltimateAttack Animation 변환시
	// m_PhysicsSimulate 를 false 로 하고
	// 여기서 다시 원상태로 세팅
	m_PhysicsSimulate = false;
}

void CPlayer2D::SetBasicSettingToChangedState()
{
	// LeftSwim
	CAnimationSequence2DData* Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftSwim");
	if (Data)
	{
		Data->SetLoop(true);
	}

	// Right Swim
	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightSwim");

	if (Data)
	{
		Data->SetLoop(true);
	}

	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetEndFunction(
		this, &CPlayer2D::ChangePlayerFallAnimation);
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetEndFunction(
		this, &CPlayer2D::ChangePlayerFallAnimation);

	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightSpitOut")->SetPlayTime(0.2f);
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftSpitOut")->SetPlayTime(0.2f);

	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightSpitOut")->SetEndFunction(
		this, &CPlayer2D::ChangeToAfterWardsAnimationAfterSpitOut);
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftSpitOut")->SetEndFunction(
		this, &CPlayer2D::ChangeToAfterWardsAnimationAfterSpitOut);


	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightSpecialAttack");

	if (Data)
	{
		Data->SetEndFunction(this, &CPlayer2D::SpecialAttack);
	}

	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftSpecialAttack");

	if (Data)
	{
		Data->SetEndFunction(this, &CPlayer2D::SpecialAttack);
	}

	// Animation Play Scale 세팅
	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightEatIdle");

	if (Data)
	{
		Data->SetPlayTime(2.f);
	}

	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftEatIdle");

	if (Data)
	{
		Data->SetPlayTime(2.f);
	}

	// RightChange
	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightChange");

	if (Data)
	{
		Data->SetLoop(false);
		Data->SetPlayTime(m_ChangeTimeMax);
		Data->SetEndFunction(
			this, &CPlayer2D::SpecialChange);
	}

	// LeftChange
	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftChange");

	if (Data)
	{
		Data->SetLoop(false);
		Data->SetPlayTime(m_ChangeTimeMax);
		Data->SetEndFunction(
			this, &CPlayer2D::SpecialChange);
	}

	// Slide Attack
	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightSlide");

	if (Data)
	{
		Data->SetPlayTime(0.8f);
		Data->SetEndFunction(this, &CPlayer2D::ChangePlayerIdleAnimation);
	}

	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftSlide");

	if (Data)
	{
		Data->SetPlayTime(0.8f);
		Data->SetEndFunction(this, &CPlayer2D::ChangePlayerIdleAnimation);
	}

	// Pull
	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightPull");

	if (Data)
	{
		Data->SetEndFunction(this, &CPlayer2D::ChangePlayerIdleAnimation);
	}

	Data = m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftPull");

	if (Data)
	{
		Data->SetEndFunction(this, &CPlayer2D::ChangePlayerIdleAnimation);
	}
}

void CPlayer2D::SpecialChangeEffect()
{
	/*
	CSpecialChangeParticle* SpecialChangeParticle = m_Scene->CreateGameObject<CSpecialChangeParticle>("Special Change");
	SpecialChangeParticle->SetWorldPos(GetWorldPos());
	*/

	// Random Star Effect
	for (int i = 0; i < 10; i++)
	{
		CEffectRandomStar* RandomStar = m_Scene->CreateGameObject<CEffectRandomStar>("RandomStar");
		RandomStar->SetWorldPos(Vector3(GetWorldPos().x, GetWorldPos().y, GetWorldPos().z));
	}
}

void CPlayer2D::UpdateChangeState(float DeltaTime)
{
	/*
	if (m_IsChanging)
	{
		if (m_ChangeTime > 0)
		{
			m_ChangeTime -= DeltaTime;
		}

		if (m_ChangeTime < 0)
		{
			// 여기에 도달하기 전에, 이미 Change 가 시작된 상태여서
			// m_IsChanging이 false가 되어야 한다.
			// 그런데 혹시나, 바뀌지 않았을 경우
			if (m_IsChanging)
			{
				SpecialChange();
			}
		}
	}
	*/
}

void CPlayer2D::LoadChangeImagesInAdvance()
{
	// Ultimate Attack
	m_Scene->GetResource()->LoadTexture("UltimateBombUI", TEXT("Project/UI/Bomb_Ultimate.png"));
	m_Scene->GetResource()->LoadTexture("UltimateBeamUI", TEXT("Project/UI/Beam_Ultimate.png"));
	m_Scene->GetResource()->LoadTexture("UltimateFightUI", TEXT("Project/UI/Fight_Ultimate.png"));
	m_Scene->GetResource()->LoadTexture("UltimateFireUI", TEXT("Project/UI/Fire_Ultimate.png"));
	m_Scene->GetResource()->LoadTexture("UltimateSwordUI", TEXT("Project/UI/Sword_Ultimate.png"));

	// Ultimate Attack
	m_Scene->GetResource()->LoadTexture("UltimateBlueAlpha", TEXT("Project/UI/UltimateBlue.jpg"));
	m_Scene->GetResource()->LoadTexture("UltimatePurpleAlpha", TEXT("Project/UI/UltimatePurple.jpg"));
	m_Scene->GetResource()->LoadTexture("UltimateOrangeAlpha", TEXT("Project/UI/UltimateOrange.jpg"));
	m_Scene->GetResource()->LoadTexture("UltimateRedAlpha", TEXT("Project/UI/UltimateRed.jpg"));
	m_Scene->GetResource()->LoadTexture("UltimateYellowAlpha", TEXT("Project/UI/UltimateYellow.jpg"));
}

void CPlayer2D::Attack(float DeltaTime)
{
	if (!m_KirbyState)
		return;

	if (m_IsAttacking && !m_AttackReady)
		return;

	if (m_IsLadderGoingUp)
		return;

	if (m_IsPulling)
		return;

	if (m_IsChanging)
		return;

	if (m_IsSwimming)
	{
		m_IsAttacking = true;

		CEffectWaterAttack* WaterAttack = m_Scene->CreateGameObject<CEffectWaterAttack>("Attack");

		if (m_ObjectMoveDir.x < 0.f)
		{
			WaterAttack->SetLeftAttackDir();

			WaterAttack->SetWorldPos(Vector3(GetWorldPos().x - GetWorldScale().x * GetPivot().x,
				GetWorldPos().y, GetWorldPos().z));

			WaterAttack->GetColliderBody()->AddCollisionCallback(Collision_State::Begin,
				this, &CPlayer2D::PlayerAttackCollisionCallback);
		}
		else
		{
			WaterAttack->SetRightAttackDir();

			WaterAttack->SetWorldPos(Vector3(GetWorldPos().x + GetWorldScale().x * GetPivot().x,
				GetWorldPos().y, GetWorldPos().z));

			WaterAttack->GetColliderBody()->AddCollisionCallback(Collision_State::Begin,
				this, &CPlayer2D::PlayerAttackCollisionCallback);
		}

		m_Scene->GetResource()->SoundPlay("PlayerNormalAttack");
	}
	else
	{
		if (!m_IsSpecialStateChanged)
			return;

		m_IsAttacking = true;

		ResetMoveInfo();

		StopPlayer();

		float YDiff = GetWorldPos().y - m_PrevPos.y;

		// 내려가고 있을 때
		if (m_IsFalling && (!m_Jump || m_Bounced) && !m_IsFlying)
		{
			if (m_MP < 15.f)
				return;

			ChangePlayerFallDownAttackAnimation();

			DecreaseMP(15.f);

			m_KirbyState->FallDownAttack();

			m_Scene->GetResource()->SoundPlay("PlayerFallDownAttack");
		}
		else if (YDiff > 0)
		{
			if (m_MP < 15.f)
				return;

			ChangePlayerGoUpAttackAnimation();

			DecreaseMP(15.f);

			m_KirbyState->GoUpAttack();

			m_Scene->GetResource()->SoundPlay("PlayerGoUpAttack");
		}
		else
		{
			// ChangePlayerAttackAnimation();

			m_KirbyState->Attack();

			m_Scene->GetResource()->SoundPlay("PlayerNormalAttack");
		}
	}
}

void CPlayer2D::SlideAttack(float DeltaTime)
{
	if (m_IsSwimming)
		return;

	if (m_IsLadderGoingUp)
		return;

	if (m_IsSwimming)
		return;

	if (m_IsFlying)
		return;

	if (m_IsFalling)
		return;

	if (m_IsAttacking)
		return;

	m_IsAttacking = true;

	m_SlideAttack = true;

	m_SlideAttackTime = m_SlideAttackTimeMax;

	m_SlideAttackSpeed = m_SlideAttackSpeedMax;

	ChangePlayerSlideAttackAnimation();

	m_Scene->GetResource()->SoundPlay("PlayerSlide");
}

void CPlayer2D::MakePlayerCloneEffect()
{
	// 현재 Animation 의 Time 을 알아낸다
	float CurrentFrameTime = m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrameTime();

	CPlayerClone* KirbyClone = m_Scene->CreateGameObject<CPlayerClone>("KirbyClone");

	if (m_IsSpecialStateChanged)
	{
		KirbyClone->SetKirbyState(m_SpecialAbilityState);
	}

	KirbyClone->SetWorldScale(m_KirbyState->GetWorldScale());

	const std::string& CurAnimName = m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName();

	// KirbyClone->GetAnimationInstance()->SetCurrentAnimation(CurAnimName);
	KirbyClone->GetAnimationInstance()->ChangeAnimation(CurAnimName);

	KirbyClone->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrameTime(CurrentFrameTime);

	KirbyClone->SetLifeTime(0.4f);

	// KirbyClone->SetObjectMoveSpeed(m_SlideAttackSpeed - 50.f);

	KirbyClone->SetObjectMoveDir(m_ObjectMoveDir.x);

	KirbyClone->SetOpacityDecreaseTime(0.4f);
	

	if (m_ObjectMoveDir.x < 0.f)
	{
		KirbyClone->SetWorldPos(GetWorldPos().x + 20.f, GetWorldPos().y, GetWorldPos().z);
	}
	else
	{
		KirbyClone->SetWorldPos(GetWorldPos().x - 20.f, GetWorldPos().y, GetWorldPos().z);
	}

	m_Scene->GetResource()->SoundPlay("PlayerCloneEffect");
}

void CPlayer2D::SpecialAttack()
{
	m_IsChanging = false;

	ResetMoveInfo();

	UndoSpecialAction();

	m_KirbyState->SpecialAttack();

	m_Scene->GetResource()->SoundPlay("PlayerSpecialAttack");
}

void CPlayer2D::UltimateAttack()
{
	// 원인은 모르지만, UltimateAttack 으로 맨 처음 Animation을 Change 한 이후에
	// m_IsGround가 false로 세팅되고, 계속 내려가게 된다.
	// 이를 방지하기 위해, UltimateAttack Animation 변환시
	// m_PhysicsSimulate 를 false 로 하고
	// 여기서 다시 원상태로 세팅
	m_PhysicsSimulate = true;

	ResetMoveInfo();

	UndoUltimateAction();
	
	SpecialChangeEffect();

	// 카메라 흔들림
	m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect(true);

	m_KirbyState->UltimateAttack();

	m_IsAttacking = false;

	m_IsChanging = false;

	ChangePlayerNormalIdleAnimation();

	m_Scene->GetResource()->SoundPlay("PlayerUltimateAttack");
}

void CPlayer2D::PrepareSpecialAction(float PrepareTime)
{
	m_Scene->SetStopEnableObjectsExceptPlayer(GetTypeID(), true);

	m_ChangeBlackBackGround = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	m_ChangeBlackBackGround->SetBlackTexture();

	m_ChangeBlackBackGround->SetLifeTime(PrepareTime);

	m_ChangeBlackBackGround->SetOpacity(0.7f);

	m_ChangeBlackBackGround->SetMaintainOpacity(true);

	// 화면 가장 앞에 보이게 세팅한다.
	m_RootComponent->SetLayerName("PlayerChange");

	m_ChangeTime = m_ChangeTimeMax + 0.001f;
}

void CPlayer2D::UndoSpecialAction()
{
	m_Scene->SetStopEnableObjectsExceptPlayer(GetTypeID(), false);

	m_RootComponent->SetLayerName("Default");
}

void CPlayer2D::PrepareUltimateAction(float BackGroundDestroyTime, float UIProceedTime)
{
	m_Scene->SetStopEnableObjectsExceptPlayer(GetTypeID(), true);

	// m_ChangeBlackBackGround = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");
	
	// m_ChangeBlackBackGround->SetUltimateAttackTexture(m_SpecialAbilityState);
	
	// m_ChangeBlackBackGround->SetApplyDecreaseDestroy(true);
	
	// m_ChangeBlackBackGround->SetStartDestroyTime(BackGroundDestroyTime);
	
	// m_ChangeBlackBackGround->SetOpacity(0.6f);
	
	// m_ChangeBlackBackGround->SetMaintainOpacity(true);

	// 화면 가장 앞에 보이게 세팅한다.
	m_RootComponent->SetLayerName("PlayerChange");

	m_ChangeTime = m_ChangeTimeMax + 0.001f;

	// 필살기라면, UI 도 세팅한다.
	m_UltimateAttackWindow = m_Scene->GetViewPort()->CreateUIWindow<CUltimateAttackWidget>("UltimateAttackWindow");

	switch (m_SpecialAbilityState)
	{
	case Ability_State::Bomb:
	{
		m_UltimateAttackWindow->SetUITexture("UltimateBombUI", TEXT("Project/UI/Bomb_Ultimate.png"));
	}
	break;
	case Ability_State::Beam:
	{
		m_UltimateAttackWindow->SetUITexture("UltimateBeamUI", TEXT("Project/UI/Beam_Ultimate.png"));
	}
	break;
	case Ability_State::Fight:
	{
		m_UltimateAttackWindow->SetUITexture("UltimateFightUI", TEXT("Project/UI/Fight_Ultimate.png"));
	}
	break;
	case Ability_State::Fire:
	{
		m_UltimateAttackWindow->SetUITexture("UltimateFireUI", TEXT("Project/UI/Fire_Ultimate.png"));
	}
	break;
	case Ability_State::Sword:
	{
		m_UltimateAttackWindow->SetUITexture("UltimateSwordUI", TEXT("Project/UI/Sword_Ultimate.png"));
	}
	break;
	}

	m_UltimateAttackWindow->SetUIProceedTime(UIProceedTime);
}

void CPlayer2D::UndoUltimateAction()
{
	m_Scene->SetStopEnableObjectsExceptPlayer(GetTypeID(), false);

	m_RootComponent->SetLayerName("Default");
}

void CPlayer2D::ChangeToIdleAfterAnimation(float DeltaTime)
{
	ChangePlayerIdleAnimation();
}

void CPlayer2D::SetUIAccordingToKirbyState()
{
	if (!m_IsSpecialStateChanged)
	{
		return;
	}

	CPlayerHUD* PlayerHUD = (CPlayerHUD*)m_Scene->GetPlayerHUD();

	if (!PlayerHUD)
		return;

	switch (m_SpecialAbilityState)
	{
	case Ability_State::Beam:
	{
		PlayerHUD->GetNameText()->SetText(TEXT("BEAM KIRBY"));

		PlayerHUD->GetIconImage()->SetTexture("BeamKirbyIconTexture", TEXT("Project/UI/Icon_BeamKirby.png"));

		PlayerHUD->GetIconImage()->SetSize(50.f, 50.f);
	}
	break;
	case Ability_State::Fire:
	{
		PlayerHUD->GetNameText()->SetText(TEXT("FIRE KIRBY"));

		PlayerHUD->GetIconImage()->SetTexture("FireKirbyIconTexture", TEXT("Project/UI/Icon_FireKirby.png"));

		PlayerHUD->GetIconImage()->SetSize(50.f, 50.f);
	}
	break;
	case Ability_State::Fight:
	{
		PlayerHUD->GetNameText()->SetText(TEXT("FIGHT KIRBY"));

		PlayerHUD->GetIconImage()->SetTexture("FightKirbyIconTexture", TEXT("Project/UI/Icon_FightKirby.png"));

		PlayerHUD->GetIconImage()->SetSize(50.f, 50.f);
	}
	break;
	case Ability_State::Bomb:
	{
		PlayerHUD->GetNameText()->SetText(TEXT("BOMB KIRBY"));

		PlayerHUD->GetIconImage()->SetTexture("BombKirbyIconTexture", TEXT("Project/UI/Icon_BombKirby.png"));

		PlayerHUD->GetIconImage()->SetSize(50.f, 50.f);
	}
	break;
	case Ability_State::Sword:
	{
		PlayerHUD->GetNameText()->SetText(TEXT("SWORD KIRBY"));

		PlayerHUD->GetIconImage()->SetTexture("SwordKirbyIconTexture", TEXT("Project/UI/Icon_SwordKirby.png"));

		PlayerHUD->GetIconImage()->SetSize(50.f, 50.f);
	}
	break;
	}
}

void CPlayer2D::SetKirbyStateSizeAccordingToKirbyState()
{
	if (!m_IsSpecialStateChanged)
	{
		m_KirbyState->SetWorldScale(80.f, 80.f, 1.f);

		return;
	}

	switch (m_SpecialAbilityState)
	{
	case Ability_State::Beam:
	{
		m_KirbyState->SetWorldScale(85.f, 95.f, 1.f);
	}
	break;
	case Ability_State::Fire:
	{
		m_KirbyState->SetWorldScale(85.f, 100.f, 1.f);
	}
	break;
	case Ability_State::Fight:
	{
		m_KirbyState->SetWorldScale(95.f, 95.f, 1.f);
	}
	break;
	case Ability_State::Bomb:
	{
		m_KirbyState->SetWorldScale(77.f, 77.f, 1.f);
	}
	break;
	case Ability_State::Sword:
	{
		m_KirbyState->SetWorldScale(80.f, 105.f, 1.f);
	}
	break;
	}
}

void CPlayer2D::PlayerAttackCollisionCallback(const CollisionResult& Result)
{
	if (m_IsAttacking)
	{
		CGameObject* Owner = Result.Dest->GetGameObject();

		CWidgetComponent* ObjectWindow = nullptr;

		if (Owner)
		{
			CMonster* DestMonster = dynamic_cast<CMonster*>(Owner);

			if (!DestMonster)
				return;

			// HP Bar 달게 하기
			DestMonster->Damage(m_AttackAbility);

			m_Scene->GetResource()->SoundPlay("PlayerDamageMonster");

			DestMonster->SetBeingHit(true);

			DestMonster->SetAIState(Monster_AI::Hit);

			if (m_ObjectMoveDir.x < 0)
				DestMonster->SetObjectMoveDir(Vector3(-1.f, 0.f, 0.f));
			else
				DestMonster->SetObjectMoveDir(Vector3(1.f, 0.f, 0.f));

			// DestMonster->Damage(2.f);

			// Create Damage Font
			ObjectWindow = Owner->FindComponentByType<CWidgetComponent>();

			if (ObjectWindow)
			{
				CUIDamageFont* DamageFont = ObjectWindow->GetWidgetWindow()->CreateUIWidget<CUIDamageFont>("DamageFont");

				DamageFont->SetDamage((int)m_AttackAbility);
			}
		}

		m_IsAttacking = false;

		// Slide Attack 중이었다면 반대 방향으로
		if (m_SlideAttack)
		{
			SimpleJump();
		}
	}
}

void CPlayer2D::PullLeftCollisionBeginCallback(const CollisionResult& Result)
{
	CColliderComponent* CollisionDest = Result.Dest;

	if (m_PullingMonster)
		return;

	if (m_IsEatingMonster)
		return;

	if (!m_IsPulling)
		return;

	CMonster* DestMonster = dynamic_cast<CMonster*>(CollisionDest->GetGameObject());

	// 현재 끌어당기는 대상이 Monster 라면
	if (DestMonster)
	{
		m_PullingMonster = DestMonster;

		if (!DestMonster->IsBeingPulled() && DestMonster->GetMonsterType() != Monster_Type::Boss)
		{
			DestMonster->SetIsBeingPulled(true);
			DestMonster->SetBeingHit(true);
			DestMonster->SetAIState(Monster_AI::Hit);
			DestMonster->SetCollisionDisable(true);
			DestMonster->SetPulledDestPos(GetWorldPos());
		}
	}
	// 그게 아니라면 EffectStart 여야 한다. 자기가 내뱉은 Start
	else
	{
		CEffectStar* EffectStar = dynamic_cast<CEffectStar*>(CollisionDest->GetGameObject());

		if (EffectStar)
		{
			// Ability Monster를 내뱉은 Star가 아니라면
			if (EffectStar->IsSpecialKirbyStar() == false)
				return;

			EffectStar->SetIsBeingPulled(true);

			m_PullingStar = EffectStar;
		}
	}
}

void CPlayer2D::ChangeAnimation(const std::string& AnimName)
{
	if (!m_KirbyState)
		return;
	if (!m_KirbyState->GetAnimationInstance())
		return;
	m_KirbyState->GetAnimationInstance()->ChangeAnimation(AnimName);
}

void CPlayer2D::PullRightEnd(float DeltaTime)
{
	m_RightMovePush = false;

	m_PullRightCollider->Enable(false);

	if (m_PullingMonster)
	{
		m_PullingMonster->SetIsBeingPulled(false);
		m_PullingMonster->ResetPulledInfo();
		m_PullingMonster->SetCollisionDisable(false);
		m_PullingMonster = nullptr;
	}

	m_PullRightCollider->SetExtend(m_PullColliderXInitScale, m_PullColliderYInitScale);

	ChangePlayerIdleAnimation();

	m_IsPulling = false;
}

void CPlayer2D::PullLeft(float DeltaTime)
{
	if (m_IsEatingMonster)
		return;

	if (m_IsBeingHit)
		return;

	if (m_IsSwimming)
		return;

	if (m_IsLadderGoingUp)
		return;

	StopPlayer();

	m_KirbyState->GetAnimationInstance()->ChangeAnimation("LeftPull");

	// 오른쪽 범위 안에 있는지 확인하기
	float PullDist = 0.f;

	// 충돌체의 크기를 늘려볼까 ?
	m_PullLeftCollider->Enable(true);

	float XScale = m_PullLeftCollider->GetInfo().Length.x;
	float YScale = m_PullLeftCollider->GetInfo().Length.y;

	if (XScale < m_PullColliderXMaxScale)
	{
		XScale += DeltaTime * 300.f;
	}
	if (YScale < m_PullColliderYMaxScale)
	{
		YScale += DeltaTime * 300.f;
	}

	m_PullLeftCollider->SetExtend(XScale, YScale);

	m_IsPulling = true;

	m_ObjectMoveDir.x = 1.f * -1;

	m_PullRandomStarMakeTime += DeltaTime;

	if (m_PullRandomStarMakeTime >= m_PullRandomStarMakeTimeMax)
	{
		m_PullRandomStarMakeTime = 0.f;

		CEffectRandomStar* PullStar = m_Scene->CreateGameObject<CEffectRandomStar>("Star");

		PullStar->SetAttackXDir(-1.f);

		PullStar->SetWorldPos(GetWorldPos().x - GetWorldScale().x * GetPivot().x, GetWorldPos().y, 0.f);
	}

	m_PullSoundTime += DeltaTime;

	if (m_PullSoundTime >= m_PullSoundTimeMax)
	{
		m_PullSoundTime = 0.f;
		m_Scene->GetResource()->SoundPlay("PlayerPull");
	}
}

void CPlayer2D::PullLeftEnd(float DeltaTime)
{
	m_LeftMovePush = false;

	m_PullLeftCollider->Enable(false);

	if (m_PullingMonster)
	{
		m_PullingMonster->SetIsBeingPulled(false);
		m_PullingMonster->SetCollisionDisable(false);
		m_PullingMonster->ResetPulledInfo();
		m_PullingMonster = nullptr;
	}

	m_PullLeftCollider->SetExtend(m_PullColliderXInitScale, m_PullColliderYInitScale);

	ChangePlayerIdleAnimation();

	m_IsPulling = false;
}

void CPlayer2D::MovePointDown(float DeltaTime)
{
	Vector2 MousePos = CInput::GetInst()->GetMouseWorld2DPos();

	Move(Vector3(MousePos.x, MousePos.y, 0.f));
}

void CPlayer2D::Skill1(float DeltaTime)
{
	/*
	CBulletCamera* Bullet = m_Scene->CreateGameObject<CBulletCamera>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot());
	*/
}
