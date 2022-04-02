#include "PlayerHUD.h"

CPlayerHUD::CPlayerHUD()
{}

CPlayerHUD::~CPlayerHUD()
{}

void CPlayerHUD::SetText(const TCHAR* Text)
{}

void CPlayerHUD::Start()
{
	CUIWindow::Start();
}

bool CPlayerHUD::Init()
{
	if (!CUIWindow::Init())
		return false;

	SetSize(700.f, 400.f);

	SetPos(0.f, 350.f);

	// Name
	m_NameText = CreateUIWidget<CUIText>("NameText");

	m_NameText->SetText(TEXT("KIRBY"));
	m_NameText->SetPos(40.f, 300.f);
	m_NameText->SetSize(300.f, 60.f);
	m_NameText->SetFontSize(32.f);
	m_NameText->SetZOrder(1);
	m_NameText->SetColor(0.9f, 0.9f, 0.9f);
	m_NameText->SetAlignH(TEXT_ALIGN_H::Center);

	// Icon
	m_IconImage = CreateUIWidget<CUIImage>("IconImage");
	m_IconImage->SetTexture("NormalKirbyIconImage", TEXT("Project/UI/Icon_NormalKirby.png"));
	m_IconImage->SetSize(60.f, 60.f);
	m_IconImage->SetPos(20.f, 300.f);
	m_IconImage->SetZOrder(1);

	// HP ProgressBar
	m_HPProgressBar = CreateUIWidget<CUIProgressBar>("HPBar");
	m_HPProgressBar->SetSize(600.f, 90.f);
	m_HPProgressBar->SetPos(20.f, 260.f);
	m_HPProgressBar->SetTexture("PlayerHPProgressBar", TEXT("HPBar.png"));
	// m_HPProgressBar->SetTextureTint(255, 0, 0, 255);

	// MP ProgressBar
	m_MPProgressBar = CreateUIWidget<CUIProgressBar>("MPBar");
	m_MPProgressBar->SetSize(600.f, 90.f);
	m_MPProgressBar->SetPos(20.f, 220.f);
	m_MPProgressBar->SetTexture("PlayerMPProgressBar", TEXT("MPBar.png"));
	// m_MPProgressBar->SetTextureTint(255, 0, 0, 255);

	return true;
}

void CPlayerHUD::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CPlayerHUD::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CPlayerHUD::Render()
{
	CUIWindow::Render();
}
