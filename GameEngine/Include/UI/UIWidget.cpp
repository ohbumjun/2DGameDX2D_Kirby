#include "UIWidget.h"

CUIWidget::CUIWidget() :
	m_Scene(nullptr),
	m_Owner(nullptr)

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

void CUIWidget::Update(float DeltaTime) 
{}

void CUIWidget::PostUpdate(float DeltaTime)
{}

void CUIWidget::Render()
{}
