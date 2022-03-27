#include "LoadingWidget.h"

CLoadingWidget::CLoadingWidget()
{}

CLoadingWidget::CLoadingWidget(const CLoadingWidget& Widget)
{}

CLoadingWidget::~CLoadingWidget()
{}

void CLoadingWidget::SetFrontTexture(const char* Name, const TCHAR* FileName)
{
	m_FrontImage->SetTexture(Name, FileName);
	m_FrontImage->SetPos(200.f, 140.f);
	// m_Back->SetSize(1280.f, 720.f);
	m_FrontImage->SetSize(880, 440.f);
}

void CLoadingWidget::SetBackTexture(const char* Name, const TCHAR* FileName)
{
	m_BackImage->SetTexture(Name, FileName);
	m_BackImage->SetSize(1280.f, 720.f);
}

bool CLoadingWidget::Init()
{
	if (!CUIWindow::Init())
		return false;

	// CSharedPtr<CUIProgressBar> m_ProgressBar;
	// m_Back = CreateUIWidget<CUIImage>("BackImage");
	// m_Back->SetTexture("LoadingBackImage", TEXT("LoadingBack.jpg"));
	// m_Back->SetSize(1280.f, 720.f);

	m_BackImage = CreateUIWidget<CUIImage>("BackImage");
	m_BackImage->SetTexture("LoadingBackImage", TEXT("Project/Item/WhiteBack.png"));
	m_BackImage->SetSize(1280.f, 720.f);
		
	m_FrontImage = CreateUIWidget<CUIImage>("AddImage");
	m_FrontImage->SetTexture("LoadingBackImage", TEXT("Project/Item/WhiteBack.png"));
	m_FrontImage->SetSize(1280.f, 720.f);
	m_FrontImage->SetZOrder(1);

	m_ProgressBar = CreateUIWidget<CUIProgressBar>("ProgressBar");
	m_ProgressBar->SetZOrder(1);
	m_ProgressBar->SetTexture("ProgressBar", TEXT("HPBar_Gradient.png"));
	m_ProgressBar->SetPos(220.f, 100.f);
	m_ProgressBar->SetSize(800.f, 30.f);
	m_ProgressBar->SetTextureTint(255, 255, 255, 235);

	return true;
}
