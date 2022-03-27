#include "IntroWidget.h"

CIntroWidget::CIntroWidget()
{}

CIntroWidget::~CIntroWidget()
{}

void CIntroWidget::Start()
{
	CUIWindow::Start();
}

bool CIntroWidget::Init()
{
	if (!CUIWindow::Init())
		return false;

	SetSize(1280.f, 720.f);

	m_NormalKirbyImage = CreateUIWidget<CUIImage>("NormalKirbyImage");
	m_NormalKirbyImage->SetPos(500.f, 500.f);
	m_NormalKirbyImage->SetTexture("NormalUIKirby", TEXT("Project/UI/UIKirby.png"));
	m_NormalKirbyImage->SetAnimationFrameData("NormalUIKirbyAnim", TEXT("UI_Kirby_Normal.anim"));
	m_NormalKirbyImage->SetSize(100.f, 100.f);

	m_RadioKirbyImage = CreateUIWidget<CUIImage>("RadioKirbyImage");
	m_RadioKirbyImage->SetPos(650.f, 500.f);
	m_RadioKirbyImage->SetTexture("RadioKirby", TEXT("Project/UI/UIKirby.png"));
	m_RadioKirbyImage->SetAnimationFrameData("RadioUIKirbyAnim", TEXT("UI_Kirby_Radio.anim"));
	m_RadioKirbyImage->SetSize(100.f, 100.f);

	m_MovieKirbyImage = CreateUIWidget<CUIImage>("MovieKirbyImage");
	m_MovieKirbyImage->SetPos(800.f, 500.f);
	m_MovieKirbyImage->SetTexture("MovieKirby", TEXT("Project/UI/UIKirby.png"));
	m_MovieKirbyImage->SetAnimationFrameData("MovieUIKirbyAnim", TEXT("UI_Kirby_Movie.anim"));
	m_MovieKirbyImage->SetSize(100.f, 100.f);

	m_KirbyWithPenguinImage = CreateUIWidget<CUIImage>("PenguinKirbyImage");
	m_KirbyWithPenguinImage->SetPos(950.f, 500.f);
	m_KirbyWithPenguinImage->SetTexture("PenguinKirby", TEXT("Project/UI/UIKirby.png"));
	m_KirbyWithPenguinImage->SetAnimationFrameData("PenguinUIKirbyAnim", TEXT("UI_Kirby_WithPenguin.anim"));
	m_KirbyWithPenguinImage->SetSize(100.f, 100.f);

	m_KirbyBackGroundImage = CreateUIWidget<CUIImage>("BackGroundImage");
	m_KirbyWithPenguinImage->SetTexture("BackGround", TEXT("Project/UI/IntroBackGround.png"));
	m_KirbyBackGroundImage->SetSize(1280.f, 720.f);

	return true;
}

void CIntroWidget::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CIntroWidget::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CIntroWidget::Render()
{
	CUIWindow::Render();
}
