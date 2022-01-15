
#include "WidgetComponent.h"
#include "CameraComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "../Device.h"

CWidgetComponent::CWidgetComponent()
{
	SetTypeID<CWidgetComponent>();

	m_Space = WidgetComponent_Space::Screen;

	SetInheritRotX(false);
	SetInheritRotY(false);
	SetInheritRotZ(false);
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& Component) :
CSceneComponent(Component)
{
	m_Space = Component.m_Space;

	if (m_Space == WidgetComponent_Space::Screen)
	{
		SetInheritRotX(false);
		SetInheritRotY(false);
		SetInheritRotZ(false);
	}
	else
	{
		SetInheritRotX(true);
		SetInheritRotY(true);
		SetInheritRotZ(true);
	}

	m_WidgetWindow = Component.m_WidgetWindow->Clone();
	m_WidgetWindow->m_OwnerComponent = this;
}

CWidgetComponent::~CWidgetComponent()
{}

void CWidgetComponent::Start()
{
	CSceneComponent::Start();
}

bool CWidgetComponent::Init()
{
	SetLayerName("ScreenWidgetComponent");
	return true;
}

void CWidgetComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (m_WidgetWindow)
		m_WidgetWindow->Update(DeltaTime);
}

void CWidgetComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	if (m_WidgetWindow)
	{
		// World 공간 위치 정보로, WidgetComponent 위치 정보는 갱신되어 있을 것이다.
		// 이를 화면 공간에서의 위치로 변환 시켜줘야 한다.
		// 월드 -> 뷰 -> 투영 변환 -> Screen을 거치면, 화면 공간으로 변환이 된다. 
		// 화면 공간으로 변환될 때에는, 변환된 x,y,z,w 가 있을 경우
		// 모든 요소들을 w로 나누어서 화면공간으로 변환을 시도한다.
		CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

		Matrix matView = Camera->GetViewMatrix();
		Matrix matProj = Camera->GetProjMatrix();
		Matrix matVP = matView * matView;

		Vector3 WorldPos = GetWorldPos();

		Vector3 ProjPos = WorldPos.TransformCoord(matVP);

		float ViewZ = WorldPos.TransformCoord(matView).z;

		ProjPos.x /= ViewZ; //-1에서 1 사이의 값으로 나오게 될 것이다.
		ProjPos.y /= ViewZ;
		ProjPos.z /= ViewZ;

		Resolution RS = CDevice::GetInst()->GetResolution();

		// 마지막 화면 변환까지 해준다.
		Vector2 ScreenPos;
		ScreenPos.x = ProjPos.x * (RS.Width / 2.f) + (RS.Width / 2.f);
		ScreenPos.y = ProjPos.y * (RS.Width / -2.f) + (RS.Width / 2.f);

		m_WidgetWindow->SetPos(ScreenPos);
		m_WidgetWindow->PostUpdate(DeltaTime);
	}
}

void CWidgetComponent::CheckCollision()
{
	CSceneComponent::CheckCollision();
}

void CWidgetComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CWidgetComponent::Render()
{
	CSceneComponent::Render();
}

void CWidgetComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CWidgetComponent* CWidgetComponent::Clone()
{
	return new CWidgetComponent(*this);
}

void CWidgetComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);
}

void CWidgetComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);
}
