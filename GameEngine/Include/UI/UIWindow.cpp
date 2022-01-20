#include "UIWindow.h"

#include "../Component/WidgetComponent.h"
#include "../Scene/Scene.h"

CUIWindow::CUIWindow() :
	m_ZOrder(0),
	m_ViewPort(nullptr),
	m_Scene(nullptr),
	m_Start(false),
	m_MoueHovered(false),
	m_CollisionMouseEnable(true)

{
	SetTypeID<CUIWindow>();
}

CUIWindow::CUIWindow(const CUIWindow& Window)
{
	*this = Window;
	m_OwnerComponent = nullptr;

	auto iter = Window.m_WidgetList.begin();
	auto iterEnd = Window.m_WidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		CUIWidget* Widget = (*iter)->Clone();
		Widget->m_Owner = this;
		m_WidgetList.push_back(Widget);
	}
}

CUIWindow::~CUIWindow()
{}

bool CUIWindow::CollisionMouse(const Vector2& MousePos)
{
	if (!m_CollisionMouseEnable)
		return false;

	if (m_Pos.x > MousePos.x)
		return false;

	else if (m_Pos.x + m_Size.x < MousePos.x)
		return false;

	else if (m_Pos.y > MousePos.y)
		return false;

	else if (m_Pos.y + m_Size.y < MousePos.y)
		return false;

	// Widget 정렬
	if (m_WidgetList.size() >= 2)
		m_WidgetList.sort(CUIWindow::SortWidget);

	// Widget 마우스 충돌 여부 세팅
	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;
		if ((*iter)->CollisionMouse(MousePos)) //
		{
			m_MoueHovered = true;
			return true;
		}
	}
	m_MoueHovered = false;
	return m_MoueHovered;
}

bool CUIWindow::Init()
{
	return true;
}

void CUIWindow::Start()
{
	m_Start = true;
	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	// 만약 WidgetComponent를 통해 만들어졌다면
	if (m_OwnerComponent)
	{
		m_ViewPort = m_OwnerComponent->GetScene()->GetViewPort();
	}
}

void CUIWindow::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			// SAFE_DELETE((*iter));
			iter = m_WidgetList.erase(iter);
			iterEnd = m_WidgetList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Update(DeltaTime);
		++iter;
	}
}

void CUIWindow::PostUpdate(float DeltaTime)
{
	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			// SAFE_DELETE((*iter));
			iter = m_WidgetList.erase(iter);
			iterEnd = m_WidgetList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}
}

void CUIWindow::Render()
{
	// Collision Mouse 에서 ZOrder 내림차순 정렬
	// ZOrdre가 높은 녀석이 위로 오게 해야 한다
	// 따라서, 뒤에서부터 그린다
	// 그래야 ZOrder 높은 애가 가장 마지막, 가장 화면 위에 그려지므로
	auto iter = m_WidgetList.rbegin();
	auto iterEnd = m_WidgetList.rend();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;
		(*iter)->Render();
	}
}

CUIWindow* CUIWindow::Clone()
{
	return new CUIWindow(*this);
}
