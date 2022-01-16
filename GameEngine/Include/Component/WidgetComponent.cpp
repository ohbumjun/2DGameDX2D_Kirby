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
		Vector3 ViewPos = WorldPos.TransformCoord(matView);
		Vector3 ProjPos = ViewPos.TransformCoord(matProj);
		// 여기까지 하면 ProjPos의 경우, z로 나누기 전의 값이 나오게 된다.
		// 원근 투영일 경우, 그냥 z로 나눠버리면 된다.
		// 그런데 WidgetComponent의 경우, 고정된 UI 형태로 처리한다
		// 즉, 직교 투영을 적용해주어야 한다는 것이다.
		// 구체적으로 정리하자면,  WidgetComponent_Space::World !
		// 즉, 만약 World 상의 UI 라면, 원근 투영을 적용해야 하고
		// 그 말은 w 값, 4번째 원소 값으로 x,y,z를 나눠줘야 한다는 것을 의미한다.
		// 반면, 고정 UI 라면 ,
		// 즉, WidgetComponent_Space::Screen
		// 이라면, 원근 투영이 아니라, 직교 투영을 적용해야 한다.
		// 직교 투영에서는 w, 즉, z 좌표에 따른 비율이 의미가 없다.
		// 나눠줄 필요가 없다는 것이다.

		if (m_Space == WidgetComponent_Space::World)
		{
			// World 일 경우에는, w를 구한다
			// View Pos와, Projection 행렬의  4번째 열을 곱한 값이 w갑이 된다.
			float w = ViewPos.x * matProj._14 + ViewPos.y * matProj._24 + ViewPos.z * matProj._34 +  1 * matProj._44;

			// 만약 w가 1이 나온다면, 원본 좌표를 그대로 유지한다는 의미이다. 
			ProjPos.x /= w;
			ProjPos.y /= w;
			ProjPos.z /= w;
		}

		Resolution RS = CDevice::GetInst()->GetResolution();
		Vector2 ScreenPos;
		ScreenPos.x = ProjPos.x * (RS.Width / 2.f) + RS.Width / 2.f;
		ScreenPos.y = ProjPos.y * (RS.Height / 2.f) + RS.Height / 2.f;

		/*if (CEngine::GetInst()->GetEngineSpace() == Engine_Space::Space2D)
		{

		}

		else
		{
			ScreenPos.x = ProjPos.x * (RS.Width / 2.f) + (RS.Width / 2.f);
			ScreenPos.y = ProjPos.y * (RS.Height / -2.f) + (RS.Height / 2.f);
		}*/

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
