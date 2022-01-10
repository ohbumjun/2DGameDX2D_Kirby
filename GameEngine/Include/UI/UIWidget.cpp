#include "UIWidget.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"
#include "../Resource/Mesh/Mesh.h"

CUIWidget::CUIWidget() :
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_Start(false),
	m_CBuffer(nullptr),
	m_Shader(nullptr),
	m_Mesh(nullptr)


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
