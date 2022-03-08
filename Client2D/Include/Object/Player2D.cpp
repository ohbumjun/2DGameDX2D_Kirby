#include "Player2D.h"

#include <Scene/CameraManager.h>

#include "Bullet.h"
#include "BulletTornaido.h"
#include "Input.h"
#include "PlayerAnimation2D.h"
#include "Resource/Material/Material.h"
#include "Scene/Scene.h"
#include "../Object/Monster.h"
#include "Component/ColliderBox2D.h"
#include "Component/CameraComponent.h"
#include "Component/TileEmptyComponent.h"
#include "BulletCamera.h"
#include "Scene/NavigationManager.h"
#include "../UI/SimpleHUD.h"

CPlayer2D::CPlayer2D():
m_MoveVelocity(0.f),
m_LeverMoveAccel(2.f),
m_LeverVelocity(0.f),
m_LeverMaxMoveVelocity(350.f),
m_DashMoveAccel(2.5f),
m_DashVelocity(0.f),
m_DashMaxMoveVelocity(250.f),
m_TriangleJumpVelocityRatio(0.8f),
m_RightMove(false),
m_ToLeftWhenRightMove(false),
m_RightMovePush(false),
m_LeftMove(false),
m_ToRightWhenLeftMove(false),
m_LeftMovePush(false),
m_IsLeverMoving(false),
m_IsDashMoving(false),
m_TriangleJump(false),
m_IsEatingMonster(false),
m_DeltaTime(0.f),
m_IsFlying(false),
m_FlySpeed(300.f),
m_PullDistance(100.f)
{
	SetTypeID<CPlayer2D>();
	m_SolW      = false;
	m_WDistance = 0.f;
	m_Opacity   = 1.f;

	m_MoveVelocityMax = m_LeverMaxMoveVelocity + m_DashMaxMoveVelocity;
	 
}

CPlayer2D::CPlayer2D(const CPlayer2D& obj) :
	CLifeObject(obj)
{
	m_KirbyState           = dynamic_cast<CKirbyState*>(FindComponent("PlayerSprite"));
	m_Body = (CColliderBox2D*)FindComponent("Body");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_SimpleHUDWidget = (CWidgetComponent*)FindComponent("SimpleHUD");

	m_NavAgent = dynamic_cast<CNavAgent*>(FindComponent("NavAgent"));

	m_Opacity = obj.m_Opacity;
}

CPlayer2D::~CPlayer2D()
{
}

bool CPlayer2D::Init()
{
	m_KirbyState  = CreateComponent<CNormalKirbyState>("PlayerSprite");
	// m_KirbyState  = CreateComponent<CFightKirbyState>("PlayerSprite");
	
	// Collider 
	m_Body = CreateComponent<CColliderBox2D>("Body");
	m_Body->SetCollisionProfile("Player");

	// Widget
	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");
	m_SimpleHUDWidget->CreateUIWindow<CSimpleHUD>("SimpleHUDWidget");

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

	// CAnimationSequence2DInstance* AnimationInstance =  m_Scene->GetResource()->LoadAnimationInstance("Kirby_Fight", TEXT("Kirby_Fight.anim"));
	//
	// m_KirbyState->SetAnimationInstance(AnimationInstance);
	
	// Pivot 값이 없다면, 원래의 pos 인 왼쪽 하단 pos 를 중심으로
	// Center 가 형성되게 될 것이다. 
	m_KirbyState->SetRelativeScale(100.f, 100.f, 1.f);
	m_KirbyState->SetRelativePos(100.f, 50.f, 0.f);
	m_KirbyState->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CPlayer2D::Start()
{
	CLifeObject::Start();


	m_KirbyState = dynamic_cast<CKirbyState*>(FindComponent("PlayerSprite"));

	SetRootComponent(m_KirbyState);
	
	m_Body = (CColliderBox2D*)FindComponent("Body");
	m_Body->AddCollisionCallback(Collision_State::Begin, this, &CPlayer2D::FallDownAttack);

	m_PullRightCollider = CreateComponent<CColliderBox2D>("PullRightCollider");
	m_PullRightCollider->SetCollisionProfile("Player");
	m_PullRightCollider->Start(); // SceneCollision 의 Collider List 에 추가하기
	m_PullRightCollider->Enable(false);
	m_PullRightCollider->AddCollisionCallback(Collision_State::Begin, this, &CPlayer2D::PullRightCollisionBeginCallback);
	m_PullRightCollider->AddCollisionCallback(Collision_State::End, this, &CPlayer2D::PullRightCollisionEndCallback);
	m_PullRightCollider->SetExtend((m_PullDistance + m_Body->GetWorldScale().x * 1.5f) * 0.5f,
		(m_KirbyState->GetWorldScale().y * 0.5f));
	m_PullRightCollider->SetRelativePos(m_Body->GetWorldScale().x * 1.5f * 0.5f, 0.f, 1.f);

	m_PullLeftCollider = CreateComponent<CColliderBox2D>("PullLeftCollider");
	m_PullLeftCollider->SetCollisionProfile("Player");
	m_PullLeftCollider->Start(); // SceneCollision 의 Collider List 에 추가하기
	m_PullLeftCollider->Enable(false);
	m_PullLeftCollider->AddCollisionCallback(Collision_State::Begin, this, &CPlayer2D::PullLeftCollisionBeginCallback);
	m_PullLeftCollider->AddCollisionCallback(Collision_State::End, this, &CPlayer2D::PullLeftCollisionEndCallback);
	m_PullLeftCollider->SetExtend((m_PullDistance + m_Body->GetWorldScale().x * 1.5f) * 0.5f,
		(m_KirbyState->GetWorldScale().y * 0.5f));
	m_PullLeftCollider->SetRelativePos(m_Body->GetWorldScale().x * -1.5f * 0.5f, 0.f, 1.f);

	m_KirbyState->AddChild(m_PullRightCollider);
	m_KirbyState->AddChild(m_PullLeftCollider);

	// m_PullLeftCollider = CreateComponent<CColliderBox2D>("PullLeftCollider");
	// m_PullLeftCollider->SetCollisionProfile("PlayerAttack");
	// m_PullLeftCollider->SetEnable(false);
	// m_RootComponent->AddChild(m_PullLeftCollider);
	// m_OriginColliderBodyScale = m_Body->GetWorldScale();

	m_NavAgent = dynamic_cast<CNavAgent*>(FindComponent("NavAgent"));

	// Root Component Animation 세팅
	// CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Fight", TEXT("Kirby_Fight.anim"));
	// m_KirbyState->SetAnimationInstance(AnimationInstance);

	// Widget Component의 Widget 생성
	m_SimpleHUDWidget = (CWidgetComponent*)(FindComponent("SimpleHUD"));
	if (!m_SimpleHUDWidget)
	{
		m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");
		m_KirbyState->AddChild(m_SimpleHUDWidget);
	}
	m_SimpleHUDWidget->CreateUIWindow<CSimpleHUD>("SimpleHUDWidget");
	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	m_Camera = (CCameraComponent*)FindComponent("Camera");
	// m_Camera = FindComponentByType<CCameraComponent>();

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
		KeyState_Down, this, &CPlayer2D::SpecialChange);

	/*
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Attack", 
		KeyState_Down, this, &CPlayer2D::Attack);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Attack1", 
		KeyState_Down, this, &CPlayer2D::Attack1);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Skill1", 
		KeyState_Down, this, &CPlayer2D::Skill1);
	*/
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MovePoint", 
		KeyState_Down, this, &CPlayer2D::MovePointDown);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Jump", 
		KeyState_Down, this, &CPlayer2D::Jump);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("JumpWhileDash",
		KeyState_Down, this, &CPlayer2D::Jump);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("SpitOut",
		KeyState_Push, this, &CPlayer2D::SpitOut);

	// Animation Play Scale 세팅
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightEatIdle")->SetPlayTime(2.f);
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftEatIdle")->SetPlayTime(2.f);

	// Animation Loop 여부 세팅
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetLoop(false);

	// Jump 가 다 끝난 이후에는, Animation 을 Fall 상태로 바꿔라
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetEndFunction(
		this, &CPlayer2D::ChangePlayerFallAnimation);
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetEndFunction(
		this, &CPlayer2D::ChangePlayerFallAnimation); 
}

void CPlayer2D::Update(float DeltaTime)
{
	CLifeObject::Update(DeltaTime);

	PlayerMoveUpdate(DeltaTime);

	FallFromCliff();

	ChangeToIdleWhenReachGroundAfterFall();
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CLifeObject::PostUpdate(DeltaTime);

	CheckOutsideWorldResolution();

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

void CPlayer2D::UpdateWhileOffGround(float DeltaTime)
{
	CLifeObject::UpdateWhileOffGround(DeltaTime);
}

void CPlayer2D::MoveUp(float DeltaTime)
{
	FlyAfterJump(DeltaTime);
	// m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime);
}

void CPlayer2D::MoveUpEnd(float DeltaTime)
{
	if (m_IsFlying)
	{
		// 땋에 닿은 상태라면, m_IsFlying을 false 로 해서
		// 다시 날려면, Jump 이후에 날 수 있게 세팅한다
		// if (m_IsGround)
		m_IsFlying = false;

		ChangePlayerFallAnimation();
	}

	// Fall StartY 를 여기서 다시 세팅한다.
	m_FallTime = 0.f;
	m_FallStartY = GetWorldPos().y;

	m_IsFalling = true;
}

void CPlayer2D::MoveDown(float DeltaTime)
{
	m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime * -1.f);
}

void CPlayer2D::MoveLeft(float DeltaTime) //
{
	// 오른쪽 버튼을 누른 상태라면 X
	if (m_RightMovePush)
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
		m_DashVelocity -= m_DashMoveAccel;

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
	ChangePlayerWalkAnimation();
}

void CPlayer2D::MoveDashLeft(float DeltaTime)
{
	// 오른쪽 버튼을 누르고 있다면
	if (m_RightMovePush)
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
	ChangePlayerRunAnimation();
	
}

void CPlayer2D::MoveRight(float DeltaTime)
{

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
		m_DashVelocity -= m_DashMoveAccel;

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
	ChangePlayerWalkAnimation();

}

void CPlayer2D::MoveDashRight(float DeltaTime)
{
	if (m_LeftMovePush)
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
	ChangePlayerRunAnimation();
	
}

void CPlayer2D::LeftLeverMoveEnd(float DeltaTime)
{
	m_LeftMovePush = false;

	ResetMoveInfo();

	ChangePlayerIdleAnimation();

	PullLeftEnd(DeltaTime);
}

void CPlayer2D::RightLeverMoveEnd(float DeltaTime)
{
	m_RightMovePush = false;

	ResetMoveInfo();

	ChangePlayerIdleAnimation();

	PullRightEnd(DeltaTime);
}

void CPlayer2D::LeftDashMoveEnd(float DeltaTime)
{
	ChangePlayerWalkAnimation();

	m_LeftMovePush = false;

	ResetMoveInfo();

	PullLeftEnd(DeltaTime);
}

void CPlayer2D::RightDashMoveEnd(float DeltaTime)
{
	ChangePlayerWalkAnimation();

	m_RightMovePush = false;

	ResetMoveInfo();

	PullRightEnd(DeltaTime);
}

void CPlayer2D::SpitOut(float DeltaTime)
{
	if (!m_IsEatingMonster)
		return;

	m_IsEatingMonster = false;

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

	m_EatenMonster = nullptr;
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
		m_LeverVelocity -= m_LeverMoveAccel ;

		if (m_LeverVelocity <= 0.f)
			m_LeverVelocity = 0.f;
	}

	// 대쉬를 진행하고 있지 않다면, 대쉬 속도도 감속 시킨다.
	if (!m_IsDashMoving)
	{
		m_DashVelocity -= m_DashMoveAccel ;

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

			ChangePlayerIdleAnimation();

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

	// 속도가 없고 + 점프 상태가 아니라면 --> Idle Animation으로 바꿔준다.
	/*
	if (m_MoveVelocity <= 0.f && !m_Jump)
	{
		ChangePlayerIdleAnimation();
	}
	*/
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
}

void CPlayer2D::SimpleJump()
{
	if (!m_Jump)
	{
  		m_Jump = true;
		m_IsGround = false;

		m_FallTime = 0.f;
		m_FallStartY = GetWorldPos().y;

		m_IsBottomCollided = false;

		// Animation Change
		if (m_IsEatingMonster)
			ChangePlayerWalkAnimation();
		else
			ChangePlayerJumpAnimation();
	}
}

void CPlayer2D::Jump(float DeltaTime)
{
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
		SimpleJump();  
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
}

void CPlayer2D::FallFromCliff()
{
	// 절벽에서 막 떨어졌을 때
	if (m_FallStartY - GetWorldPos().y > 5.f && !m_IsBottomCollided && !m_IsFlying)
	{
		ChangePlayerFallAnimation();
	}
}

void CPlayer2D::ChangeToIdleWhenReachGroundAfterFall()
{
	if (m_IsBottomCollided)
	{
		// 충돌하는 순간 Animation 이 Fall 이라면, Animation을 Idle로
		std::string CurAnimName = m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName();

		if (CurAnimName == "RightFall" || CurAnimName == "LeftFall" || 
			CurAnimName == "RightJump" || CurAnimName == "LeftJump")
		{
			if (m_IsEatingMonster)
				ChangePlayerEatIdleAnimation();
			else
				ChangePlayerNormalIdleAnimation();
		}

		m_IsFalling = false;
	}
}

void CPlayer2D::ChangePlayerIdleAnimation()
{
	if (!m_Jump && !m_IsFlying)
	{
		std::string CurAnimName = m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName();

		if (CurAnimName == "RightFall" || CurAnimName == "LeftFall")
			return;

		if (m_IsEatingMonster)
			ChangePlayerEatIdleAnimation();
		else
			ChangePlayerNormalIdleAnimation();
	}
}

void CPlayer2D::ChangePlayerNormalIdleAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftIdle");
	else
		ChangeAnimation("RightIdle");
}

void CPlayer2D::ChangePlayerWalkAnimation()
{
	if (!m_Jump && !m_IsFlying)
	{
		std::string CurAnimName = m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName();

		if (CurAnimName == "RightFall" || CurAnimName == "LeftFall")
			return;

		if (m_IsEatingMonster)
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
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftWalk");
	else
		ChangeAnimation("RightWalk");
}

void CPlayer2D::ChangePlayerHitAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftHit");
	else
		ChangeAnimation("RightHit");
}

void CPlayer2D::ChangePlayerRunAnimation()
{
	if (!m_Jump && !m_IsFlying)
	{
		std::string CurAnimName = m_KirbyState->GetAnimationInstance()->GetCurrentAnimation()->GetName();

		if (CurAnimName == "RightFall" || CurAnimName == "LeftFall")
			return;

		if (m_IsEatingMonster)
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
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftRun");
	else
		ChangeAnimation("RightRun");
}

void CPlayer2D::ChangePlayerEatRunAnimation()
{
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

void CPlayer2D::ChangePlayerFlyAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftFly");
	else
		ChangeAnimation("RightFly");
}

void CPlayer2D::ChangePlayerJumpAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftJump");
	else
 		ChangeAnimation("RightJump");
}

void CPlayer2D::ChangePlayerFallAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftFall");
	else
		ChangeAnimation("RightFall");
}

void CPlayer2D::ChangePlayerEatIdleAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftEatIdle");
	else
		ChangeAnimation("RightEatIdle");
}

void CPlayer2D::ChangePlayerEatWalkAnimation()
{
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
}

void CPlayer2D::FallDownAttack(const CollisionResult& Result)
{
	Vector3 CurrentPos = GetWorldPos();

	if (m_FallStartY > CurrentPos.y && !m_IsGround)
	{
		CColliderComponent* DestCollider = Result.Dest;

		CGameObject* OwnerObject = DestCollider->GetGameObject();

		CMonster* OwnerMonster = (CMonster*)OwnerObject;

		if (!OwnerMonster)
			return;

		// 1) Hit Effect

		// 2) Sound

		// 3) Damage Font 작성

		// Hit 상태로 변경
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
	}
}

void CPlayer2D::PullRight(float DeltaTime)
{
	m_KirbyState->GetAnimationInstance()->ChangeAnimation("RightPull");

	// 오른쪽 범위 안에 있는지 확인하기
	float PullDist = 0.f;

	m_PullRightCollider->Enable(true);

	m_IsPulling = true;

	m_ObjectMoveDir.x = 1.f;
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

	if (!DestMonster)
		return;

	m_PullingMonster = DestMonster;

	if (!DestMonster->IsBeingPulled())
	{
		DestMonster->SetIsBeingPulled(true);
		DestMonster->SetCollisionDisable(true);
		DestMonster->SetPulledDestPos(GetWorldPos());
	}
}

void CPlayer2D::PullRightCollisionEndCallback(const CollisionResult& Result)
{
	if (m_PullingMonster)
	{
		m_PullingMonster->SetIsBeingPulled(false);
		m_PullingMonster->ResetPulledInfo();
		m_PullingMonster->SetCollisionDisable(false);
		m_PullingMonster = nullptr;
	}
}

void CPlayer2D::PullLeftCollisionEndCallback(const CollisionResult& Result)
{
	if (m_PullingMonster)
	{
		m_PullingMonster->SetIsBeingPulled(false);
		m_PullingMonster->ResetPulledInfo();
		m_PullingMonster->SetCollisionDisable(false);
		m_PullingMonster = nullptr;
	}
}

void CPlayer2D::SpecialChange(float DeltaTime)
{
	/*
	// 먹고 있는 녀석이 없으면 X
	if (!m_IsEatingMonster)
		return;

	// 능력 몬스터가 아니라면 Return
	if (!m_EatenMonster->IsAbilityMonster())
		return;
	*/

	// 아래 녀석들을 기존에서 Child 에서 떼어내고 새롭게 붙여야 한다. 

	m_KirbyState->DeleteChild(m_Camera);
	m_KirbyState->DeleteChild(m_SimpleHUDWidget);
	m_KirbyState->DeleteChild(m_Body);
	m_KirbyState->DeleteChild(m_PullRightCollider);
	m_KirbyState->DeleteChild(m_PullLeftCollider);

	Vector3 OriginalPos = GetWorldPos();

	// SAFE_DELETE(m_KirbyState);

	if (m_TempChange)
	{
		m_KirbyState = CreateComponent<CFightKirbyState>("FightKirbyState");

		m_TempChange = false;
	}
	else
	{
		m_KirbyState = CreateComponent<CNormalKirbyState>("FightKirbyState");

		m_TempChange = true;
	}

	SetRootComponent(m_KirbyState);

	m_KirbyState->SetWorldPos(OriginalPos);

	m_KirbyState->AddChild(m_Camera);
	m_KirbyState->AddChild(m_SimpleHUDWidget);
	m_KirbyState->AddChild(m_Body);
	m_KirbyState->AddChild(m_PullRightCollider);
	m_KirbyState->AddChild(m_PullLeftCollider);
}

void CPlayer2D::Attack()
{
	if (!m_KirbyState)
		return;
	// m_KirbyState->Attack();
}

void CPlayer2D::FallDownAttack()
{
	if (!m_KirbyState)
		return;
	// m_KirbyState->FallDownAttack();
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

	if (!DestMonster)
		return;

	m_PullingMonster = DestMonster;

	if (!DestMonster->IsBeingPulled())
	{
		DestMonster->SetIsBeingPulled(true);
		DestMonster->SetCollisionDisable(true);
		DestMonster->SetPulledDestPos(GetWorldPos());
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

	ChangePlayerIdleAnimation();

	m_IsPulling = false;
}

void CPlayer2D::PullLeft(float DeltaTime)
{
	m_KirbyState->GetAnimationInstance()->ChangeAnimation("LeftPull");

	// 오른쪽 범위 안에 있는지 확인하기
	float PullDist = 0.f;

	// 충돌체의 크기를 늘려볼까 ?
	m_PullLeftCollider->Enable(true);

	m_IsPulling = true;

	m_ObjectMoveDir.x = 1.f * -1;
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

	ChangePlayerIdleAnimation();

	m_IsPulling = false;
}

void CPlayer2D::MovePointDown(float DeltaTime)
{
	Vector2 MousePos = CInput::GetInst()->GetMouseWorld2DPos();

	Move(Vector3(MousePos.x, MousePos.y, 0.f));
}
