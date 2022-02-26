#include "NavigationThread.h"
#include "Navigation.h"
#include "NavigationManager.h"

CNavigationThread::CNavigationThread()
{
	SetLoop(true);

	m_Navigation = new CNavigation;

	m_ExitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

CNavigationThread::~CNavigationThread()
{
	WaitForSingleObject(m_ExitEvent, INFINITE);

	CloseHandle(m_ExitEvent);

	SAFE_DELETE(m_Navigation);
}

void CNavigationThread::CreateNavigationNode(CTileMapComponent* TileMap)
{
	m_Navigation->CreateNavigationNode(TileMap);
}

void CNavigationThread::Run()
{
	while (m_Loop)
	{
		if (!m_WorkQueue.empty())
		{
			NavWorkData Data = m_WorkQueue.front();

			m_WorkQueue.pop();

			NavResultData Result;

			m_Navigation->FindPath(Data.Start, Data.End, Result.vecPath);

			Result.Callback = Data.Callback;

			m_NavManager->AddResultData(Result);
		}
	}

	SetEvent(m_ExitEvent);
}
