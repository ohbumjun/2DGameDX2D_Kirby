#include "IMGUIWidget.h"

CIMGUIWidget::CIMGUIWidget() :
    m_Font(nullptr),
    m_Color{ 1.f, 1.f, 1.f, 1.f },
    m_Size{ 150.f, 150.f }
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
