#include "PlayerOld.h"
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

CPlayerOld::CPlayerOld() :
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
	SetTypeID<CPlayerOld>();
	m_SolW = false;
	m_WDistance = 0.f;
	m_Opacity = 1.f;

	m_MoveVelocityMax = m_LeverMaxMoveVelocity + m_DashMaxMoveVelocity;

}

CPlayerOld::CPlayerOld(const CPlayerOld& obj) :
	CLifeObject(obj)
{
	m_KirbyState = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerSprite"));
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
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_SimpleHUDWidget = (CWidgetComponent*)FindComponent("SimpleHUD");

	m_NavAgent = dynamic_cast<CNavAgent*>(FindComponent("NavAgent"));

	m_Opacity = obj.m_Opacity;
}

CPlayerOld::~CPlayerOld()
{
}

bool CPlayerOld::Init()
{
	m_KirbyState = CreateComponent<CSpriteComponent>("PlayerSprite");
	m_ChildLeftSprite = CreateComponent<CSpriteComponent>("PlayerChildLeftSprite");
	m_ChildRightSprite = CreateComponent<CSpriteComponent>("PlayerChildRightSprite");
	m_ChildRoot = CreateComponent<CSceneComponent>("PlayerChildRoot");
	m_Muzzle = CreateComponent<CSceneComponent>("Muzzle");

	m_ChildLeftMuzzle = CreateComponent<CSceneComponent>("LeftMuzzle");
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
	m_Camera->OnViewportCenter(); // Player �߽� ����

	// NavAgent
	m_NavAgent = CreateComponent<CNavAgent>("NavAgent");

	SetRootComponent(m_KirbyState);
	m_KirbyState->AddChild(m_ChildLeftSprite);
	m_KirbyState->AddChild(m_ChildRightSprite);
	m_KirbyState->AddChild(m_Muzzle);
	m_KirbyState->AddChild(m_ChildRoot);
	m_KirbyState->AddChild(m_Body);
	m_KirbyState->AddChild(m_Camera);
	m_KirbyState->AddChild(m_SimpleHUDWidget);

	// �ݵ�� �� ��ġ���� ( AddChild ���Ŀ� ��ġ ��ǥ ���� )
	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	m_KirbyState->SetTransparency(true);

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Fight", TEXT("Kirby_Fight.anim"));

	m_KirbyState->SetAnimationInstance(AnimationInstance);

	// m_KirbyState->CreateAnimationInstance<CPlayerAnimation2D>();

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
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveUp", KeyState_Push, this, &CPlayerOld::MoveUp);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveDown", KeyState_Push, this, &CPlayerOld::MoveDown);
	// CInput::GetInst()->SetKeyCallback<CPlayerOld>("RotationZInv", KeyState_Push, this, &CPlayerOld::RotationZInv);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveLeft", KeyState_Push, this, &CPlayerOld::MoveLeft);
	// CInput::GetInst()->SetKeyCallback<CPlayerOld>("RotationZ", KeyState_Push, this, &CPlayerOld::RotationZ);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveRight", KeyState_Push, this, &CPlayerOld::MoveRight);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("Attack", KeyState_Down, this, &CPlayerOld::Attack);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("Attack1", KeyState_Down, this, &CPlayerOld::Attack1);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("Skill1", KeyState_Down, this, &CPlayerOld::Skill1);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MovePoint", KeyState_Down, this, &CPlayerOld::MovePointDown);
	*/

	// Pivot ���� ���ٸ�, ������ pos �� ���� �ϴ� pos �� �߽�����
	// Center �� �����ǰ� �� ���̴�. 
	m_KirbyState->SetRelativeScale(100.f, 100.f, 1.f);
	m_KirbyState->SetRelativePos(100.f, 50.f, 0.f);
	m_KirbyState->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CPlayerOld::Start()
{
	CLifeObject::Start();

	m_KirbyState = dynamic_cast<CSpriteComponent*>(FindComponent("PlayerSprite"));
	SetRootComponent(m_KirbyState);

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
	m_Body->AddCollisionCallback(Collision_State::Begin, this, &CPlayerOld::FallDownAttack);

	m_PullRightCollider = CreateComponent<CColliderBox2D>("PullRightCollider");
	m_PullRightCollider->SetCollisionProfile("Player");
	m_PullRightCollider->Start(); // SceneCollision �� Collider List �� �߰��ϱ�
	m_PullRightCollider->Enable(false);
	m_PullRightCollider->AddCollisionCallback(Collision_State::Begin, this, &CPlayerOld::PullRightCollisionBeginCallback);
	m_PullRightCollider->AddCollisionCallback(Collision_State::End, this, &CPlayerOld::PullRightCollisionEndCallback);
	m_PullRightCollider->SetExtend((m_PullDistance + m_Body->GetWorldScale().x * 1.5f) * 0.5f,
		(m_KirbyState->GetWorldScale().y * 0.5f));
	m_PullRightCollider->SetRelativePos(m_Body->GetWorldScale().x * 1.5f * 0.5f, 0.f, 1.f);

	m_PullLeftCollider = CreateComponent<CColliderBox2D>("PullLeftCollider");
	m_PullLeftCollider->SetCollisionProfile("Player");
	m_PullLeftCollider->Start(); // SceneCollision �� Collider List �� �߰��ϱ�
	m_PullLeftCollider->Enable(false);
	m_PullLeftCollider->AddCollisionCallback(Collision_State::Begin, this, &CPlayerOld::PullLeftCollisionBeginCallback);
	m_PullLeftCollider->AddCollisionCallback(Collision_State::End, this, &CPlayerOld::PullLeftCollisionEndCallback);
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

	// Root Component Animation ����
	// CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Fight", TEXT("Kirby_Fight.anim"));
	// m_KirbyState->SetAnimationInstance(AnimationInstance);

	// Widget Component�� Widget ����
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
	// todo : �� ���⼭ ���Ӱ� ������ָ� �۵��� ���ϰ� �Ǵ� ���ϱ� ?
	if (!m_Camera)
	{
		m_Camera = CreateComponent<CCameraComponent>("Camera");
		m_Camera->OnViewportCenter();
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
		m_KirbyState->AddChild(m_Camera);
	}
	*/


	// Key Input ���� 
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveUp",
		KeyState_Push, this, &CPlayerOld::MoveUp);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveUp",
		KeyState_Up, this, &CPlayerOld::MoveUpEnd);

	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveDown",
		KeyState_Push, this, &CPlayerOld::MoveDown);

	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveLeft",
		KeyState_Push, this, &CPlayerOld::MoveLeft);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveLeft",
		KeyState_Up, this, &CPlayerOld::LeftLeverMoveEnd);

	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveDashLeft",
		KeyState_Push, this, &CPlayerOld::MoveDashLeft);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveDashLeft",
		KeyState_Up, this, &CPlayerOld::LeftDashMoveEnd);

	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveRight",
		KeyState_Push, this, &CPlayerOld::MoveRight);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveRight",
		KeyState_Up, this, &CPlayerOld::RightLeverMoveEnd);

	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveDashRight",
		KeyState_Push, this, &CPlayerOld::MoveDashRight);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MoveDashRight",
		KeyState_Up, this, &CPlayerOld::RightDashMoveEnd);

	CInput::GetInst()->SetKeyCallback<CPlayerOld>("PullRight",
		KeyState_Push, this, &CPlayerOld::PullRight);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("PullRight",
		KeyState_Up, this, &CPlayerOld::PullRightEnd);

	CInput::GetInst()->SetKeyCallback<CPlayerOld>("PullLeft",
		KeyState_Push, this, &CPlayerOld::PullLeft);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("PullLeft",
		KeyState_Up, this, &CPlayerOld::PullLeftEnd);

	CInput::GetInst()->SetKeyCallback<CPlayerOld>("SpecialChange",
		KeyState_Push, this, &CPlayerOld::MoveDown);

	/*
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("Attack",
		KeyState_Down, this, &CPlayerOld::Attack);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("Attack1",
		KeyState_Down, this, &CPlayerOld::Attack1);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("Skill1",
		KeyState_Down, this, &CPlayerOld::Skill1);
	*/
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("MovePoint",
		KeyState_Down, this, &CPlayerOld::MovePointDown);

	CInput::GetInst()->SetKeyCallback<CPlayerOld>("Jump",
		KeyState_Down, this, &CPlayerOld::Jump);
	CInput::GetInst()->SetKeyCallback<CPlayerOld>("JumpWhileDash",
		KeyState_Down, this, &CPlayerOld::Jump);

	CInput::GetInst()->SetKeyCallback<CPlayerOld>("SpitOut",
		KeyState_Push, this, &CPlayerOld::SpitOut);

	// Animation Play Scale ����
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightEatIdle")->SetPlayTime(2.f);
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftEatIdle")->SetPlayTime(2.f);

	// Animation Loop ���� ����
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetLoop(false);

	// Jump �� �� ���� ���Ŀ���, Animation �� Fall ���·� �ٲ��
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetEndFunction(
		this, &CPlayerOld::ChangePlayerFallAnimation);
	m_KirbyState->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetEndFunction(
		this, &CPlayerOld::ChangePlayerFallAnimation);
}

void CPlayerOld::Update(float DeltaTime)
{
	CLifeObject::Update(DeltaTime);

	PlayerMoveUpdate(DeltaTime);

	FallFromCliff();

	ChangeToIdleWhenReachGroundAfterFall();

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

		m_KirbyState->SetOpacity(m_Opacity);
	}
}

void CPlayerOld::PostUpdate(float DeltaTime)
{
	CLifeObject::PostUpdate(DeltaTime);

	CheckOutsideWorldResolution();

	m_DeltaTime = 0.f;
}

CPlayerOld* CPlayerOld::Clone()
{
	return new CPlayerOld(*this);
}

void CPlayerOld::SetIsEatingMonster(bool Enable)
{
	m_IsEatingMonster = Enable;

	if (Enable)
	{
		// Animation Change
		ChangePlayerIdleAnimation();
	}
}

void CPlayerOld::SetEatenMonster(CMonster* Monster)
{
	m_EatenMonster = Monster;
}

void CPlayerOld::UpdateWhileOffGround(float DeltaTime)
{
	CLifeObject::UpdateWhileOffGround(DeltaTime);
}

void CPlayerOld::MoveUp(float DeltaTime)
{
	FlyAfterJump(DeltaTime);
	// m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime);
}

void CPlayerOld::MoveUpEnd(float DeltaTime)
{
	if (m_IsFlying)
	{
		// ���� ���� ���¶��, m_IsFlying�� false �� �ؼ�
		// �ٽ� ������, Jump ���Ŀ� �� �� �ְ� �����Ѵ�
		// if (m_IsGround)
		m_IsFlying = false;

		ChangePlayerFallAnimation();
	}

	// Fall StartY �� ���⼭ �ٽ� �����Ѵ�.
	m_FallTime = 0.f;
	m_FallStartY = GetWorldPos().y;

	m_IsFalling = true;
}

void CPlayerOld::MoveDown(float DeltaTime)
{
	m_KirbyState->AddRelativePos(m_KirbyState->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime * -1.f);
}

void CPlayerOld::MoveLeft(float DeltaTime) //
{
	// ������ ��ư�� ���� ���¶�� X
	if (m_RightMovePush)
		return;

	// ���� ���� �ϱ�
	float WorldPosLeftX = GetWorldPos().x - GetPivot().x * GetWorldScale().x;
	float WorldPosRightX = GetWorldPos().x + GetPivot().x * GetWorldScale().x;

	// ����� Pull �ϰ� �ִ� ���°� �ƴϹǷ� false �� �����Ѵ�.
	m_IsPulling = false;

	// �ﰢ ���� ���¿��ٸ� ��� �̵� ��Ų��
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



	// ���� �̵� ǥ��
	m_IsLeverMoving = true;

	// ���� ��ư ������ �� ǥ��
	m_LeftMovePush = true;

	// ���� ���� ǥ��
	m_ObjectMoveDir.x = 1.f * -1;

	// �̵� ���� ���� ǥ��
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

	// �Ϲ� �̵� �ӵ� ���
	CalculateLeverMoveSpeed(DeltaTime);

	// ����� ���� ���� Dash Move ���� �ƴ϶�� ���̹Ƿ�
	if (m_DashVelocity > 0.f)
	{
		m_DashVelocity -= m_DashMoveAccel;

		if (m_DashVelocity < 0.f)
			m_DashVelocity = 0.f;
	}

	// �� �̵� �ӵ��� ����Ѵ�
	CalculateTotalMoveSpeed(DeltaTime);

	// ���� �̵� ó���� �Ѵ�
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

	// Animation ��ȯ
	ChangePlayerWalkAnimation();
}

void CPlayerOld::MoveDashLeft(float DeltaTime)
{
	// ������ ��ư�� ������ �ִٸ�
	if (m_RightMovePush)
		return;

	// ���� ���� �ϱ�
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

	// �̵� ���� True + ��ư �̵� ���� True
	m_IsLeverMoving = true;
	m_IsDashMoving = true;

	m_LeftMovePush = true;

	// ���� ���� ǥ��
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

	// Animation ��ȯ
	ChangePlayerRunAnimation();

}

void CPlayerOld::MoveRight(float DeltaTime)
{

	// ���� Ű�� ������ �־��ٸ� �ȸ԰� �Ѵ�
	if (m_LeftMovePush)
		return;

	float WorldPosLeftX = GetWorldPos().x - GetPivot().x * GetWorldScale().x;
	float WorldPosRightX = GetWorldPos().x + GetPivot().x * GetWorldScale().x;

	// ����� Pull �ϰ� �ִ� ���°� �ƴϹǷ� false �� �����Ѵ�.
	m_IsPulling = false;

	// Triangle Jump ���̶�� �̵��� ��� �ϵ� + Ű����� �ȸ԰� �Ѵ�
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


	// �Ϲ� �̵� ǥ��
	m_IsLeverMoving = true;

	// ������ �̵� ǥ��
	m_RightMovePush = true;

	// ���� ���� ǥ��
	m_ObjectMoveDir.x = 1.f;

	// �̵� ���� Update
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

	// �̵� �ӵ� ���
	CalculateLeverMoveSpeed(DeltaTime);

	// �뽬 �ӵ� Update
	if (m_DashVelocity > 0.f)
	{
		m_DashVelocity -= m_DashMoveAccel;

		if (m_DashVelocity < 0.f)
			m_DashVelocity = 0.f;
	}

	// �� �̵� �ӵ� ���
	CalculateTotalMoveSpeed(DeltaTime);

	// ���� �̵� ����
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

	// Animation ��ȯ
	ChangePlayerWalkAnimation();

}

void CPlayerOld::MoveDashRight(float DeltaTime)
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

	m_IsDashMoving = true;

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

	// Animation ��ȯ
	ChangePlayerRunAnimation();

}

void CPlayerOld::LeftLeverMoveEnd(float DeltaTime)
{
	m_LeftMovePush = false;

	ResetMoveInfo();

	ChangePlayerIdleAnimation();

	PullLeftEnd(DeltaTime);
}

void CPlayerOld::RightLeverMoveEnd(float DeltaTime)
{
	m_RightMovePush = false;

	ResetMoveInfo();

	ChangePlayerIdleAnimation();

	PullRightEnd(DeltaTime);
}

void CPlayerOld::LeftDashMoveEnd(float DeltaTime)
{
	ChangePlayerWalkAnimation();

	m_LeftMovePush = false;

	ResetMoveInfo();

	PullLeftEnd(DeltaTime);
}

void CPlayerOld::RightDashMoveEnd(float DeltaTime)
{
	ChangePlayerWalkAnimation();

	m_RightMovePush = false;

	ResetMoveInfo();

	PullRightEnd(DeltaTime);
}

void CPlayerOld::SpitOut(float DeltaTime)
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

float CPlayerOld::CalculateLeverMoveSpeed(float DeltaTime)
{
	// ���������� �̵����̶��
	if (m_RightMove)
	{
		// ��� ������ �̵� ���̶��
		if (!m_ToLeftWhenRightMove)
		{
			m_LeverVelocity += m_LeverMoveAccel;
		}
		// �װ� �ƴ϶�, ���������� �̵� ���̴ٰ�, �������� �����ϴ� ���̶��
		else
		{
			m_LeverVelocity -= m_LeverMoveAccel * 2.f;
		}
	}

	// ���� ���� �̵����̶��
	else if (m_LeftMove)
	{
		// ��� ���� �̵� ���̶��
		if (!m_ToRightWhenLeftMove)
		{
			m_LeverVelocity += m_LeverMoveAccel;
		}
		// �������� �̵� ���̴ٰ�, ���������� �����ϴ� ���̶��
		else
		{
			m_LeverVelocity -= m_LeverMoveAccel * 2.f;
		}
	}

	// �ִ� �ӵ� ����
	if (m_LeverVelocity >= m_LeverMaxMoveVelocity)
	{
		m_LeverVelocity = m_LeverMaxMoveVelocity;
	}

	// �ּ� �ӵ� ����
	if (m_LeverVelocity < 0.f)
		m_LeverVelocity = 0.f;

	return m_LeverVelocity;
}

float CPlayerOld::CalculateDashMoveSpeed(float DeltaTime)
{
	// ���߿� ���ִٸ� Dash Move �� �������� �ʴ´�
	if (!m_IsGround)
		return m_LeverVelocity;

	// ���������� �̵� ���̶��
	if (m_RightMove)
	{
		// ��� ���������� �̵� ���̶��
		if (!m_ToLeftWhenRightMove)
		{
			m_DashVelocity += m_DashMoveAccel;
		}
		else
		{
			m_DashVelocity -= m_DashMoveAccel * 2.f;
		}
	}
	// �������� �̵� ���̶��
	else if (m_LeftMove)
	{
		// ��� �������� �̵� ���̶��
		if (!m_ToRightWhenLeftMove)
		{
			m_DashVelocity += m_DashMoveAccel;
		}
		else
		{
			m_DashVelocity -= m_DashMoveAccel * 2.f;
		}
	}

	// �ִ� ġ ����
	if (m_DashVelocity >= m_DashMaxMoveVelocity)
	{
		m_DashVelocity = m_DashMaxMoveVelocity;
	}

	// �ּ� ġ ����
	if (m_DashVelocity < 0.f)
		m_DashVelocity = 0.f;

	return m_DashVelocity;
}

float CPlayerOld::CalculateTotalMoveSpeed(float DeltaTime)
{
	m_MoveVelocity = m_LeverVelocity + m_DashVelocity;

	// �ִ� ġ ����
	if (m_MoveVelocity > m_MoveVelocityMax)
		m_MoveVelocity = m_MoveVelocityMax;

	// �ּ� ġ ����
	if (m_MoveVelocity <= 0.1f)
	{
		// ���� ������, ���� �̵� ǥ�� ���¸� �������ش�.
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

void CPlayerOld::PlayerMoveUpdate(float DeltaTime)
{
	// ���� ������ --> ���� �������� �ʰ� �ִ� ��Ȳ�̶��, �ڿ� ������ ������� �Ѵ�.
	// �ִ� �������� ��������� �Ѵ�.
	if (m_MoveVelocity >= m_MoveVelocityMax)
	{
		m_MoveVelocity = m_MoveVelocityMax;
	}

	// ������ ������ ���� �ʴٸ�, ���� �ӵ��� ���� ��Ų��.
	if (!m_IsLeverMoving)
	{
		m_LeverVelocity -= m_LeverMoveAccel;

		if (m_LeverVelocity <= 0.f)
			m_LeverVelocity = 0.f;
	}

	// �뽬�� �����ϰ� ���� �ʴٸ�, �뽬 �ӵ��� ���� ��Ų��.
	if (!m_IsDashMoving)
	{
		m_DashVelocity -= m_DashMoveAccel;

		if (m_DashVelocity <= 0.f)
			m_DashVelocity = 0.f;
	}

	// ��ü �ӵ� ������ �����Ѵ�.
	if (!m_IsLeverMoving && !m_IsDashMoving && m_MoveVelocity > 0.f)
	{
		// ����
		m_MoveVelocity = m_LeverVelocity + m_DashVelocity;

		// �̵��� ���߸� -> ������ , ���� �̵� ǥ�ø� ������ش�.
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

		// Animation ����
		// ChangePlayerWalkAnimation();

		// ���� ���� �ϱ�
		float WorldPosLeftX = GetWorldPos().x - GetPivot().x * GetWorldScale().x;
		float WorldPosRightX = GetWorldPos().x + GetPivot().x * GetWorldScale().x;


		// ���� ���ӿ��� �̵��� ������� �Ѵ�.
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

	// �ӵ��� ���� + ���� ���°� �ƴ϶�� --> Idle Animation���� �ٲ��ش�.
	/*
	if (m_MoveVelocity <= 0.f && !m_Jump)
	{
		ChangePlayerIdleAnimation();
	}
	*/
}

void CPlayerOld::ResetMoveInfo()
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

void CPlayerOld::RotationZInv(float DeltaTime) //
{
	m_KirbyState->AddRelativeRotationZ(180.f * DeltaTime);
}

void CPlayerOld::RotationZ(float DeltaTime)
{
	m_KirbyState->AddRelativeRotationZ(-180.f * DeltaTime);
}

void CPlayerOld::FlyAfterJump(float DeltaTime)
{
	// �ϴ� �ѹ� �� ���¿��� ���ƾ� �Ѵ�.
	/*
	if (m_Jump)
	{
		m_IsFlying = true;

		// �߷� ���� ����
		m_Jump = false;
		m_IsGround = true;
	}
	*/

	m_IsFlying = true;

	// �ﰢ ���� ����
	m_TriangleJump = false;

	// �߷� ���� ����
	m_Jump = false;
	m_IsGround = true;

	// ���� ���̶��, ��� ���� �����Ѵ�
	if (m_IsFlying)
	{
		m_KirbyState->AddWorldPos(Vector3(0.f, 1.f, 0.f) * m_FlySpeed * DeltaTime);

		// todo : Animation Change
		ChangePlayerFlyAnimation();

		// �߷� ���� ����
		m_Jump = false;
		m_IsGround = true;
	}
}

void CPlayerOld::SimpleJump()
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

void CPlayerOld::Jump(float DeltaTime)
{
	// �ﰢ �浹 �����ϱ�
	bool SideCollision = false;

	CTileEmptyComponent* TileMap = m_Scene->GetTileEmptyComponent();

	Vector3 WorldPos = GetWorldPos();
	Vector3 WorldScale = GetWorldScale();
	Vector3 Pivot = GetPivot();

	Vector3 LB = WorldPos - Pivot * WorldScale;
	Vector3 RT = LB + WorldScale;

	// �������̸�, ������ ��ĭ Ÿ��
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

				Vector3 TilePos = TileMap->GetTileEmpty(IndexFinal)->GetWorldPos();
				Vector3 TileSize = TileMap->GetTileEmpty(IndexFinal)->GetSize();

				// ���� ���� �� �پ��ִٸ� �����Ѵ�
				if (TilePos.y + TileSize.y - m_GroundOffSet <= LB.y &&
					LB.y <= TilePos.y + TileSize.y + m_GroundOffSet)
					continue;

				// ���� ��ġ Tile �� Wall �̰�, ���ʵ� Wall �̶��
				// �װ��� �ﰢ ������ �ƴϰ� �ȴ�. �׳� Tile�� ���������� �ִ� ���� ���ؼ��� �ﰢ ���� X
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

	// �������� �̵��ϴ� ���̶��
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

		// ���� 2���� Ÿ���� �����Ѵ�.
		for (int row = BottomIndexY; row <= TopIndexY; row++)
		{
			for (int col = IndexX; col >= IndexXLeft; col--)
			{
				int IndexFinal = row * TileMap->GetTileCountX() + col;

				if (TileMap->GetTileEmpty(IndexFinal)->GetTileType() != Tile_Type::Wall)
					continue;

				Vector3 TilePos = TileMap->GetTileEmpty(IndexFinal)->GetWorldPos();
				Vector3 TileSize = TileMap->GetTileEmpty(IndexFinal)->GetSize();

				// ���� ȭ�鿡 �� �پ��ִ� ��쿡�� �����Ѵ�.
				if (TilePos.y + TileSize.y - m_GroundOffSet <= LB.y &&
					TilePos.y + TileSize.y + m_GroundOffSet)
					continue;

				// ���� ��ġ Tile�� Wall �̰�, �����ʵ� Wall �̶��
				// ���������� Tile�� �����ִ� ��
				// �׷��� ���� ���ؼ��� �ﰢ ���� X
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
		// ���� �����̴� ���� �ݴ�� �������� �Ѵ�
		// ������
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

void CPlayerOld::TriangleJumpLeft(float DeltaTime)
{
	m_Jump = true;
	m_IsGround = false;

	// ���� ���� ���� ����
	m_FallStartY = GetWorldPos().y;

	// Fall Time �ʱ�ȭ
	m_FallTime = 0.f;

	m_TriangleJump = true;

	m_LeftMove = true;
	m_RightMove = false;

	m_ToLeftWhenRightMove = false;
	m_ToRightWhenLeftMove = false;
}

void CPlayerOld::TriangleJumpRight(float DeltaTime)
{
	m_Jump = true;
	m_IsGround = false;

	// ���� ���� ���� ����
	m_FallStartY = GetWorldPos().y;

	// Fall Time �ʱ�ȭ
	m_FallTime = 0.f;

	m_TriangleJump = true;

	m_RightMove = true;
	m_LeftMove = false;

	m_ToLeftWhenRightMove = false;
	m_ToRightWhenLeftMove = false;
}

void CPlayerOld::FallFromCliff()
{
	// �������� �� �������� ��
	if (m_FallStartY - GetWorldPos().y > 5.f && !m_IsBottomCollided && !m_IsFlying)
	{
		ChangePlayerFallAnimation();
	}
}

void CPlayerOld::ChangeToIdleWhenReachGroundAfterFall()
{
	if (m_IsBottomCollided)
	{
		// �浹�ϴ� ���� Animation �� Fall �̶��, Animation�� Idle��
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

void CPlayerOld::ChangePlayerIdleAnimation()
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

void CPlayerOld::ChangePlayerNormalIdleAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftIdle");
	else
		ChangeAnimation("RightIdle");
}

void CPlayerOld::ChangePlayerWalkAnimation()
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

void CPlayerOld::ChangePlayerNormalWalkAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftWalk");
	else
		ChangeAnimation("RightWalk");
}

void CPlayerOld::ChangePlayerHitAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftHit");
	else
		ChangeAnimation("RightHit");
}

void CPlayerOld::ChangePlayerRunAnimation()
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

void CPlayerOld::ChangePlayerNormalRunAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftRun");
	else
		ChangeAnimation("RightRun");
}

void CPlayerOld::ChangePlayerEatRunAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftEatWalk");
	else
		ChangeAnimation("RightEatWalk");
}

void CPlayerOld::ChangePlayerDeathAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftDeath");
	else
		ChangeAnimation("RightDeath");
}

void CPlayerOld::ChangePlayerAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftAttack");
	else
		ChangeAnimation("RightAttack");
}

void CPlayerOld::ChangePlayerFlyAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftFly");
	else
		ChangeAnimation("RightFly");
}

void CPlayerOld::ChangePlayerJumpAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftJump");
	else
		ChangeAnimation("RightJump");
}

void CPlayerOld::ChangePlayerFallAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftFall");
	else
		ChangeAnimation("RightFall");
}

void CPlayerOld::ChangePlayerEatIdleAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftEatIdle");
	else
		ChangeAnimation("RightEatIdle");
}

void CPlayerOld::ChangePlayerEatWalkAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		ChangeAnimation("LeftEatWalk");
	else
		ChangeAnimation("RightEatWalk");
}

void CPlayerOld::SetObjectLand()
{
	CLifeObject::SetObjectLand();

	m_TriangleJump = false;

	m_IsFlying = false;
}

void CPlayerOld::FallDownAttack(const CollisionResult& Result)
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

		// 3) Damage Font �ۼ�

		// Hit ���·� ����
		OwnerMonster->SetAIState(Monster_AI::Hit);

		// ���� Player �������� ���ư��� �ϱ�
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

void CPlayerOld::PullRight(float DeltaTime)
{
	m_KirbyState->GetAnimationInstance()->ChangeAnimation("RightPull");

	// ������ ���� �ȿ� �ִ��� Ȯ���ϱ�
	float PullDist = 0.f;

	m_PullRightCollider->Enable(true);

	m_IsPulling = true;

	m_ObjectMoveDir.x = 1.f;
}

void CPlayerOld::PullRightCollisionBeginCallback(const CollisionResult& Result)
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

void CPlayerOld::PullRightCollisionEndCallback(const CollisionResult& Result)
{
	if (m_PullingMonster)
	{
		m_PullingMonster->SetIsBeingPulled(false);
		m_PullingMonster->ResetPulledInfo();
		m_PullingMonster->SetCollisionDisable(false);
		m_PullingMonster = nullptr;
	}
}

void CPlayerOld::PullLeftCollisionEndCallback(const CollisionResult& Result)
{
	if (m_PullingMonster)
	{
		m_PullingMonster->SetIsBeingPulled(false);
		m_PullingMonster->ResetPulledInfo();
		m_PullingMonster->SetCollisionDisable(false);
		m_PullingMonster = nullptr;
	}
}

void CPlayerOld::SpecialChange(float DeltaTime)
{
	// �԰� �ִ� �༮�� ������ X
	if (!m_IsEatingMonster)
		return;

	// �ɷ� ���Ͱ� �ƴ϶�� Return
	if (m_EatenMonster->GetMonsterType() != Monster_Type::Ability)
		return;

}

void CPlayerOld::Attack()
{
	if (!m_KirbyState)
		return;
	// m_KirbyState->Attack();
}

void CPlayerOld::FallDownAttack()
{
	if (!m_KirbyState)
		return;
	// m_KirbyState->FallDownAttack();
}

void CPlayerOld::PullLeftCollisionBeginCallback(const CollisionResult& Result)
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

void CPlayerOld::ChangeAnimation(const std::string& AnimName)
{
	if (!m_KirbyState)
		return;
	if (!m_KirbyState->GetAnimationInstance())
		return;
	m_KirbyState->GetAnimationInstance()->ChangeAnimation(AnimName);
}

void CPlayerOld::PullRightEnd(float DeltaTime)
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

void CPlayerOld::PullLeft(float DeltaTime)
{
	m_KirbyState->GetAnimationInstance()->ChangeAnimation("LeftPull");

	// ������ ���� �ȿ� �ִ��� Ȯ���ϱ�
	float PullDist = 0.f;

	// �浹ü�� ũ�⸦ �÷����� ?
	m_PullLeftCollider->Enable(true);

	m_IsPulling = true;

	m_ObjectMoveDir.x = 1.f * -1;
}

void CPlayerOld::PullLeftEnd(float DeltaTime)
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

void CPlayerOld::Attack(float DeltaTime)
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

void CPlayerOld::Attack1(float DeltaTime)
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

void CPlayerOld::Skill1(float DeltaTime)
{
	CBulletCamera* BulletCamera = m_Scene->CreateGameObject<CBulletCamera>("BulletCamera");
	BulletCamera->SetWorldPos(m_Muzzle->GetWorldPos());
	BulletCamera->SetWorldRotation(GetWorldRot());
}

void CPlayerOld::MovePointDown(float DeltaTime)
{
	Vector2 MousePos = CInput::GetInst()->GetMouseWorld2DPos();

	Move(Vector3(MousePos.x, MousePos.y, 0.f));
}
