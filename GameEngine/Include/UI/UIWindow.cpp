#include "UIWindow.h"

CUIWindow::CUIWindow() :
	m_ZOrder(0),
	m_ViewPort(nullptr),
	m_Scene(nullptr),
	m_Start(false),
	m_MoueHovered(false)

{
	SetTypeID<CUIWindow>();
}

CUIWindow::~CUIWindow()
{}

bool CUIWindow::CollisionMouse(const Vector2& MousePos)
{
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
		if ((*iter)->CollisionMouse(MousePos))
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
	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WidgetList.erase(iter);
			iterEnd = m_WidgetList.end();
		}
		++iter;
	}

	iter = m_WidgetList.begin();
	iterEnd = m_WidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;
		(*iter)->Render();
	}
}
