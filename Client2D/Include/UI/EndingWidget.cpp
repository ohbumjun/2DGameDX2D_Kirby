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
	m_NormalKirbyImage->SetPos(100.f, 500.f);
	m_NormalKirbyImage->SetTexture("NormalUIKirby", TEXT("Project/UI/UIKirby.png"));
	m_NormalKirbyImage->SetAnimationFrameData("NormalUIKirbyAnim", TEXT("UI_Kirby_Normal.anim"));
	m_NormalKirbyImage->SetSize(200.f, 200.f);

	m_TrophyKirbyImage = CreateUIWidget<CUIImage>("RadioKirbyImage");
	m_TrophyKirbyImage->SetPos(500.f, 500.f);
	m_TrophyKirbyImage->SetTexture("RadioKirby", TEXT("Project/UI/UIKirby.png"));
	m_TrophyKirbyImage->SetAnimationFrameData("RadioUIKirbyAnim", TEXT("UI_Kirby_Radio.anim"));
	m_TrophyKirbyImage->SetSize(200.f, 200.f);

	m_FightBackGroundImage = CreateUIWidget<CUIImage>("BackGroundImage");
	m_FightBackGroundImage->SetTexture("BackGround", TEXT("Project/UI/IntroBackGround.jpg"));
	m_FightBackGroundImage->SetSize(1280.f, 720.f);

	m_FightForeGroundImage = CreateUIWidget<CUIImage>("BackGroundImage");
	m_FightForeGroundImage->SetTexture("BackGround", TEXT("Project/UI/IntroBackGround.jpg"));
	m_FightForeGroundImage->SetSize(1280.f, 720.f);

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

void CEndingWidget::StartGameButtonClick()
{
	CSceneManager::GetInst()->CreateNewScene(false);

	CSceneManager::GetInst()->CreateSceneModeEmpty<CBeginningScene>(false);

	CSceneManager::GetInst()->GetNextScene()->PrepareResources();

	if (CSceneManager::GetInst()->GetNextScene()->Load("Beginning.scn", SCENE_PATH))
	{
		CSceneManager::GetInst()->ChangeNextScene();
	}
}
