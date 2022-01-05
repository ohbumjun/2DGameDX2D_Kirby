#include "UIWindow.h"

CUIWindow::CUIWindow()
{
	SetTypeID<CUIWindow>();
}

CUIWindow::~CUIWindow()
{}

bool CUIWindow::Init()
{
	return true;
}

void CUIWindow::Start()
{
	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

void CUIWindow::Update(float DeltaTime)
{
	auto iter = m_WidgetList.begin();
	auto iterEnd = m_WidgetList.end();

	for (; iter != iterEnd;)
	{
		if ((*iter)->IsActive())
		{
			SAFE_DELETE((*iter));
			iter = m_WidgetList.erase(iter);
			iterEnd = m_WidgetList.end();
		}
		if ((*iter)->IsEnable())
			continue;
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
		if ((*iter)->IsActive())
		{
			SAFE_DELETE((*iter));
			iter = m_WidgetList.erase(iter);
			iterEnd = m_WidgetList.end();
		}
		if ((*iter)->IsEnable())
			continue;
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
		if ((*iter)->IsActive())
		{
			SAFE_DELETE((*iter));
			iter = m_WidgetList.erase(iter);
			iterEnd = m_WidgetList.end();
		}
		++iter;
	}

	iter = m_WidgetList.begin();
	iterEnd = m_WidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->IsEnable())
			continue;
		(*iter)->Render();
	}
}
