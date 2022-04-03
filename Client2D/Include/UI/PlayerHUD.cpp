#include "PlayerHUD.h"

CPlayerHUD::CPlayerHUD() :
	m_MPBarToggleMaxTime(0.5f),
	m_IsOpacityToggle(false)
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

	// HP ProgressBar Back
	m_HPProgressBarBack = CreateUIWidget<CUIImage>("HPBarBack");
	m_HPProgressBarBack->SetSize(590.f, 110.f);
	m_HPProgressBarBack->SetPos(15.f, 255.f);
	m_HPProgressBarBack->SetTexture("PlayerHPProgressBarBack", TEXT("BarBack_Silver.png"));

	// MP ProgressBar
	m_MPProgressBar = CreateUIWidget<CUIProgressBar>("MPBar");
	m_MPProgressBar->SetSize(600.f, 90.f);
	m_MPProgressBar->SetPos(20.f, 215.f);
	m_MPProgressBar->SetTexture("PlayerMPProgressBar", TEXT("MPBar.png"));
	// m_MPProgressBar->SetTextureTint(255, 0, 0, 255);

	// MP ProgressBar Back
	m_MPProgressBarBack = CreateUIWidget<CUIImage>("MPBarBack");
	m_MPProgressBarBack->SetSize(590.f, 110.f);
	m_MPProgressBarBack->SetPos(15.f, 210.f);
	m_MPProgressBarBack->SetTexture("PlayerMPProgressBarBack", TEXT("BarBack_Silver.png"));

	// m_MPProgressBarWhiteBack
	m_MPProgressBarWhiteBack = CreateUIWidget<CUIImage>("MPBarWhiteBack");
	m_MPProgressBarWhiteBack->SetSize(590.f, 110.f);
	m_MPProgressBarWhiteBack->SetPos(15.f, 210.f);
	m_MPProgressBarWhiteBack->SetTexture("PlayerMPProgressBarWhiteBack", TEXT("BarBack_White.png"));
	m_MPProgressBarWhiteBack->SetZOrder(1);
	m_MPProgressBarWhiteBack->SetOpacity(0.f);

	return true;
}

void CPlayerHUD::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	// 만약 MP가 가득 찼다면 주기적으로 반짝이게 해준다. (Opacity)
	if (m_MPProgressBar->GetPercent() == 1.f)
	{
		if (m_IsOpacityToggle)
		{
			m_MPProgressBarWhiteBack->SetOpacity(m_MPProgressBarWhiteBack->GetOpacity() - DeltaTime * 2.f);

			if (m_MPProgressBarWhiteBack->GetOpacity() <= 0.1f)
			{
				m_IsOpacityToggle = false;
			}
		}
		else
		{
			m_MPProgressBarWhiteBack->SetOpacity(m_MPProgressBarWhiteBack->GetOpacity() + DeltaTime * 2.f);

			if (m_MPProgressBarWhiteBack->GetOpacity() >= 0.99f)
			{
				m_IsOpacityToggle = true;
			}
		}
	}
	else
	{
		m_MPProgressBarWhiteBack->SetOpacity(0.f);
	}

}

void CPlayerHUD::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CPlayerHUD::Render()
{
	CUIWindow::Render();
}
