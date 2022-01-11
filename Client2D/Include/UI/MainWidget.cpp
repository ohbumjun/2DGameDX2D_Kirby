#include "MainWidget.h"

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

	m_Button->SetTextureTint(Button_State::Normal, 100, 100, 100, 255);
	m_Button->SetTextureTint(Button_State::MouseOn, 50, 50, 50, 255);
	m_Button->SetTextureTint(Button_State::Click, 20, 20, 100, 255);

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
