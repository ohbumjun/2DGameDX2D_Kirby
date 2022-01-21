#include "StartWidget.h"

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

	m_StartButton = CreateUIWidget<CUIButton>("StartButton");
	m_StartButton->SetTexture(Button_State::Normal, "StartButton", TEXT("Start.png"));
	m_StartButton->SetTexture(Button_State::MouseOn, "StartButton", TEXT("Start.png"));
	m_StartButton->SetTexture(Button_State::Click, "StartButton", TEXT("Start.png"));

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
{}

void CStartWidget::ExitButtonClick()
{}
