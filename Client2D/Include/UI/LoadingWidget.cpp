#include "LoadingWidget.h"

CLoadingWidget::CLoadingWidget()
{}

CLoadingWidget::CLoadingWidget(const CLoadingWidget& Widget)
{}

CLoadingWidget::~CLoadingWidget()
{}

bool CLoadingWidget::Init()
{
	if (!CUIWindow::Init())
		return false;

	// CSharedPtr<CUIProgressBar> m_ProgressBar;
	// m_Back = CreateUIWidget<CUIImage>("BackImage");
	// m_Back->SetTexture("LoadingBackImage", TEXT("LoadingBack.jpg"));
	// m_Back->SetSize(1280.f, 720.f);

	m_Back = CreateUIWidget<CUIImage>("BackImage");
	m_Back->SetTexture("LoadingBackImage", TEXT("LoadingBack.jpg"));
	m_Back->SetSize(1280.f, 720.f);

	m_ProgressBar = CreateUIWidget<CUIProgressBar>("ProgressBar");
	m_ProgressBar->SetZOrder(1);
	m_ProgressBar->SetTexture("ProgressBar", TEXT("HPBar.png"));
	m_ProgressBar->SetPos(150.f, 100.f);
	m_ProgressBar->SetSize(980.f, 40.f);
	m_ProgressBar->SetTextureTint(255, 0, 0, 255);

	return true;
}
