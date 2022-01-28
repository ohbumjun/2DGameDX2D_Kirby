#include "EditorManager.h"

#ifdef _DEBUG

#pragma comment(lib, "GameEngine_Debug.lib")

#else

#pragma comment(lib, "GameEngine.lib")

#endif // _DEBUG


int APIENTRY wWinMain(_In_ HINSTANCE     hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR        lpCmdLine,
                      _In_ int           nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(448);

	if (!CEditorManager::GetInst()->Init(hInstance))
	{
		CEditorManager::DestroyInst();
		return 0;
	}

	CEditorManager::GetInst()->CreateDefaultSceneMode();

	int Ret = CEditorManager::GetInst()->Run();

	CEditorManager::DestroyInst();

	return Ret;
}
