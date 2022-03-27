#include "EmptyObjectHUD.h"
CEmptyObjectHUD::CEmptyObjectHUD()
{}

CEmptyObjectHUD::CEmptyObjectHUD(const CEmptyObjectHUD& window) :
	CUIWindow(window)
{
}

CEmptyObjectHUD::~CEmptyObjectHUD()
{}


void CEmptyObjectHUD::Start()
{
	CUIWindow::Start();
}

bool CEmptyObjectHUD::Init()
{
	if (!CUIWindow::Init())
		return false;

	return true;
}

void CEmptyObjectHUD::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CEmptyObjectHUD::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CEmptyObjectHUD::Render()
{
	CUIWindow::Render();
}

CEmptyObjectHUD* CEmptyObjectHUD::Clone()
{
	return new CEmptyObjectHUD(*this);
}
