#include "Player2D.h"

#include <Scene/CameraManager.h>

#include "Bullet.h"
#include "BulletTornaido.h"
#include "Input.h"
#include "PlayerAnimation2D.h"
#include "Resource/Material/Material.h"
#include "Scene/Scene.h"
#include "Component/ColliderBox2D.h"
#include "Component/CameraComponent.h"
#include "Component/TileEmptyComponent.h"
#include "BulletCamera.h"
#include "Scene/NavigationManager.h"
#include "../UI/SimpleHUD.h"

CPlayer2D::CPlayer2D():
m_MoveVelocity(0.f),
m_LeverMoveAccel(1.f),
m_LeverVelocity(0.f),
m_LeverMaxMoveVelocity(350.f),
m_DashMoveAccel(1.5f),
m_DashVelocity(0.f),
m_DashMaxMoveVelocity(250.f),
m_RightMove(false),
m_ToLeftWhenRightMove(false),
m_RightMovePush(false),
m_LeftMove(false),
m_ToRightWhenLeftMove(false),
m_LeftMovePush(false),
m_IsLeverMoving(false),
m_IsDashMoving(false),
m_TriangleJump(false)
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
	m_Sprite           = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerSprite"));
	m_ChildLeftSprite  = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChildLeftSprite"));
	m_ChildRightSprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChildRightSprite"));
	m_ChildLeftMuzzle  = dynamic_cast<CSceneComponent*>(FindComponent("LeftMuzzle"));
	m_ChildRightMuzzle = dynamic_cast<CSceneComponent*>(FindComponent("RightMuzzle"));
	m_ChildRoot        = dynamic_cast<CSceneComponent*>(FindComponent("PlayerChildRoot"));
	m_Muzzle           = dynamic_cast<CSceneComponent*>(FindComponent("Muzzle"));

	m_Child1Sprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChild1Sprite"));
	m_Child2Sprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChild2Sprite"));
	m_Child3Sprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChild3Sprite"));
	m_Child4Sprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChild4Sprite"));

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
	m_Sprite           = CreateComponent<CSpriteComponent>("PlayerSprite");
	m_ChildLeftSprite  = CreateComponent<CSpriteComponent>("PlayerChildLeftSprite");
	m_ChildRightSprite = CreateComponent<CSpriteComponent>("PlayerChildRightSprite");
	m_ChildRoot        = CreateComponent<CSceneComponent>("PlayerChildRoot");
	m_Muzzle           = CreateComponent<CSceneComponent>("Muzzle");

	m_ChildLeftMuzzle  = CreateComponent<CSceneComponent>("LeftMuzzle");
	m_ChildRightMuzzle = CreateComponent<CSceneComponent>("RightMuzzle");

	m_Child1Sprite = CreateComponent<CSpriteComponent>("PlayerChild1Sprite");
	m_Child2Sprite = CreateComponent<CSpriteComponent>("PlayerChild2Sprite");
	m_Child3Sprite = CreateComponent<CSpriteComponent>("PlayerChild3Sprite");
	m_Child4Sprite = CreateComponent<CSpriteComponent>("PlayerChild4Sprite");

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

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_ChildLeftSprite);
	m_Sprite->AddChild(m_ChildRightSprite);
	m_Sprite->AddChild(m_Muzzle);
	m_Sprite->AddChild(m_ChildRoot);
	m_Sprite->AddChild(m_Body);
	m_Sprite->AddChild(m_Camera);
	m_Sprite->AddChild(m_SimpleHUDWidget);

	// 반드시 이 위치에서 ( AddChild 이후에 위치 좌표 세팅 )
	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	m_Sprite->SetTransparency(true);
	//m_Sprite->SetOpacity(0.5f);

	CAnimationSequence2DInstance* AnimationInstance =  m_Scene->GetResource()->LoadAnimationInstance("Kirby_Fight", TEXT("Kirby_Fight.anim"));

	m_Sprite->SetAnimationInstance(AnimationInstance);

	// m_Sprite->CreateAnimationInstance<CPlayerAnimation2D>();

	m_ChildLeftSprite->AddChild(m_ChildLeftMuzzle);
	m_ChildRightSprite->AddChild(m_ChildRightMuzzle);
	m_ChildLeftSprite->SetTexture(0, 0, (int)(Buffer_Shader_Type::Pixel), "Teemo",
	                              TEXT("Teemo.jpg"));
	m_ChildRightSprite->SetTexture(0, 0, (int)(Buffer_Shader_Type::Pixel), "Teemo",
	                               TEXT("Teemo.jpg"));
	m_ChildLeftSprite->SetBaseColor(1.f, 0.f, 0.f, 1.f);
	m_ChildRightSprite->SetBaseColor(1.f, 0.f, 0.f, 1.f);


	m_ChildRoot->AddChild(m_Child1Sprite);
	m_ChildRoot->AddChild(m_Child2Sprite);
	m_ChildRoot->AddChild(m_Child3Sprite);
	m_ChildRoot->AddChild(m_Child4Sprite);

	m_Muzzle->SetRelativePos(0.f, 150.f, 0.f);
	m_Muzzle->SetInheritRotZ(true);

	m_ChildLeftMuzzle->SetRelativePos(0.f, 100.f, 0.f);
	m_ChildLeftMuzzle->SetInheritRotZ(true);

	m_ChildRightMuzzle->SetRelativePos(0.f, 100.f, 0.f);
	m_ChildRightMuzzle->SetInheritRotZ(true);

	m_ChildRightSprite->SetRelativeScale(50.f, 50.f, 1.f);
	m_ChildRightSprite->SetInheritScale(false);
	m_ChildRightSprite->SetRelativePos(100.f, 0.f, 0.f);
	m_ChildRightSprite->SetPivot(0.5f, 0.5f, 0.f);
	m_ChildRightSprite->SetInheritRotZ(true);

	m_ChildLeftSprite->SetRelativeScale(50.f, 50.f, 1.f);
	m_ChildLeftSprite->SetInheritScale(false);
	m_ChildLeftSprite->SetRelativePos(-100.f, 0.f, 0.f);
	m_ChildLeftSprite->SetPivot(0.5f, 0.5f, 0.f);
	m_ChildLeftSprite->SetInheritRotZ(true);

	m_Child1Sprite->SetRelativeScale(25.f, 25.f, 1.f);
	m_Child1Sprite->SetInheritScale(false);
	m_Child1Sprite->SetRelativePos(200.f, 0.f, 0.f);
	m_Child1Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Child1Sprite->SetInheritRotZ(true);

	m_Child2Sprite->SetRelativeScale(25.f, 25.f, 1.f);
	m_Child2Sprite->SetInheritScale(false);
	m_Child2Sprite->SetRelativePos(-200.f, 0.f, 0.f);
	m_Child2Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Child2Sprite->SetInheritRotZ(true);

	m_Child3Sprite->SetRelativeScale(25.f, 25.f, 1.f);
	m_Child3Sprite->SetInheritScale(false);
	m_Child3Sprite->SetRelativePos(0.f, 200.f, 0.f);
	m_Child3Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Child3Sprite->SetInheritRotZ(true);

	m_Child4Sprite->SetRelativeScale(25.f, 25.f, 1.f);
	m_Child4Sprite->SetInheritScale(false);
	m_Child4Sprite->SetRelativePos(0.f, -200.f, 0.f);
	m_Child4Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Child4Sprite->SetInheritRotZ(true);

	/*
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveUp", KeyState_Push, this, &CPlayer2D::MoveUp);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDown", KeyState_Push, this, &CPlayer2D::MoveDown);
	// CInput::GetInst()->SetKeyCallback<CPlayer2D>("RotationZInv", KeyState_Push, this, &CPlayer2D::RotationZInv);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveLeft", KeyState_Push, this, &CPlayer2D::MoveLeft);
	// CInput::GetInst()->SetKeyCallback<CPlayer2D>("RotationZ", KeyState_Push, this, &CPlayer2D::RotationZ);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveRight", KeyState_Push, this, &CPlayer2D::MoveRight);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Attack", KeyState_Down, this, &CPlayer2D::Attack);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Attack1", KeyState_Down, this, &CPlayer2D::Attack1);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Skill1", KeyState_Down, this, &CPlayer2D::Skill1);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MovePoint", KeyState_Down, this, &CPlayer2D::MovePointDown);
	*/

	// Pivot 값이 없다면, 원래의 pos 인 왼쪽 하단 pos 를 중심으로
	// Center 가 형성되게 될 것이다. 
	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(100.f, 50.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CPlayer2D::Start()
{
	CLifeObject::Start();

	m_Sprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerSprite"));
	m_ChildLeftSprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChildLeftSprite"));
	m_ChildRightSprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChildRightSprite"));
	m_ChildLeftMuzzle = dynamic_cast<CSceneComponent*>(FindComponent("LeftMuzzle"));
	m_ChildRightMuzzle = dynamic_cast<CSceneComponent*>(FindComponent("RightMuzzle"));
	m_ChildRoot = dynamic_cast<CSceneComponent*>(FindComponent("PlayerChildRoot"));
	m_Muzzle = dynamic_cast<CSceneComponent*>(FindComponent("Muzzle"));

	m_Child1Sprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChild1Sprite"));
	m_Child2Sprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChild2Sprite"));
	m_Child3Sprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChild3Sprite"));
	m_Child4Sprite = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerChild4Sprite"));

	m_Body = (CColliderBox2D*)FindComponent("Body");
	m_NavAgent = dynamic_cast<CNavAgent*>(FindComponent("NavAgent"));

	// Root Component Animation 세팅
	// CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Fight", TEXT("Kirby_Fight.anim"));
	// m_Sprite->SetAnimationInstance(AnimationInstance);

	// Widget Component의 Widget 생성
	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");
	m_SimpleHUDWidget->CreateUIWindow<CSimpleHUD>("SimpleHUDWidget");

	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Camera->OnViewportCenter(); // Player 중심 세팅
	m_Sprite->AddChild(m_Camera);

	// CameraManager 에 세팅하기
	// m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);

	// Key Input 세팅 
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveUp", 
		KeyState_Push, this, &CPlayer2D::MoveUp);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDown", 
		KeyState_Push, this, &CPlayer2D::MoveDown);
	// CInput::GetInst()->SetKeyCallback<CPlayer2D>("RotationZInv", KeyState_Push, this, &CPlayer2D::RotationZInv);
	// CInput::GetInst()->SetKeyCallback<CPlayer2D>("RotationZ", KeyState_Push, this, &CPlayer2D::RotationZ);

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

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Attack", 
		KeyState_Down, this, &CPlayer2D::Attack);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Attack1", 
		KeyState_Down, this, &CPlayer2D::Attack1);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Skill1", 
		KeyState_Down, this, &CPlayer2D::Skill1);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MovePoint", 
		KeyState_Down, this, &CPlayer2D::MovePointDown);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Jump", 
		KeyState_Down, this, &CPlayer2D::Jump);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("JumpWhileDash",
		KeyState_Down, this, &CPlayer2D::Jump);

}

void CPlayer2D::Update(float DeltaTime)
{
	CLifeObject::Update(DeltaTime);

	PlayerMoveUpdate(DeltaTime);

	static bool Fire2 = false;
	
	static bool Hide = false;

	if (GetAsyncKeyState('2') & 0x8000)
	{
		Fire2 = true;
	}

	else if (Fire2)
	{
		Fire2 = false;

		CBulletTornaido* Bullet = m_Scene->CreateGameObject<CBulletTornaido>("Bullet");

		//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
		Bullet->SetWorldPos(m_Muzzle->GetWorldPos());
		Bullet->SetWorldRotation(GetWorldRot());

		Hide = true;
	}

	m_ChildRoot->AddRelativeRotation(0.f, 0.f, 180.f * DeltaTime);

	if (Hide)
	{
		m_Opacity -= DeltaTime / 5.f;

		if (m_Opacity < 0.f)
			m_Opacity = 0.f;
		
		m_Sprite->SetOpacity(m_Opacity);
	}
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CLifeObject::PostUpdate(DeltaTime);

	CheckOutsideWorldResolution();
}

CPlayer2D* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::CheckOutsideWorldResolution()
{
	// todo : Up
	Vector3 OriginalPos = GetWorldPos();
	Vector3 WorldScale = GetWorldScale();
	Vector3 Pivot = GetPivot();

	Vector2 WorldResolution = m_Scene->GetWorldResolution();

	if (OriginalPos.y + WorldScale.y * Pivot.y >= WorldResolution.y)
		OriginalPos.y = WorldResolution.y - WorldScale.y * Pivot.y - 0.001f;

	// todo : Down ( 나중에 )

	// Left
	if (OriginalPos.x - WorldScale.x * Pivot.x < 0.f)
		OriginalPos.x = WorldScale.x * Pivot.x + 0.001f;

	// Right
	if (OriginalPos.x + WorldScale.x * Pivot.x >= WorldResolution.x)
		OriginalPos.x = WorldResolution.x - WorldScale.x * Pivot.x - 0.001f;

	SetWorldPos(OriginalPos);
}

void CPlayer2D::MoveUp(float DeltaTime)
{
	m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime);
}

void CPlayer2D::MoveDown(float DeltaTime)
{
	m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime * -1.f);
}

void CPlayer2D::MoveLeft(float DeltaTime) //
{
	// 삼각 점프 상태였다면 계속 이동 시킨다
	if (m_TriangleJump)
	{
		if (m_RightMove)
			m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
		else
			m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);

		return;
	}

	// 오른쪽 버튼을 누른 상태라면 X
	if (m_RightMovePush)
		return;

	// 레버 이동 표시
	m_IsLeverMoving = true;

	// 왼쪽 버튼 누르는 중 표시
	m_LeftMovePush = true;

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
		m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
	else
		m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);

	// todo : 여기서 Animation 변환 처리를 해준다.
}

void CPlayer2D::MoveDashLeft(float DeltaTime)
{
	if (m_TriangleJump)
	{
		if (m_RightMove)
			m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
		else
			m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);

		return;
	}

	// 오른쪽 버튼을 누르고 있다면
	if (m_RightMovePush)
		return;

	// 이동 상태 True + 버튼 이동 상태 True
	m_IsLeverMoving = true;
	m_IsDashMoving = true;

	m_LeftMovePush = true;


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
		m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
	else
		m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);

	// todo : Change Animation
}

void CPlayer2D::MoveRight(float DeltaTime)
{
	// Triangle Jump 중이라면 이동은 계속 하되 + 키보드는 안먹게 한다
	if (m_TriangleJump)
	{
		if (m_RightMove)
			m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
		else
			m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);

		return;
	}

	// 왼쪽 키를 누르고 있었다면 안먹게 한다
	if (m_LeftMovePush)
		return;

	// 일반 이동 표시
	m_IsLeverMoving = true;

	// 오른쪽 이동 표시
	m_RightMovePush = true;

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
		m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
	else
		m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);

	// Animation 전환


}

void CPlayer2D::MoveDashRight(float DeltaTime)
{
	if (m_TriangleJump)
	{
		if (m_RightMove)
			m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
		else
			m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);
		return;
	}

	if (m_LeftMovePush)
		return;

	m_IsLeverMoving = true;
	m_IsDashMoving  = true;

	m_RightMovePush = true;

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
		m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
	else
		m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);

	// todo : Change Animation

}

void CPlayer2D::LeftLeverMoveEnd(float DeltaTime)
{
	m_LeftMovePush = false;
	ResetMoveInfo();
}

void CPlayer2D::RightLeverMoveEnd(float DeltaTime)
{
	m_RightMovePush = false;
	ResetMoveInfo();
}

void CPlayer2D::LeftDashMoveEnd(float DeltaTime)
{
	m_LeftMovePush = false;
	ResetMoveInfo();
}

void CPlayer2D::RightDashMoveEnd(float DeltaTime)
{
	m_RightMovePush = false;
	ResetMoveInfo();
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
			m_LeverVelocity -= m_LeverMoveAccel * 1.5f;
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
			m_LeverVelocity -= m_LeverMoveAccel * 1.5f;
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
			m_DashVelocity -= m_DashMoveAccel * 1.5f;
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
			m_DashVelocity -= m_DashMoveAccel * 1.5f;
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
		m_LeverVelocity -= m_LeverMoveAccel * 0.5f;

		if (m_LeverVelocity <= 0.f)
			m_LeverVelocity = 0.f;
	}

	// 대쉬를 진행하고 있지 않다면, 대쉬 속도도 감속 시킨다.
	if (!m_IsDashMoving)
	{
		m_DashVelocity -= m_DashMoveAccel * 0.5f;

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

			m_MoveVelocity = 0.f;
		}

		// 감속 중임에도 이동은 시켜줘야 한다.
		if (m_RightMove)
			m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime);
		else
			m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * m_MoveVelocity * DeltaTime * -1.f);

	}
}

void CPlayer2D::ResetMoveInfo()
{
	m_IsDashMoving = false;
	m_IsLeverMoving = false;
	m_ToRightWhenLeftMove = false;
	m_ToLeftWhenRightMove = false;
}

void CPlayer2D::RotationZInv(float DeltaTime) //
{
	m_Sprite->AddRelativeRotationZ(180.f * DeltaTime);
}

void CPlayer2D::RotationZ(float DeltaTime)
{
	m_Sprite->AddRelativeRotationZ(-180.f * DeltaTime);
}

void CPlayer2D::SimpleJump()
{
	if (!m_Jump)
	{
		m_Jump = true;
		m_IsGround = false;

		m_FallTime = 0.f;
		m_FallStartY = GetWorldPos().y;
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
				if (TilePos.y + TileSize.y - 0.01f <= LB.y && LB.y <= TilePos.y + TileSize.y + 0.01f)
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
				if (TilePos.y + TileSize.y - 0.001f <= LB.y &&
					TilePos.y + TileSize.y + 0.001f)
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

	// 현재 그냥 땅에 있는 상태였다면 Simple Jump를 수행한다
	if (m_IsGround)
		SimpleJump();
	// 그게 아니라면 삼각 점프를 수행한다.
	else if (SideCollision)
	{
		// 현재 움직이는 방향 반대로 움직여야 한다
		// 오른쪽
		if (m_RightMove)
	}

}

void CPlayer2D::Attack(float DeltaTime)
{
	CBullet* Bullet = m_Scene->CreateGameObject<CBullet>("Bullet");
	// Bullet->SetCollisionProfile("MonsterAttack");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(m_Muzzle->GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot());

	Bullet = m_Scene->CreateGameObject<CBullet>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(m_ChildLeftMuzzle->GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot());

	Bullet = m_Scene->CreateGameObject<CBullet>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(m_ChildRightMuzzle->GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot());
	// Bullet->SetCollisionProfile("MonsterAttack");

}

void CPlayer2D::Attack1(float DeltaTime)
{
	CBullet* Bullet = m_Scene->CreateGameObject<CBullet>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(m_Muzzle->GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot());
	Bullet->SetCollisionProfile("PlayerAttack");

	Bullet = m_Scene->CreateGameObject<CBullet>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(m_Muzzle->GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot() + Vector3(0.f, 0.f, 45.f));
	Bullet->SetCollisionProfile("PlayerAttack");

	Bullet = m_Scene->CreateGameObject<CBullet>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(m_Muzzle->GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot() + Vector3(0.f, 0.f, -45.f));
	Bullet->SetCollisionProfile("PlayerAttack");
}

void CPlayer2D::Skill1(float DeltaTime)
{
	CBulletCamera* BulletCamera = m_Scene->CreateGameObject<CBulletCamera>("BulletCamera");
	BulletCamera->SetWorldPos(m_Muzzle->GetWorldPos());
	BulletCamera->SetWorldRotation(GetWorldRot());
}

void CPlayer2D::MovePointDown(float DeltaTime)
{
	Vector2 MousePos = CInput::GetInst()->GetMouseWorld2DPos();

	Move(Vector3(MousePos.x, MousePos.y, 0.f));
}
