#include "EditorManager.h"
#include "Scene/CameraManager.h"
#include "Engine.h"
#include "IMGUIManager.h"
#include "Input.h"
#include "resource.h"
#include "Object/DragObject.h"
#include "Object/SpriteEditObject.h"
#include "Render/RenderManager.h"
#include "Scene/DefaultScene.h"
#include "Scene/SceneManager.h"
#include "Window/SpriteEditWindow.h"
#include "Window/EditorMenu.h"
#include "Window/ObjectHierarchy.h"
#include "Window/DetailInfoWindow.h"
#include "Window/TileMapWindow.h"
#include "Window/BackGroundWindow.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/PaperBurnComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderPixel.h"
#include "Component/TileMapComponent.h"
#include "Component/BackGroundComponent.h"
#include "Component/TileEmptyComponent.h"
#include "Component/CameraComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Object/Player2D.h"
#include "Object/ShowObject.h"
#include "../../Client2D/Include/Object/YellowBird.h"
#include "../../Client2D/Include/Object/BeamMonster.h"

DEFINITION_SINGLE(CEditorManager)

CEditorManager::CEditorManager() :
	m_EditMode(EditMode::Scene),
	m_DragObj(nullptr),
	m_CameraMoveSpeed(1000.f)
{
}

CEditorManager::~CEditorManager()
{
	CEngine::DestroyInst();
}

void CEditorManager::SetEditMode(EditMode Mode)
{
	m_EditMode = Mode;

	// + Combo Box 상태에 대해 Callback 함수도 지정해줘야 한다.

	char EditModeText[MAX_PATH] = {};

	if (m_EditMode == EditMode::Sprite)
	{
		if (m_DragObj)
			m_DragObj->Destroy();

		m_DragObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDragObject>("DragObject");
		m_DragObj->SetWorldScale(0.f, 0.f, 1.f);

		sprintf_s(EditModeText, "%s", "Sprite");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);
	}
	else if (m_EditMode == EditMode::Scene)
	{
		if (m_DragObj)
			m_DragObj->Destroy();

		sprintf_s(EditModeText, "%s", "Scene");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);
	}
	else if (m_EditMode == EditMode::Tile)
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}

		sprintf_s(EditModeText, "%s", "Tile");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);
	}
	else if (m_EditMode == EditMode::Back)
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}

		sprintf_s(EditModeText, "%s", "Back");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);
	}
	else if (m_EditMode == EditMode::Line)
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}

		sprintf_s(EditModeText, "%s", "Sprite");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);
	}
}

void CEditorManager::SetSceneEditObject()
{
	if (!m_ShowObj)
	{
		m_ShowObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CShowObject>("SceneEditObject");
		m_ShowObj->SetWorldScale(0.f, 0.f, 1.f);

	}
}

/*
void CEditorManager::CreateCameraObject()
{
	if (!m_CameraObject)
	{
		m_CameraObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteCameraObject>("CameraObject");
		m_CameraObject->SetWorldScale(0.f, 0.f, 1.f);
	}
}
*/

bool CEditorManager::Init(HINSTANCE hInst)//
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"),
	                              1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}

	CSceneManager::GetInst()->SetCreateSceneModeCallback(this, &CEditorManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectCallback(this, &CEditorManager::CreateGameObject);
	CSceneManager::GetInst()->SetCreateComponentCallback(this, &CEditorManager::CreateComponent);
	CSceneManager::GetInst()->SetCreateAnimInstanceCallback(this, &CEditorManager::CreateAnimationInstance);

	// IMGUI로 에디터에서 사용할 윈도우를 만들어준다.
	m_SpriteWindow = CIMGUIManager::GetInst()->AddWindow<CSpriteEditWindow>("SpriteEditWindow");
	m_EditorMenu        = CIMGUIManager::GetInst()->AddWindow<CEditorMenu>("EditorMenu");
	m_ObjectHierarchy = CIMGUIManager::GetInst()->AddWindow<CObjectHierarchy>("ObjectHierarchy");
	m_DetailInfoWindow = CIMGUIManager::GetInst()->AddWindow<CDetailInfoWindow>("DetailInfoWindow");
	m_TileMapWindow = CIMGUIManager::GetInst()->AddWindow<CTileMapWindow>("TileMapWindow");
	m_BackGroundWindow = CIMGUIManager::GetInst()->AddWindow<CBackGroundWindow>("BackGroundWindow");

	// Sprite 편집할 때
	CRenderManager::GetInst()->CreateLayer("DragLayer", INT_MAX);

	// Scene 편집할 때
	CRenderManager::GetInst()->CreateLayer("SceneEditLayer", INT_MAX);

	// Mouse
	CInput::GetInst()->CreateKey("MouseLButton", VK_LBUTTON);
	CInput::GetInst()->SetKeyCallback("MouseLButton", Key_State::KeyState_Down, this, &CEditorManager::MouseLButtonDown);
	CInput::GetInst()->SetKeyCallback("MouseLButton", Key_State::KeyState_Up, this, &CEditorManager::MouseLButtonUp);
	CInput::GetInst()->SetKeyCallback("MouseLButton", Key_State::KeyState_Push, this, &CEditorManager::MouseLButtonPush);

	// KeyBoard
	// CInput::GetInst()->CreateKey("Up", VK_UP);
	// CInput::GetInst()->CreateKey("Down", VK_DOWN);
	// CInput::GetInst()->CreateKey("Left", VK_LEFT);
	// CInput::GetInst()->CreateKey("Right", VK_RIGHT);
	CInput::GetInst()->CreateKey("Up", 'W');
	CInput::GetInst()->CreateKey("Down", 'S');
	CInput::GetInst()->CreateKey("Left", 'A');
	CInput::GetInst()->CreateKey("Right", 'D');

	CInput::GetInst()->CreateKey("UpXSize", VK_RIGHT);
	CInput::GetInst()->SetCtrlKey("UpXSize", true);
	CInput::GetInst()->CreateKey("DownXSize", VK_LEFT);
	CInput::GetInst()->SetCtrlKey("DownXSize", true);
	CInput::GetInst()->CreateKey("UpYSize", VK_UP);
	CInput::GetInst()->SetCtrlKey("UpYSize", true);
	CInput::GetInst()->CreateKey("DownYSize", VK_DOWN);
	CInput::GetInst()->SetCtrlKey("DownYSize", true);

	CInput::GetInst()->SetKeyCallback("Up", Key_State::KeyState_Push, this, &CEditorManager::KeyBoardUp);
	CInput::GetInst()->SetKeyCallback("Down", Key_State::KeyState_Push, this, &CEditorManager::KeyBoardDown);
	CInput::GetInst()->SetKeyCallback("Left", Key_State::KeyState_Push, this, &CEditorManager::KeyBoardLeft);
	CInput::GetInst()->SetKeyCallback("Right", Key_State::KeyState_Push, this, &CEditorManager::KeyBoardRight);

	CInput::GetInst()->SetKeyCallback("UpXSize", Key_State::KeyState_Down, this, &CEditorManager::IncreaseXSize);
	CInput::GetInst()->SetKeyCallback("DownXSize", Key_State::KeyState_Down, this, &CEditorManager::DecreaseXSize);
	CInput::GetInst()->SetKeyCallback("UpYSize", Key_State::KeyState_Down, this, &CEditorManager::IncreaseYSize);
	CInput::GetInst()->SetKeyCallback("DownYSize", Key_State::KeyState_Down, this, &CEditorManager::DecreaseYSize);

	// Camera 를 화면 가운데 오게 세팅한다.
	CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->OnViewportCenter();

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
		Vector2 CameraLB = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetLeftBottom();
		m_DragObj->SetStartPos(CInput::GetInst()->GetMousePos() + CameraLB);
	}
}

void CEditorManager::MouseLButtonPush(float DeltaTime)
{
	m_MousePush = true;

	if (m_DragObj)
	{
		Vector2 CameraLB = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetLeftBottom();
		m_DragObj->SetEndPos(CInput::GetInst()->GetMousePos() + CameraLB);
	}
}

void CEditorManager::MouseLButtonUp(float DeltaTime)
{
	m_MousePush = false;
}

void CEditorManager::KeyBoardUp(float DeltaTime)
{
	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Vector2 WorldRS = CSceneManager::GetInst()->GetScene()->GetWorldResolution();

	Resolution RS = CEngine::GetInst()->GetResolution();

	if (Camera)
	{
		Camera->AddWorldPos(Vector3(0.f, m_CameraMoveSpeed * DeltaTime, 0.f));

		Vector3 CameraOriginPos = Camera->GetWorldPos();


		if (CameraOriginPos.y + RS.Height * 0.5f >= WorldRS.y)
		{
			CameraOriginPos.y = WorldRS.y - (RS.Height * 0.5f);

			Camera->SetWorldPos(CameraOriginPos);
		}

		m_SpriteWindow->SetCameraPosText(CameraOriginPos.x, CameraOriginPos.y);
	}
}

void CEditorManager::KeyBoardLeft(float DeltaTime)
{
	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Vector2 WorldRS = CSceneManager::GetInst()->GetScene()->GetWorldResolution();

	Resolution RS = CEngine::GetInst()->GetResolution();

	if (Camera)
	{
		Camera->AddWorldPos(Vector3(-1.f * m_CameraMoveSpeed * DeltaTime, 0.f, 0.f));

		Vector3 CameraOriginPos = Camera->GetWorldPos();

		// 

		/*
		if (CameraOriginPos.x - RS.Width * 0.5f < 0.f)
		{
			CameraOriginPos.x = RS.Width * 0.5f;

			Camera->SetWorldPos(CameraOriginPos);
		}
		*/

		m_SpriteWindow->SetCameraPosText(CameraOriginPos.x, CameraOriginPos.y);
	}
}

void CEditorManager::KeyBoardRight(float DeltaTime)
{
	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Vector2 WorldRS = CSceneManager::GetInst()->GetScene()->GetWorldResolution();

	Resolution RS = CEngine::GetInst()->GetResolution();

	if (Camera)
	{
		Camera->AddWorldPos(Vector3(m_CameraMoveSpeed * DeltaTime, 0.f, 0.f));

		/*
		Vector3 CameraPos = Camera->GetWorldPos();

		if (CameraPos.x + RS.Width * 0.5f > WorldRS.x)
		{
			CameraPos.x = WorldRS.x -  RS.Width * 0.5f;

			Camera->SetWorldPos(Vector3(CameraPos.x, CameraPos.y, CameraPos.z));
		}
		*/

		m_SpriteWindow->SetCameraPosText(Camera->GetWorldPos().x, Camera->GetWorldPos().y);
	}
}

void CEditorManager::KeyBoardDown(float DeltaTime)
{
	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Vector2 WorldRS = CSceneManager::GetInst()->GetScene()->GetWorldResolution();

	Resolution RS = CEngine::GetInst()->GetResolution();

	if (Camera)
	{
		Camera->AddWorldPos(Vector3(0.f, -1.f * m_CameraMoveSpeed * DeltaTime, 0.f));

		/*
		Vector3 CameraPos = Camera->GetWorldPos();

		if (CameraPos.y - RS.Height * 0.5f  < 0.f)
		{
			CameraPos.y = RS.Height * 0.5f;

			Camera->SetWorldPos(Vector3(CameraPos.x, CameraPos.y, CameraPos.z));
		}
		*/

		m_SpriteWindow->SetCameraPosText(Camera->GetWorldPos().x, Camera->GetWorldPos().y);
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

void CEditorManager::CreateSceneMode(CScene* Scene, const size_t SceneModeTypeID)
{
	if (SceneModeTypeID == typeid(CDefaultScene).hash_code())
	{
		Scene->LoadSceneMode<CDefaultScene>();
	}
}

CGameObject* CEditorManager::CreateGameObject(CScene* Scene, const size_t GameObjectTypeID)
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
	else if (GameObjectTypeID == typeid(CPlayer2D).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CPlayer2D>();
		return Obj;
	}
	/*
	else if (GameObjectTypeID == typeid(CYellowBird).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CYellowBird>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CBeamMonster).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CBeamMonster>();
		return Obj;
	}
	*/
	return nullptr;
}

CComponent* CEditorManager::CreateComponent(CGameObject* Obj, const size_t Type)
{
	if (Type == typeid(CSceneComponent).hash_code())
	{
		CSceneComponent* Component = Obj->LoadComponent<CSceneComponent>();

		return Component;
	}
	if (Type == typeid(CStaticMeshComponent).hash_code())
	{
		CStaticMeshComponent* Component = Obj->LoadComponent<CStaticMeshComponent>();

		return Component;
	}
	if (Type == typeid(CSpriteComponent).hash_code())
	{
		CSpriteComponent* Component = Obj->LoadComponent<CSpriteComponent>();

		return Component;
	}
	if (Type == typeid(CColliderBox2D).hash_code())
	{
		CColliderBox2D* Component = Obj->LoadComponent<CColliderBox2D>();

		return Component;
	}
	if (Type == typeid(CColliderCircle).hash_code())
	{
		CColliderCircle* Component = Obj->LoadComponent<CColliderCircle>();

		return Component;
	}
	if (Type == typeid(CColliderPixel).hash_code())
	{
		CColliderPixel* Component = Obj->LoadComponent<CColliderPixel>();

		return Component;
	}
	if (Type == typeid(CWidgetComponent).hash_code())
	{
		CWidgetComponent* Component = Obj->LoadComponent<CWidgetComponent>();

		return Component;
	}
	if (Type == typeid(CCameraComponent).hash_code())
	{
		CCameraComponent* Component = Obj->LoadComponent<CCameraComponent>();

		return Component;
	}
	if (Type == typeid(CNavAgent).hash_code())
	{
		CNavAgent* Component = Obj->LoadComponent<CNavAgent>();

		return Component;
	}
	if (Type == typeid(CTileMapComponent).hash_code())
	{
		CTileMapComponent* Component = Obj->LoadComponent<CTileMapComponent>();

		Component->EnableEditMode(true);

		return Component;
	}
	if (Type == typeid(CTileEmptyComponent).hash_code())
	{
		CTileEmptyComponent* Component = Obj->LoadComponent<CTileEmptyComponent>();

		Component->EnableEditMode(true);

		return Component;
	}
	if (Type == typeid(CParticleComponent).hash_code())
	{
		CParticleComponent* Component = Obj->LoadComponent<CParticleComponent>();

		return Component;
	}
	if (Type == typeid(CPaperBurnComponent).hash_code())
	{
		CPaperBurnComponent* Component = Obj->LoadComponent<CPaperBurnComponent>();

		return Component;
	}
	if (Type == typeid(CNavAgent).hash_code())
	{
		CNavAgent* Component = Obj->LoadComponent<CNavAgent>();

		return Component;
	}
	if (Type == typeid(CBackGroundComponent).hash_code())
	{
		CNavAgent* Component = Obj->LoadComponent<CNavAgent>();

		return Component;
	}
	return nullptr;
}

void CEditorManager::CreateAnimationInstance(CSpriteComponent* Component, const size_t AnimationTypeID)
{
	/*
	if (AnimationTypeID == typeid(CAnimationSequence2DInstance).hash_code())
	{
		Component->LoadAnimationInstance<CAnimationSequence2DInstance>();
	}
	*/

	Component->LoadAnimationInstance<CAnimationSequence2DInstance>();
}
