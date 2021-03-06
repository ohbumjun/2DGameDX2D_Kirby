#include "ViewPort.h"
#include "../UI/UIWidget.h"
#include "../UI/UIWindow.h"
#include "../Input.h"

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
			// SAFE_DELETE((*iter));
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
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

void CViewPort::PostUpdate(float DeltaTime)
{
	auto iter = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			// SAFE_DELETE((*iter));
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
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

void CViewPort::Render()
{
	auto rIter = m_WindowList.rbegin();
	auto rIterEnd = m_WindowList.rend();

	for (; rIter != rIterEnd; ++rIter)
	{
		if (!(*rIter)->IsEnable())
			continue;
		(*rIter)->Render();
	}
}

bool CViewPort::CollisionMouse()
{
	Vector2 MousePos = CInput::GetInst()->GetMousePos();

	// 출력 우선순위가 높은 위젯부터
	// 충돌 처리를 시작한다.
	if (m_WindowList.size() >= 2)
		m_WindowList.sort(CViewPort::SortWindow);

	auto iter	  = m_WindowList.begin();
	auto iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}
		++iter;
	}

	iter = m_WindowList.begin();
	iterEnd = m_WindowList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;
		if ((*iter)->CollisionMouse(MousePos))
		{
			return true;
		}
	}

	return false;
}

void CViewPort::Collision()
{}

void CViewPort::StartBossHUDCallback()
{
	if (m_BossHUDStartCallback)
	{
		m_BossHUDStartCallback();
		m_BossHUDStartCallback = nullptr;
	}
}
