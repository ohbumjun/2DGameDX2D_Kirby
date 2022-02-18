#include "NavigationThread.h"
#include "Navigation.h"
#include "NavigationManager.h"

CNavigationThread::CNavigationThread()
{
	m_Navigation = new CNavigation;
}

CNavigationThread::~CNavigationThread()
{
	SAFE_DELETE(m_Navigation);
}

void CNavigationThread::CreateNavigationNode(CTileMapComponent* TileMap)
{
	if (!m_Navigation)
		return;

	m_Navigation->CreateNavigationNode(TileMap);
}

void CNavigationThread::Run()
{
	CThread::Run();

	while (m_Loop)
	{
		NavWorkData Data = m_WorkQueue.front();
		m_WorkQueue.pop();

		NavResultData Result;

		m_Navigation->FindPath(Data.Start, Data.End, Result.vecPath);

		Result.Callback = Data.Callback;

		m_NavManager->AddNavResult(Result);
	}
}
