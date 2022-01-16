#include "WidgetComponent.h"
#include "CameraComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "../Device.h"

CWidgetComponent::CWidgetComponent() :
	m_WidgetWindow(nullptr)
{
	SetTypeID<CWidgetComponent>();

	m_Space = WidgetComponent_Space::Screen;
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

	if (Component.m_WidgetWindow)
	{
		m_WidgetWindow = Component.m_WidgetWindow->Clone();
		m_WidgetWindow->m_OwnerComponent = this;
	}
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
	if (m_WidgetWindow)
	{
		Matrix matView, matProj, matVP;

		CCameraComponent* cameraComponent = m_Scene->GetCameraManager()->GetCurrentCamera();
		matView = cameraComponent->GetViewMatrix();
		matProj = cameraComponent->GetProjMatrix();
		matVP = matView * matProj;

		Vector3 WorldPos = GetWorldPos();
		Vector3 ProjPos = WorldPos.TransformCoord(matVP);
		float ViewZ = WorldPos.TransformCoord(matView).z;

		ProjPos.x /= ViewZ;
		ProjPos.y /= ViewZ;
		ProjPos.z /= ViewZ;

		Resolution RS = CDevice::GetInst()->GetResolution();
		Vector2 ScreenPos;
		ScreenPos.x = ProjPos.x * (RS.Width / 2.f) + RS.Width / 2.f;
		ScreenPos.y = ProjPos.y * (RS.Height / 2.f) + RS.Height / 2.f;

		m_WidgetWindow->SetPos(ScreenPos);
		m_WidgetWindow->PostUpdate(DeltaTime);
	}

	CSceneComponent::PostUpdate(DeltaTime);
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
