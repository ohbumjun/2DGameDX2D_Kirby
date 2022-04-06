#include "UltimateAttackWidget.h"
#include <Scene/Scene.h>
#include "Scene/CameraManager.h"
#include "Engine.h"
#include "Component/SpriteComponent.h"

CUltimateAttackWidget::CUltimateAttackWidget() :
	m_UIProceedTime(0.f)
{}

CUltimateAttackWidget::~CUltimateAttackWidget()
{}

void CUltimateAttackWidget::SetUITexture(const char* Name, const TCHAR* FileName)
{
	m_FrontImage->SetTexture(Name, FileName);
	m_FrontImage->SetSize(800.f, 720.f);
}


bool CUltimateAttackWidget::Init()
{
	if (!CUIWindow::Init())
		return false;

	Resolution RS = CEngine::GetInst()->GetResolution();
	m_RS = RS;

	m_FrontImage = CreateUIWidget<CUIImage>("AddImage");
	m_FrontImage->SetTexture("LoadingBackImage", TEXT("Project/Item/WhiteBack.png"));
	m_FrontImage->SetSize(800.f, 720.f);
	m_FrontImage->SetPos((float)RS.Width, 10.f);
	m_FrontImage->SetZOrder(1);

	return true;
}

void CUltimateAttackWidget::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	m_UIProceedTime += DeltaTime;

	float SpeedPerSec = (float)m_RS.Width * 1.5f;

	// if (m_UIProceedTime < m_UIProceedTimeMax)
	if (m_FrontImage->GetWindowPos().x > -800.f)
	{
		float MoveSpeed = -1.f;

		Vector2 CurrentPos = m_FrontImage->GetWindowPos();

		// if (m_UIProceedTime <= m_UIProceedTimeMax * 0.1f ||
		if (m_UIProceedTime <= m_UIProceedTimeMax * 0.1f ||
			m_UIProceedTime > m_UIProceedTimeMax * 0.9f)
		{
			// 0.1 초 동안, 30% 정도를 가야 한다.

			if (m_UIProceedTime >= m_UIProceedTimeMax * 0.1f)
			{
				Vector2 CurrentPos = m_FrontImage->GetWindowPos();
			}

			MoveSpeed = DeltaTime * SpeedPerSec * 3.f * -1.f;

		}
		else if (m_UIProceedTimeMax * 0.1f < m_UIProceedTime &&
			m_UIProceedTime <= m_UIProceedTimeMax * 0.9f)
		{
			// 반대로 0.8초 동안, 40% 정도를 가야 한다.
			// 즉, 0.1초에 5.xx% 정도를 가야 한다.
			// 1초에 0.5% 를 가야 한다.
			Vector2 CurrentPos = m_FrontImage->GetWindowPos();

			MoveSpeed = DeltaTime * SpeedPerSec * 0.5f * -1.f;
		}

		m_FrontImage->SetPos(CurrentPos.x + MoveSpeed,CurrentPos.y);
	}
	else
	{
		Destroy();
	}
}
