#include "MainWidget.h"

#include <Engine.h>

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

	m_TestImage = CreateUIWidget<CUIImage>("Image");

	m_TestImage->SetTexture("Image", TEXT("Number.bmp"));
	m_TestImage->SetPlayTime(2.f);

	CTexture* Texture = m_TestImage->GetWidgetImageInfo().m_Texture;
	Vector2 StartPos = Vector2(0.f, 0.f);
	float TextureWidth = (float)Texture->GetWidth();
	float TextureHeight = (float)Texture->GetHeight();
	Vector2 Size = Vector2(TextureWidth /10.f, (float)TextureHeight);

	for (int i = 0; i < 10; i++)
	{
		m_TestImage->AddAnimationFrameData(StartPos, Size);
		StartPos.x += TextureWidth / 10.f;
	}

	m_TestImage->SetPos(400.f, 100.f);
	m_TestImage->SetSize(200.f, 50.f);

	// SlideBar
	m_TestSlider = CreateUIWidget<CUISlideBar>("Slider");
	m_TestSlider->SetPos(400.f, 200.f);
	m_TestSlider->SetSize(40.f, 100.f);

	// Number Widget
	std::vector<TCHAR*> vecFileName;

	// Hour 
	for (int i = 0; i < 10; i++)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(FileName, TEXT("Number/1.NoRed0.%d.png"), i);
		vecFileName.push_back(FileName);
	}

	m_Hour = CreateUIWidget<CUINumberWidget>("Hour");
	m_Hour->SetTexture("Number", vecFileName);
	m_Hour->AddAnimationFrameData(10);

	for (int i = 0; i < 10; i++)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	vecFileName.clear();

	// Minute 
	for (int i = 0; i < 10; i++)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(FileName, TEXT("Number/1.NoRed0.%d.png"), i);
		vecFileName.push_back(FileName);
	}

	m_Minute= CreateUIWidget<CUINumberWidget>("Minute");
	m_Minute->SetTexture("Number", vecFileName);
	m_Minute->SetPos(100.f, 0.f);
	m_Minute->AddAnimationFrameData(10);

	for (int i = 0; i < 10; i++)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	vecFileName.clear();

	// Second 
	for (int i = 0; i < 10; i++)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];
		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(FileName, TEXT("Number/1.NoRed0.%d.png"), i);
		vecFileName.push_back(FileName);
	}

	m_Second = CreateUIWidget<CUINumberWidget>("Second");
	m_Second->SetTexture("Number", vecFileName);
	m_Second->SetPos(200.f, 0.f);
	m_Second->AddAnimationFrameData(10);

	for (int i = 0; i < 10; i++)
	{
		SAFE_DELETE_ARRAY(vecFileName[i]);
	}

	// Frame
	m_FPSText = CreateUIWidget<CUIText>("FPSText");
	m_FPSText->SetText(TEXT("Button1"));
	m_FPSText->SetPos(900.f, 300.f);
	m_FPSText->SetSize(300.f, 40.f);
	m_FPSText->SetZOrder(1);
	m_FPSText->SetColor(1.f, 0.f, 0.f);
	m_FPSText->SetAlignH(TEXT_ALIGN_H::Center);
	m_FPSText->SetShadowAlphaEnable(true);


	return true;
}

void CMainWidget::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	// Number Widget update
	SYSTEMTIME time;

	GetLocalTime(&time);

	m_Hour->SetNumber((int)time.wHour);
	m_Minute->SetNumber((int)time.wMinute);
	m_Second->SetNumber((int)time.wSecond);

	// Show Real Time FPS
	float FPS = CEngine::GetInst()->GetFPS();

	char FPSText[MAX_PATH] = {};
	sprintf_s(FPSText, "FPS : %.5f", FPS);

	TCHAR WFPSText[MAX_PATH] = {};

	int ConvertLength = MultiByteToWideChar(CP_ACP, 0, FPSText, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, FPSText, -1, WFPSText, MAX_PATH);

	m_FPSText->SetText(WFPSText);

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
