#include "MainWidget.h"

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

	return true;
}

void CMainWidget::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CMainWidget::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CMainWidget::Render()
{
	CUIWindow::Render();
}
