#include "BossHUD.h"
#include "Scene/ViewPort.h"
#include <Scene/Scene.h>

CBossHUD::CBossHUD() :
	m_EnableStart(false),
	m_InitHPReachedEnd(false)
{}

CBossHUD::~CBossHUD()
{}

void CBossHUD::EnableBossHUD()
{
	Enable(true);

	m_EnableStart = true;
}

void CBossHUD::SetText(const TCHAR* Text)
{}

void CBossHUD::SetCallback()
{
	m_Scene->GetViewPort()->SetBossHUDStartCallback(this, &CBossHUD::EnableBossHUD);
}

void CBossHUD::Start()
{
	CUIWindow::Start();
}

bool CBossHUD::Init()
{
	if (!CUIWindow::Init())
		return false;

	SetSize(550.f, 400.f);

	SetPos(700.f, 0.f);

	// VS Icon Image
	m_VSIconImage = CreateUIWidget<CUIImage>("VSIconImage");
	m_VSIconImage->SetTexture("BossVSIconImage", TEXT("Project/UI/VSIcon_Hammer.png"));
	m_VSIconImage->SetPos(100.f, 100.f);
	m_VSIconImage->SetSize(180.f, 80.f);
	m_VSIconImage->SetZOrder(1);

	// Icon
	m_IconImage = CreateUIWidget<CUIImage>("IconImage");
	m_IconImage->SetTexture("BossIconImage", TEXT("Project/UI/Icon_Hammer_Big.png"));
	m_IconImage->SetSize(50.f, 50.f);
	m_IconImage->SetPos(20.f, 120.f);
	m_IconImage->SetZOrder(1);

	// HP ProgressBar
	m_HPProgressBar = CreateUIWidget<CUIProgressBar>("HPBar");
	m_HPProgressBar->SetTexture("BossHPProgressBar", TEXT("HPBar_Big.png"));
	m_HPProgressBar->SetSize(480.f, 50.f);
	m_HPProgressBar->SetPos(30.f, 52.f);
	m_HPProgressBar->SetZOrder(2);

	// 처음에는 0 % 에서 시작한다.
	m_HPProgressBar->SetPercent(0.f);

	// m_HPProgressBar->SetTextureTint(255, 0, 0, 255);

	m_HPProgressBarBack = CreateUIWidget<CUIProgressBar>("HPBarBack");
	m_HPProgressBarBack->SetTexture("BossHPProgressBarBack", TEXT("Project/UI/Boss_HP_UI.png"));
	m_HPProgressBarBack->SetSize(500.f, 120.f);
	m_HPProgressBarBack->SetPos(20.f, 35.f);
	m_HPProgressBarBack->SetZOrder(1);

	return true;
}

void CBossHUD::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	// 처음 UI 가 시작될 때, HP Progress Bar를 100 까지 끌어올린다.
	if (m_EnableStart && !m_InitHPReachedEnd)
	{
		float HPPercent = m_HPProgressBar->GetPercent();

		if (HPPercent >= 1.f)
		{
			m_InitHPReachedEnd = true;

			return;
		}

		m_HPProgressBar->SetPercent(HPPercent + DeltaTime * 0.5f);

	}
}

void CBossHUD::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CBossHUD::Render()
{
	CUIWindow::Render();
}
