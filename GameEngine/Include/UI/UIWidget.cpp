#include "UIWidget.h"

CUIWidget::CUIWidget()
{
	SetTypeID<CUIWidget>();
}

CUIWidget::~CUIWidget()
{}

void CUIWidget::Start()
{}

bool CUIWidget::Init()
{
	return true;
}

void CUIWidget::Update(float DeltaTime) ////
{}

void CUIWidget::PostUpdate(float DeltaTime)
{}

void CUIWidget::Render()
{}
