#include "ViewPort.h"
#include "../UI/UIWidget.h"
#include "../UI/UIWindow.h"

CViewPort::CViewPort() :
	m_Scene(nullptr)
{}

CViewPort::~CViewPort()
{}

bool CViewPort::Init()
{
	return true;
}

void CViewPort::Start()
{
	auto iter = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

void CViewPort::Update(float DeltaTime)
{
	auto iter = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_DELETE((*iter));
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
			continue;
		(*iter)->Update(DeltaTime);
		++iter;
	}
}

void CViewPort::PostUpdate(float DeltaTime)
{
	auto iter = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_DELETE((*iter));
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
			continue;
		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}
}

void CViewPort::Render()
{
	auto iter = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_DELETE((*iter));
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}
	}

	iter = m_WindowList.begin();
	iterEnd = m_WindowList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;
		(*iter)->Render();
	}
}
