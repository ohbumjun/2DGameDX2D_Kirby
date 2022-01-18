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

	SetInheritRotX(false);
	SetInheritRotY(false);
	SetInheritRotZ(false);

	m_Render = true;
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
{
}

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
		Matrix matView, matProj;
		Matrix matWP;

		CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();
		matView = Camera->GetViewMatrix();
		matProj = Camera->GetProjMatrix();
		matWP = matView * matProj;


		Vector3 WorldPos = GetWorldPos();
		Vector3 ViewPos  = WorldPos.TransformCoord(matView);
		Vector3 ProjPos   = ViewPos.TransformCoord(matProj);

		if (m_Space == WidgetComponent_Space::World)
		{
			float w = ViewPos.x * matProj._14 * ViewPos.y * matProj._24 + ViewPos.z * matProj._34 + 1 * matProj._44;

			ProjPos.x /= w;
			ProjPos.y /= w;
			ProjPos.z /= w;
		}

		Resolution RS = CDevice::GetInst()->GetResolution();

		Vector2 ScreenPos;
		ScreenPos.x = ProjPos.x * (RS.Width / 2.f) + (RS.Width / 2.f);
		ScreenPos.y = ProjPos.y * (RS.Height / 2.f) + (RS.Height / 2.f);

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

	if (m_WidgetWindow)
		m_WidgetWindow->Render();
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
