#include "ClientManager.h"
#include "Engine.h"
#include "Input.h"
#include "resource.h"
#include "Scene/MainScene.h"
#include "Scene/StartScene.h"
#include "Scene/SceneManager.h"
#include "UI/MouseNormal.h"
#include "UI/MouseAttack.h"

DEFINITION_SINGLE(CClientManager)

CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
	CEngine::DestroyInst();
}

bool CClientManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"),
	                              1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}

	CInput::GetInst()->CreateKey("MoveUp", 'W');
	CInput::GetInst()->CreateKey("MoveDown", 'S');
	CInput::GetInst()->CreateKey("RotationZInv", 'A');
	CInput::GetInst()->CreateKey("RotationZ", 'D');
	CInput::GetInst()->CreateKey("Attack", VK_SPACE);
	CInput::GetInst()->CreateKey("Attack1", VK_LBUTTON);
	CInput::GetInst()->CreateKey("Skill1", '1');

	// Sound
	CResourceManager::GetInst()->CreateChannelGroup("UI");

	// Mouse Á¦ÀÛ
	CEngine::GetInst()->CreateMouseUIWindow<CMouseNormal>(Mouse_State::Normal, "MouseNormal");
	CEngine::GetInst()->CreateMouseUIWindow<CMouseAttack>(Mouse_State::State1, "MouseNormal");

	return true;
}

void CClientManager::CreateDefaultSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CStartScene>();
	// CSceneManager::GetInst()->CreateSceneMode<CMainScene>();
}

int CClientManager::Run()
{
	return CEngine::GetInst()->Run();
}
