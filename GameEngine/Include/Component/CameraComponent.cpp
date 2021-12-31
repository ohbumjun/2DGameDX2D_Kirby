#include "CameraComponent.h"
#include "../Device.h"

CCameraComponent::CCameraComponent()
{
	SetTypeID<CCameraComponent>();

	// 이 녀석은 보이지 않게 세팅한다.
	m_Render = false;

	m_ViewAngle = 90.f;
	m_Distance = 1000.f;
}

CCameraComponent::CCameraComponent(const CCameraComponent& com) :
CSceneComponent(com)
{
	m_CameraType = com.m_CameraType;
	m_matProj = com.m_matProj;
	m_matView = com.m_matView;
	m_ViewAngle = com.m_ViewAngle;
	m_Distance = com.m_Distance;
	m_RS = com.m_RS;
}

CCameraComponent::~CCameraComponent()
{}

void CCameraComponent::CreateProjectionMatrix()
{
	
}

void CCameraComponent::Start()
{
	CSceneComponent::Start();

	// Start 할때 Create Projection Matrix를 사용한다
	CreateProjectionMatrix();
}

bool CCameraComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	m_RS = CDevice::GetInst()->GetResolution();
	return true;
}

void CCameraComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CCameraComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CCameraComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CCameraComponent::Render()
{
	CSceneComponent::Render();
}

void CCameraComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CCameraComponent* CCameraComponent::Clone()
{
	return new CCameraComponent(*this);
}

void CCameraComponent::Save(FILE* pFile)
{}

void CCameraComponent::Load(FILE* pFile)
{}
