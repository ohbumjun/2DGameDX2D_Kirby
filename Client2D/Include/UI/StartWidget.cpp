#include "StartWidget.h"
#include "Device.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "../Scene/LoadingScene.h"

CStartWidget::CStartWidget() :
	m_StartButton(nullptr),
	m_ExitButton(nullptr),
	m_Back(nullptr)
{}

CStartWidget::CStartWidget(const CStartWidget& Widget) :
CUIWindow(Widget)
{}

CStartWidget::~CStartWidget()
{}

void CStartWidget::Start()
{
	CUIWindow::Start();
}

bool CStartWidget::Init()
{
	if (!CUIWindow::Init())
		return false;

	SetSize(1280.f, 720.f);

	Resolution RS = CDevice::GetInst()->GetResolution();

	m_StartButton = CreateUIWidget<CUIButton>("StartButton");
	m_StartButton->SetTexture(Button_State::Normal, "StartButton", TEXT("Start.png"));
	m_StartButton->SetTexture(Button_State::MouseOn, "StartButton", TEXT("Start.png"));
	m_StartButton->SetTexture(Button_State::Click, "StartButton", TEXT("Start.png"));

	Vector2 StartButtonSize = m_StartButton->GetWindowSize();
	m_StartButton->SetPos(RS.Width / 2.f - StartButtonSize.x / 2.f, RS.Height / 2.f + StartButtonSize.y);
	m_StartButton->SetClickCallback(this, &CStartWidget::StartButtonClick);

	m_ExitButton = CreateUIWidget<CUIButton>("ExitButton");
	m_ExitButton->SetTexture(Button_State::Normal, "ExitButton", TEXT("End.png"));
	m_ExitButton->SetTexture(Button_State::MouseOn, "ExitButton", TEXT("End.png"));
	m_ExitButton->SetTexture(Button_State::Click, "ExitButton", TEXT("End.png"));

	Vector2 EndButtonSize = m_ExitButton->GetWindowSize();
	m_ExitButton->SetPos(RS.Width / 2.f - StartButtonSize.x / 2.f, RS.Height / 2.f - StartButtonSize.y);
	m_ExitButton->SetClickCallback(this, &CStartWidget::ExitButtonClick);

	return true;
}

void CStartWidget::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CStartWidget::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CStartWidget::Render()
{
	CUIWindow::Render();
}

CStartWidget* CStartWidget::Clone()
{
	return new CStartWidget(*this);
}

void CStartWidget::StartButtonClick()
{
	CSceneManager::GetInst()->CreateNewScene();
	CSceneManager::GetInst()->CreateSceneMode<CLoadingScene>(false);

}

void CStartWidget::ExitButtonClick()
{
	CEngine::GetInst()->Exit();
}
