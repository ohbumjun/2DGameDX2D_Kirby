#include "Player2D.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"
#include "Component/SceneComponent.h"
#include "Component/NavAgent.h"
#include "Component/ColliderBox2D.h"
#include "Component/WidgetComponent.h"
#include "Component/CameraComponent.h"
#include "Component/NormalKirbyState.h"
#include "Component/NormalKirbyState.cpp"
#include "Component/KirbyState.h"
#include "Component/KirbyState.cpp"



CPlayer2D::CPlayer2D() 
{
	SetTypeID<CPlayer2D>();
}

CPlayer2D::CPlayer2D(const CPlayer2D& Obj) : CLifeObject(Obj)
{
}

CPlayer2D::~CPlayer2D()
{
	
}

bool CPlayer2D::Init()
{
	if (!CLifeObject::Init())
		return false;

	m_KirbyState = CreateComponent<CNormalKirbyState>("PlayerSprite");
	// m_KirbyState  = CreateComponent<CFightKirbyState>("PlayerSprite");

	// Collider 
	m_Body = CreateComponent<CColliderBox2D>("Body");
	m_Body->SetCollisionProfile("Player");

	// Widget
	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");

	// Camera
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Camera->OnViewportCenter(); // Player �߽� ����

	// NavAgent
	m_NavAgent = CreateComponent<CNavAgent>("NavAgent");

	SetRootComponent(m_KirbyState);
	m_KirbyState->AddChild(m_Body);
	m_KirbyState->AddChild(m_Camera);
	m_KirbyState->AddChild(m_SimpleHUDWidget);

	// �ݵ�� �� ��ġ���� ( AddChild ���Ŀ� ��ġ ��ǥ ���� )
	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	m_KirbyState->SetTransparency(true);

	// Pivot ���� ���ٸ�, ������ pos �� ���� �ϴ� pos �� �߽�����
	// Center �� �����ǰ� �� ���̴�. 
	m_KirbyState->SetRelativeScale(80.f, 80.f, 1.f);
	m_KirbyState->SetRelativePos(100.f, 50.f, 0.f);
	m_KirbyState->SetPivot(0.5f, 0.5f, 0.f);

	// Sprite ũ�� ����
	return true;
}

void CPlayer2D::Update(float DeltaTime)
{
	CLifeObject::Update(DeltaTime);
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CLifeObject::PostUpdate(DeltaTime);
	
}
CPlayer2D* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::Start()
{
	CLifeObject::Start();

	// Editor �� Player2D �Լ���
	// CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveUp", KeyState_Push, this, &CPlayer2D::MoveUp);
	// CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDown", KeyState_Push, this, &CPlayer2D::MoveDown);
	// CInput::GetInst()->SetKeyCallback<CPlayer2D>("RotationZInv", KeyState_Push, this, &CPlayer2D::AddRotationZInv);
	// CInput::GetInst()->SetKeyCallback<CPlayer2D>("RotationZ", KeyState_Push, this, &CPlayer2D::AddRotationZ);
}

void CPlayer2D::Save(FILE* pFile)
{
	CLifeObject::Save(pFile);
}

void CPlayer2D::Load(FILE* pFile)
{
	CLifeObject::Load(pFile);
}
