#include "IMGUIWidget.h"

CIMGUIWidget::CIMGUIWidget() :
	m_Size{150.f, 150.f},
	m_Color{1.f, 1.f, 1.f, 1.f},
	m_Font(nullptr)
{
}

CIMGUIWidget::~CIMGUIWidget()
{
}

bool CIMGUIWidget::Init()
{
	return false;
}

void CIMGUIWidget::Render()
{
}
