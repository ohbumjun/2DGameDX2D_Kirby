#include "ClientManager.h"

#ifdef _DEBUG

#pragma comment(lib, "GameEngine_Debug.lib")

#else

#pragma comment(lib, "GameEngine.lib")

#endif // _DEBUG

#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

int APIENTRY wWinMain(_In_ HINSTANCE     hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR        lpCmdLine,
                      _In_ int           nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(309);
	if (!CClientManager::GetInst()->Init(hInstance))
	{
		CClientManager::DestroyInst();
		return 0;
	}

	CClientManager::GetInst()->CreateDefaultSceneMode();

	int Ret = CClientManager::GetInst()->Run();

	CClientManager::DestroyInst();

	return Ret;
}
