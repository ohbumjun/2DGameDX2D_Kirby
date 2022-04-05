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

	m_NormalKirbyLeftImage = CreateUIWidget<CUIImage>("NormalKirbyImage");
	m_NormalKirbyLeftImage->SetPos(200.f, 200.f);
	m_NormalKirbyLeftImage->SetTexture("NormalUIKirby", TEXT("Project/UI/UIKirby.png"));
	m_NormalKirbyLeftImage->SetAnimationFrameData("NormalUIKirbyAnim", TEXT("UI_Kirby_End_Normal.anim"));
	m_NormalKirbyLeftImage->SetSize(200.f, 200.f);
	m_NormalKirbyLeftImage->SetZOrder(2);

	m_TrophyKirbyImage = CreateUIWidget<CUIImage>("RadioKirbyImage");
	m_TrophyKirbyImage->SetPos(550.f, 200.f);
	m_TrophyKirbyImage->SetTexture("RadioKirby", TEXT("Project/UI/UIKirby.png"));
	m_TrophyKirbyImage->SetAnimationFrameData("RadioUIKirbyAnim", TEXT("UI_Kirby_End_Trophy.anim"));
	m_TrophyKirbyImage->SetSize(300.f, 300.f);
	m_TrophyKirbyImage->SetZOrder(2);

	m_NormalKirbyRightImage = CreateUIWidget<CUIImage>("NormalKirbyImage");
	m_NormalKirbyRightImage->SetPos(900.f, 200.f);
	m_NormalKirbyRightImage->SetTexture("NormalUIKirby", TEXT("Project/UI/UIKirby.png"));
	m_NormalKirbyRightImage->SetAnimationFrameData("NormalUIKirbyAnim", TEXT("UI_Kirby_End_Normal.anim"));
	m_NormalKirbyRightImage->SetSize(200.f, 200.f);
	m_NormalKirbyRightImage->SetZOrder(2);

	m_FightBackGroundImage = CreateUIWidget<CUIImage>("BackGroundImage");
	m_FightBackGroundImage->SetTexture("BackGround", TEXT("Project/Scene/EndingBack.png"));
	m_FightBackGroundImage->SetSize(1280.f, 520.f);
	m_FightBackGroundImage->SetPos(0.f, 200.f);

	m_FightForeGroundImage = CreateUIWidget<CUIImage>("ForeGroundImage");
	m_FightForeGroundImage->SetTexture("ForeGround", TEXT("Project/Scene/EndingFront.png"));
	m_FightForeGroundImage->SetSize(1280.f, 520.f);
	m_FightForeGroundImage->SetPos(0.f, 0.f);
	m_FightForeGroundImage->SetZOrder(1);

	m_BlackToLeft = CreateUIWidget<CUIImage>("BlackLImage");
	m_BlackToLeft->SetTexture("BlackToLeft", TEXT("Project/UI/Black.jpg"));
	m_BlackToLeft->SetSize(1280.f, 720.f);
	m_BlackToLeft->SetPos(-640.f, 0.f);
	m_BlackToLeft->SetZOrder(5);

	m_BlackToRight = CreateUIWidget<CUIImage>("BlackRImage");
	m_BlackToRight->SetTexture("BlackToRight", TEXT("Project/UI/Black.jpg"));
	m_BlackToRight->SetSize(1280.f, 720.f);
	m_BlackToRight->SetPos(640.f, 0.f);
	m_BlackToRight->SetZOrder(5);

	return true;
}

void CEndingWidget::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	if (m_BlackToLeft->GetWindowPos().x >= -1290.f)
	{
		float NewXPos = m_BlackToLeft->GetWindowPos().x - 40.f * DeltaTime;
		m_BlackToLeft->SetPos(NewXPos, m_BlackToLeft->GetWindowPos().y);
	}

	if (m_BlackToRight->GetWindowPos().x <= 1290.f)
	{
		float NewXPos = m_BlackToRight->GetWindowPos().x + 40.f * DeltaTime;
		m_BlackToRight->SetPos(NewXPos, m_BlackToRight->GetWindowPos().y);
	}
}

void CEndingWidget::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CEndingWidget::Render()
{
	CUIWindow::Render();
}
