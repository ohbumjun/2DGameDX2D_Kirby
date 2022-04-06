#include "IntroWidget.h"
#include "Scene/SceneManager.h"
#include "../Scene/BeginningScene.h"

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
	m_NormalKirbyImage->SetPos(100.f, 500.f);
	m_NormalKirbyImage->SetTexture("NormalUIKirby", TEXT("Project/UI/UIKirby.png"));
	m_NormalKirbyImage->SetAnimationFrameData("NormalUIKirbyAnim", TEXT("UI_Kirby_Normal.anim"));
	m_NormalKirbyImage->SetSize(200.f, 200.f);

	m_RadioKirbyImage = CreateUIWidget<CUIImage>("RadioKirbyImage");
	m_RadioKirbyImage->SetPos(500.f, 500.f);
	m_RadioKirbyImage->SetTexture("RadioKirby", TEXT("Project/UI/UIKirby.png"));
	m_RadioKirbyImage->SetAnimationFrameData("RadioUIKirbyAnim", TEXT("UI_Kirby_Radio.anim"));
	m_RadioKirbyImage->SetSize(200.f, 200.f);

	m_MovieKirbyImage = CreateUIWidget<CUIImage>("MovieKirbyImage");
	m_MovieKirbyImage->SetPos(50.f, 50.f);
	m_MovieKirbyImage->SetTexture("MovieKirby", TEXT("Project/UI/UIKirby.png"));
	m_MovieKirbyImage->SetAnimationFrameData("MovieUIKirbyAnim", TEXT("UI_Kirby_Movie.anim"));
	m_MovieKirbyImage->SetSize(250.f, 250.f);

	m_KirbyWithPenguinImage = CreateUIWidget<CUIImage>("PenguinKirbyImage");
	m_KirbyWithPenguinImage->SetPos(900.f, 450.f);
	m_KirbyWithPenguinImage->SetTexture("PenguinKirby", TEXT("Project/UI/UIKirby.png"));
	m_KirbyWithPenguinImage->SetAnimationFrameData("PenguinUIKirbyAnim", TEXT("UI_Kirby_WithPenguin.anim"));
	m_KirbyWithPenguinImage->SetSize(250.f, 250.f);

	m_KirbyBackGroundImage = CreateUIWidget<CUIImage>("BackGroundImage");
	m_KirbyBackGroundImage->SetTexture("BackGround", TEXT("Project/UI/IntroBackGround.jpg"));
	m_KirbyBackGroundImage->SetSize(1280.f, 720.f);

	m_StartLogo = CreateUIWidget<CUIImage>("StartLogo");
	m_StartLogo->SetTexture("StartLogo", TEXT("Project/UI/KirbyIntroLogo.png"));
	m_StartLogo->SetSize(500.f, 200.f);
	m_StartLogo->SetPos(350.f, 200.f);
	m_StartLogo->SetZOrder(1);

	// Start Button
	m_StartButton = CreateUIWidget<CUIButton>("StartButton");

	m_StartButton->SetPos(900.f, 50.f);
	m_StartButton->SetSize(200.f, 150.f);

	m_StartButton->SetTexture(Button_State::Normal, "IntroStartBtn", TEXT("Project/UI/IntroStartButton.png"));
	m_StartButton->SetTexture(Button_State::MouseOn, "IntroStartBtn", TEXT("Project/UI/IntroStartButton.png"));
	m_StartButton->SetTexture(Button_State::Click, "IntroStartBtn", TEXT("Project/UI/IntroBaIntroStartButtonckGround.png"));
	m_StartButton->SetZOrder(1);

	m_StartButton->SetTextureTint(Button_State::Normal, 220, 220, 220, 255);
	m_StartButton->SetTextureTint(Button_State::MouseOn, 255, 255, 255, 255);
	m_StartButton->SetTextureTint(Button_State::Click, 150, 150, 150, 255);

	m_StartButton->SetClickCallback<CIntroWidget>(this, &CIntroWidget::StartGameButtonClick);


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

void CIntroWidget::StartGameButtonClick()
{
	CSceneManager::GetInst()->CreateNewScene(false);

	CSceneManager::GetInst()->CreateSceneModeEmpty<CBeginningScene>(false);

	CSceneManager::GetInst()->GetNextScene()->PrepareResources();

	if (CSceneManager::GetInst()->GetNextScene()->Load("Beginning.scn", SCENE_PATH))
	{
		CSceneManager::GetInst()->ChangeNextScene();
	}
}
