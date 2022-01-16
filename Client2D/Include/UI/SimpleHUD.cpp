#include "SimpleHUD.h"

CSimpleHUD::CSimpleHUD()
{}

CSimpleHUD::CSimpleHUD(const CSimpleHUD& window) :
CUIWindow(window)
{
	m_NameText = FindUIWidget<CUIText>("NameText");
}

CSimpleHUD::~CSimpleHUD()
{}

void CSimpleHUD::SetText(const TCHAR* Text)
{
	m_NameText->SetText(Text);

}

void CSimpleHUD::Start()
{
	CUIWindow::Start();
}

bool CSimpleHUD::Init()
{
	if (!CUIWindow::Init())
		return false;

	SetSize(100.f, 50.f);

	m_NameText = CreateUIWidget<CUIText>("NameText");

	m_NameText->SetText(TEXT("Name!"));
	m_NameText->SetPos(0.f, 20.f);
	m_NameText->SetSize(100.f, 30.f);
	m_NameText->SetFontSize(13.f);
	m_NameText->SetZOrder(1);
	m_NameText->SetColor(1.f, 1.f, 1.f);
	m_NameText->SetAlignH(TEXT_ALIGN_H::Center);

	return true;
}

void CSimpleHUD::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CSimpleHUD::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CSimpleHUD::Render()
{
	CUIWindow::Render();
}

CSimpleHUD* CSimpleHUD::Clone()
{
	return new CSimpleHUD(*this);
}
