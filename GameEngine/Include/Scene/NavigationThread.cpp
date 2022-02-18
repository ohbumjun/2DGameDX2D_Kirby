#include "NavigationThread.h"

CNavigationThread::CNavigationThread()
{
	SetLoop(true);


}

CNavigationThread::~CNavigationThread()
{}

void CNavigationThread::CreateNavigationNode(CTileMapComponent* TileMap)
{}

void CNavigationThread::Run()
{
	CThread::Run();
}
