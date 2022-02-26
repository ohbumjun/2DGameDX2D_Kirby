#include "NavigationManager.h"

CNavigationManager::CNavigationManager()
{}

CNavigationManager::~CNavigationManager()
{
	size_t Size = m_vecNavigationThread.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecNavigationThread[i]);
	}

	m_vecNavigationThread.clear();
}

void CNavigationManager::SetNavTileMap(CTileMapComponent* NavTileMap)
{
	m_NavTileMap = NavTileMap;

	if (!m_vecNavigationThread.empty())
	{
		size_t Size = m_vecNavigationThread.size();

		for (size_t i = 0; i < Size; i++)
		{
			SAFE_DELETE(m_vecNavigationThread[i]);
		}

		m_vecNavigationThread.clear();
	}

	if (m_NavTileMap)
	{
		for (int i = 0; i < 4; i++)
		{
			char Name[MAX_PATH];

			sprintf_s(Name, "Thread%d", i);

			CNavigationThread* Thread = CThread::CreateThread<CNavigationThread>(Name);

			Thread->m_NavManager = this;

			Thread->CreateNavigationNode(m_NavTileMap);

			Thread->Start();

			m_vecNavigationThread.push_back(Thread);
		}
	}
}

void CNavigationManager::AddResultData(NavResultData Result)
{
	m_ResultQueue.push(Result);
}

void CNavigationManager::Start()
{}

bool CNavigationManager::Init()
{
	return true;
}

void CNavigationManager::Update(float DeltaTime)
{
	if (!m_ResultQueue.empty())
	{
		NavResultData Data = m_ResultQueue.front();

		m_ResultQueue.pop();

		Data.Callback(Data.vecPath);
	}
}
