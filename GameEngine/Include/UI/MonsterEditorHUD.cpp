#include "MonsterEditorHUD.h"

CMonsterEditorHUD::CMonsterEditorHUD()
{}

CMonsterEditorHUD::~CMonsterEditorHUD()
{}

void CMonsterEditorHUD::SetText(const TCHAR* Text)
{
	m_NameText->SetText(Text);
}

void CMonsterEditorHUD::Start()
{
	CUIWindow::Start();
}

bool CMonsterEditorHUD::Init()
{
	if (!CUIWindow::Init())
		return false;

	SetSize(100.f, 50.f);

	m_NameText = CreateUIWidget<CUIText>("NameText");

	m_NameText->SetText(TEXT("Name!"));
	m_NameText->SetPos(-150.f, 35.f);
	m_NameText->SetSize(300.f, 100.f);
	m_NameText->SetFontSize(35.f);
	m_NameText->SetZOrder(1);
	m_NameText->SetColor(0.5f, 0.5f, 0.5f);
	m_NameText->SetAlignH(TEXT_ALIGN_H::Center);

	return true;
}

void CMonsterEditorHUD::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CMonsterEditorHUD::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CMonsterEditorHUD::Render()
{
	CUIWindow::Render();
}
