#pragma once

#include "../GameInfo.h"
#include "../Component/TileMapComponent.h"
#include "../ThreadQueue.h"
#include "NavigationThread.h"

class CNavigationManager
{
	friend class CScene;

private :
	CNavigationManager();
	~CNavigationManager();

private :
	class CScene* m_Scene;
	std::vector<CNavigationThread*> m_vecNavigationThread;
	CSharedPtr<CTileMapComponent> m_NavData;
	CThreadQueue<NavResultData> m_ResultQueue;
public :
	void SetNavData(CTileMapComponent* NavData);
	void AddNavResult(const NavResultData& NavData);
public :
	bool Init();
	void Start();
	void Update(float DeltaTime);
public :
	
};

