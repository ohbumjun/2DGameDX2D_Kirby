#include "BackGroundWindow.h"

CBackGroundWindow::CBackGroundWindow()
{}

CBackGroundWindow::~CBackGroundWindow()
{}

void CBackGroundWindow::SetBackGround(CBackGroundComponent* TileMap)
{}

bool CBackGroundWindow::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	return true;
}

void CBackGroundWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CBackGroundWindow::SetEditModeCallback(int Index, const char* Name)
{}

void CBackGroundWindow::BackGroundImageLoadButton()
{}

void CBackGroundWindow::SetBackGroundScrollRatio()
{}
