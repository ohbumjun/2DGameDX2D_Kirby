#include "MainWidget.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CMainWidget::CMainWidget()
{}

CMainWidget::~CMainWidget()
{}

void CMainWidget::Start()
{
	CUIWindow::Start();
}

bool CMainWidget::Init()
{
	if (!CUIWindow::Init())
		return false;

	SetSize(1280.f, 720.f);

	m_Button = CreateUIWidget<CUIButton>("Button");
	m_Button->SetPos(400.f, 300.f);
	m_Button->SetSize(200.f, 50.f);

	m_Button->SetTexture(Button_State::Normal,
		"StartButton", TEXT("Start.png"));
	m_Button->SetTexture(Button_State::MouseOn,
		"StartButton", TEXT("Start.png"));
	m_Button->SetTexture(Button_State::Click,
		"StartButton", TEXT("Start.png"));

	m_Button->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
	m_Button->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
	m_Button->SetTextureTint(Button_State::Click, 150, 150, 150, 255);

	m_Button->SetClickCallback<CMainWidget>(this, &CMainWidget::ButtonClick);
	// m_Button->SetMouseClickSound(m_Scene->GetResource()->FindSound("Fire"));
	// m_Button->SetMouseHoverSound(m_Scene->GetResource()->FindSound("Fire"));

	m_Button1 = CreateUIWidget<CUIButton>("TextButton");
	m_Button1->SetPos(50.f, 200.f);
	m_Button1->SetSize(100.f, 40.f);
	m_Button1->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
	m_Button1->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
	m_Button1->SetTextureTint(Button_State::Click, 150, 150, 150, 255);
	m_Text = CreateUIWidget<CUIText>("BasicText");
	m_Text->SetPos(50.f, 200.f);
	m_Text->SetSize(100.f, 40.f);
	m_Text->SetZOrder(1);
	m_Text->SetColor(1.f, 0.f, 0.f);
	m_Text->SetZOrder(1);
	m_Text->SetColor(1.f, 0.f, 0.f);
	m_Text->SetOpacity(0.5f);
	m_Text->SetAlignH(TEXT_ALIGN_H::Center);
	m_Text->SetShadowEnable(true);
	m_Text->SetShadowOffset(2.f, 2.f);

	return true;
}

void CMainWidget::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CMainWidget::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CMainWidget::Render()
{
	CUIWindow::Render();
}

void CMainWidget::ButtonClick()
{
	MessageBox(0, TEXT("Click"), TEXT("Click"), MB_OK);
}
