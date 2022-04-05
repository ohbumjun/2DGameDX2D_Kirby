#include "EndingWidget.h"
#include "Scene/SceneManager.h"
#include "../Scene/BeginningScene.h"

CEndingWidget::CEndingWidget()
{}

CEndingWidget::~CEndingWidget()
{}

void CEndingWidget::Start()
{
	CUIWindow::Start();
}

bool CEndingWidget::Init()
{
	if (!CUIWindow::Init())
		return false;

	SetSize(1280.f, 720.f);

	m_NormalKirbyImage = CreateUIWidget<CUIImage>("NormalKirbyImage");
	m_NormalKirbyImage->SetPos(100.f, 200.f);
	m_NormalKirbyImage->SetTexture("NormalUIKirby", TEXT("Project/UI/UIKirby.png"));
	m_NormalKirbyImage->SetAnimationFrameData("NormalUIKirbyAnim", TEXT("UI_Kirby_End_Normal.anim"));
	m_NormalKirbyImage->SetSize(200.f, 200.f);
	m_NormalKirbyImage->SetZOrder(2);

	m_TrophyKirbyImage = CreateUIWidget<CUIImage>("RadioKirbyImage");
	m_TrophyKirbyImage->SetPos(500.f, 200.f);
	m_TrophyKirbyImage->SetTexture("RadioKirby", TEXT("Project/UI/UIKirby.png"));
	m_TrophyKirbyImage->SetAnimationFrameData("RadioUIKirbyAnim", TEXT("UI_Kirby_End_Trophy.anim"));
	m_TrophyKirbyImage->SetSize(200.f, 200.f);
	m_TrophyKirbyImage->SetZOrder(2);

	m_FightBackGroundImage = CreateUIWidget<CUIImage>("BackGroundImage");
	m_FightBackGroundImage->SetTexture("BackGround", TEXT("Project/Scene/EndingBack.png"));
	m_FightBackGroundImage->SetSize(1280.f, 520.f);
	m_FightBackGroundImage->SetPos(0.f, 200.f);

	m_FightForeGroundImage = CreateUIWidget<CUIImage>("ForeGroundImage");
	m_FightForeGroundImage->SetTexture("ForeGround", TEXT("Project/Scene/EndingFront.png"));
	m_FightForeGroundImage->SetSize(1280.f, 520.f);
	m_FightForeGroundImage->SetPos(0.f, 0.f);
	m_FightForeGroundImage->SetZOrder(1);

	return true;
}

void CEndingWidget::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CEndingWidget::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CEndingWidget::Render()
{
	CUIWindow::Render();
}
