#include "EditorManager.h"
#include "Engine.h"
#include "IMGUIManager.h"
#include "Input.h"
#include "resource.h"
#include "Object/DragObject.h"
#include "Object/SpriteEditObject.h"
#include "Render/RenderManager.h"
#include "Scene/DefaultScene.h"
#include "Scene/SceneManager.h"
#include "Window/SpriteWindow.h"
#include "Window/EditorMenu.h"
#include "Window/ObjectHierarchy.h"
#include "Window/DetailInfoWindow.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"

DEFINITION_SINGLE(CEditorManager)

CEditorManager::CEditorManager() :
	m_EditMode(EditMode::Scene),
	m_DragObj(nullptr)
{
}

CEditorManager::~CEditorManager()
{
	CEngine::DestroyInst();
}

void CEditorManager::SetEditMode(EditMode Mode)
{
	m_EditMode = Mode;

	if (m_EditMode == EditMode::Sprite)
	{
		if (m_DragObj)
			m_DragObj->Destroy();
		m_DragObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDragObject>("DragObject");
		m_DragObj->SetWorldScale(0.f, 0.f, 1.f);
	}
}

bool CEditorManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"),
	                              1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}

	// IMGUI로 에디터에서 사용할 윈도우를 만들어준다.
	m_SpriteWindow = CIMGUIManager::GetInst()->AddWindow<CSpriteWindow>("SpriteWindow");
	m_EditorMenu        = CIMGUIManager::GetInst()->AddWindow<CEditorMenu>("EditorMenu");
	m_ObjectHierarchy = CIMGUIManager::GetInst()->AddWindow<CObjectHierarchy>("ObjectHierarchy");
	m_DetailInfoWindow = CIMGUIManager::GetInst()->AddWindow<CDetailInfoWindow>("DetailInfoWindow");

	CRenderManager::GetInst()->CreateLayer("DragLayer", INT_MAX);

	// Mouse
	CInput::GetInst()->CreateKey("MouseLButton", VK_LBUTTON);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Down, this, &CEditorManager::MouseLButtonDown);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Up, this, &CEditorManager::MouseLButtonUp);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Push, this, &CEditorManager::MouseLButtonPush);

	// KeyBoard
	CInput::GetInst()->CreateKey("Up", VK_UP);
	CInput::GetInst()->CreateKey("Down", VK_DOWN);
	CInput::GetInst()->CreateKey("Left", VK_LEFT);
	CInput::GetInst()->CreateKey("Right", VK_RIGHT);

	CInput::GetInst()->CreateKey("UpXSize", VK_RIGHT);
	CInput::GetInst()->SetCtrlKey("UpXSize", true);
	CInput::GetInst()->CreateKey("DownXSize", VK_LEFT);
	CInput::GetInst()->SetCtrlKey("DownXSize", true);
	CInput::GetInst()->CreateKey("UpYSize", VK_UP);
	CInput::GetInst()->SetCtrlKey("UpYSize", true);
	CInput::GetInst()->CreateKey("DownYSize", VK_DOWN);
	CInput::GetInst()->SetCtrlKey("DownYSize", true);

	CInput::GetInst()->SetKeyCallback("Up", KeyState_Down, this, &CEditorManager::KeyBoardUp);
	CInput::GetInst()->SetKeyCallback("Down", KeyState_Down, this, &CEditorManager::KeyBoardDown);
	CInput::GetInst()->SetKeyCallback("Left", KeyState_Down, this, &CEditorManager::KeyBoardLeft);
	CInput::GetInst()->SetKeyCallback("Right", KeyState_Down, this, &CEditorManager::KeyBoardRight);
	CInput::GetInst()->SetKeyCallback("UpXSize", KeyState_Down, this, &CEditorManager::IncreaseXSize);
	CInput::GetInst()->SetKeyCallback("DownXSize", KeyState_Down, this, &CEditorManager::DecreaseXSize);
	CInput::GetInst()->SetKeyCallback("UpYSize", KeyState_Down, this, &CEditorManager::IncreaseYSize);
	CInput::GetInst()->SetKeyCallback("DownYSize", KeyState_Down, this, &CEditorManager::DecreaseYSize);

	return true;
}

void CEditorManager::CreateDefaultSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CDefaultScene>();
}

int CEditorManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CEditorManager::MouseLButtonDown(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->SetStartPos(CInput::GetInst()->GetMousePos());
	}
}

void CEditorManager::MouseLButtonPush(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->SetEndPos(CInput::GetInst()->GetMousePos());
	}
}

void CEditorManager::MouseLButtonUp(float DeltaTime)
{
}

void CEditorManager::KeyBoardUp(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldPos(Vector3(0.f, 1.f, 0.f));
	}
}

void CEditorManager::KeyBoardLeft(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldPos(Vector3(-1.f, 0.f, 0.f));
	}
}

void CEditorManager::KeyBoardRight(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldPos(Vector3(1.f, 0.0f, 0.f));
	}
}

void CEditorManager::KeyBoardDown(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldPos(Vector3(0.f, -1.f, 0.f));
	}
}

void CEditorManager::IncreaseXSize(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldScale(Vector3(1.f, 0.f, 0.f));
	}
}

void CEditorManager::DecreaseXSize(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldScale(Vector3(-1.f, 0.f, 0.f));
	}
}

void CEditorManager::IncreaseYSize(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldScale(Vector3(0.f, 1.f, 0.f));
	}
}

void CEditorManager::DecreaseYSize(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->AddWorldScale(Vector3(0.f, -1.f, 0.f));
	}
}

void CEditorManager::CreateSceneMode(CScene* Scene, size_t SceneModeTypeID)
{
	if (SceneModeTypeID == typeid(CDefaultScene).hash_code())
	{
		Scene->LoadSceneMode<CDefaultScene>();
	}
}

CGameObject* CEditorManager::CreateGameObject(CScene* Scene, size_t GameObjectTypeID)
{
	if (GameObjectTypeID == typeid(CGameObject).hash_code())
	{
		CGameObject* Object = Scene->LoadGameObject<CGameObject>();
		return Object;
	}
	else if (GameObjectTypeID == typeid(CSpriteEditObject).hash_code())
	{
		CGameObject* Object  = Scene->LoadGameObject<CSpriteEditObject>();
		return Object;
	}
	else if (GameObjectTypeID == typeid(CDragObject).hash_code())
	{
		CGameObject* Object = Scene->LoadGameObject<CDragObject>();
		return Object;
	}
}

CComponent* CEditorManager::CreateComponent(CGameObject* Object, size_t ComponentTypeID)
{
	if (ComponentTypeID == typeid(CSceneComponent).hash_code())
	{
		CComponent* Component = Object->LoadComponent<CSceneComponent>();
		return Component;
	}
	else if (ComponentTypeID == typeid(CSpriteComponent).hash_code())
	{
		CComponent* Component =  Object->LoadComponent<CSpriteComponent>();
		return Component;
	}
	else if (ComponentTypeID == typeid(CStaticMeshComponent).hash_code())
	{
		CComponent* Component =  Object->LoadComponent<CStaticMeshComponent>();
		return Component;
	}
}

void CEditorManager::CreateAnimationInstance(CComponent* Component, size_t AnimationTypeID)
{
}
