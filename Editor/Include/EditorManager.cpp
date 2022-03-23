#include "EditorManager.h"
#include "Engine.h"
#include "Input.h"
#include "resource.h"
// IMGUI
#include "IMGUIManager.h"
#include "IMGUIListBox.h"
// Scene
#include "Animation/AnimationSequence2DInstance.h"
#include "Render/RenderManager.h"
#include "Scene/DefaultScene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneCollision.h"
#include "Scene/CameraManager.h"
// Window
#include "Window/SpriteEditWindow.h"
#include "Window/EditorMenu.h"
#include "Window/ObjectHierarchy.h"
#include "Window/DetailInfoWindow.h"
#include "Window/TileMapWindow.h"
#include "Window/BackGroundWindow.h"
#include "Window/LineEditWindow.h"
// Component 
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
// Object
#include "Object/Player2D.h"
#include "Object/ShowObject.h"
#include "Object/YellowBird.h"
#include "Object/PurpleBeatle.h"
#include "Object/NormalBear.h"
#include "Object/Squid.h"
#include "Object/TileMapEmpty.h"
#include "Object/MushRoom.h"
#include "Object/BackGround.h"
#include "Object/BeamMonster.h"
#include "Object/FireMonster.h"
#include "Object/FightMonster.h"
#include "Object/DragObject.h"
#include "Object/Block.h"
#include "Object/LineObject.h"
#include "Object/WaterFloat1.h"
#include "Object/WaterFloat2_2.h"
#include "Object/SpriteEditObject.h"
#include "Object/HPYellowItem.h"
#include "Object/HPRedItem.h"
#include "Object/HPGreenItem.h"
#include "Object/Line.h"
#include "Object/LineContainer.h"
#include "Object/EffectSceneChangeStar.h"
#include "Object/EffectChangeToGreen2.h"
#include "Object/EffectChangeToGreen1.h"
#include "Object/EffectChangeToGreen4.h"
#include "Object/EffectChangeToGreen5.h"
#include "Object/EffectChangeToFloat2_1.h"
#include "Object/EffectChangeToFloat2_2.h"
#include "Object/MiddleBossHammer.h"
#include "Object/BossTree.h"

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
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}

		m_DragObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDragObject>("DragObject");
		m_DragObj->SetWorldScale(0.f, 0.f, 1.f);

		// m_LineObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CLineObject>("DragObject");
		// m_LineObj->SetWorldScale(0.f, 0.f, 1.f);

		sprintf_s(EditModeText, "%s", "Sprite");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);

		// 타일을 그리지 않도록 세팅한다.
		m_TileMapWindow->SetTileMapComponentEditMode(false);
	}
	else if (m_EditMode == EditMode::Scene)
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}

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
		if (m_ShowObj)
		{
			m_ShowObj->Destroy();
			m_ShowObj = nullptr;
		}

		sprintf_s(EditModeText, "%s", "Tile");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);

		// 타일을 그리지 않도록 세팅한다.
		m_TileMapWindow->SetTileMapComponentEditMode(true);
	}
	else if (m_EditMode == EditMode::Back)
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}
		if (m_ShowObj)
		{
			m_ShowObj->Destroy();
			m_ShowObj = nullptr;
		}

		sprintf_s(EditModeText, "%s", "Back");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);

		// 타일을 그리지 않도록 세팅한다.
		m_TileMapWindow->SetTileMapComponentEditMode(false);
	}
	else if (m_EditMode == EditMode::CharacterCreate)
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}

		sprintf_s(EditModeText, "%s", "CharacterCreate");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);

		// 타일을 그리지 않도록 세팅한다.
		m_TileMapWindow->SetTileMapComponentEditMode(false);
	}
	else if (m_EditMode == EditMode::CharacterEdit)
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}

		sprintf_s(EditModeText, "%s", "CharacterEdit");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);

		// 타일을 그리지 않도록 세팅한다.
		m_TileMapWindow->SetTileMapComponentEditMode(true);
	}
	else if (m_EditMode == EditMode::LineCreate)
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}

		sprintf_s(EditModeText, "%s", "LineCreate");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);

		// 타일을 그리지 않도록 세팅한다.
		m_TileMapWindow->SetTileMapComponentEditMode(true);
	}
	else if (m_EditMode == EditMode::LineEdit)
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}

		sprintf_s(EditModeText, "%s", "LineEdit");

		_strupr_s(EditModeText);

		m_EditorMenu->SetEditModeText(EditModeText);

		// 타일을 그리지 않도록 세팅한다.
		m_TileMapWindow->SetTileMapComponentEditMode(true);
	}

}

void CEditorManager::SetSceneEditObject()
{
	if (!m_ShowObj)
	{
		m_ShowObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CShowObject>(OBJECTNAME_SHOWOBJECT);
		m_ShowObj->SetWorldScale(0.f, 0.f, 1.f);
	}
}

void CEditorManager::SetSceneEditObjectPos(CGameObject* Object)
{
	if (!CEditorManager::GetInst()->GetShowObject())
	{
		CEditorManager::GetInst()->SetSceneEditObject();
	}

	// 화면에 ShowObject 위치를 Object의 Root Component 것으로 세팅
	CShowObject* ShowObject = CEditorManager::GetInst()->GetShowObject();

	Vector3 ObjectPivot = Object->GetPivot();
	Vector3 ObjectSize = Object->GetWorldScale();
	Vector3 ObjectPos = Object->GetWorldPos();
	Vector3 Pos = ObjectPos - ObjectPivot * ObjectSize;

	Vector2 StartPos = Vector2(Pos.x, Pos.y);
	Vector2 EndPos = Vector2(Pos.x + ObjectSize.x, Pos.y + ObjectSize.y);

	ShowObject->SetStartPos(StartPos);
	ShowObject->SetEndPos(EndPos);

	// Camera 도 해당 위치에 세팅한다.
	// CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->SetWorldPos(ObjectPos);
}



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
	m_LineEditWindow = CIMGUIManager::GetInst()->AddWindow<CLineEditWindow>("LineEditWindow");

	// SceneMode 에 중력 적용 X 를 세팅한다.
	CSceneManager::GetInst()->GetScene()->SetIsEditMode(true);

	// Sprite 편집할 때
	CRenderManager::GetInst()->CreateLayer("DragLayer", INT_MAX);

	// Scene 편집할 때
	CRenderManager::GetInst()->CreateLayer("SceneEditLayer", INT_MAX);

	// Mouse
	CInput::GetInst()->CreateKey("MouseLButton", VK_LBUTTON);
	CInput::GetInst()->SetKeyCallback("MouseLButton", Key_State::KeyState_Down, this, &CEditorManager::MouseLButtonDown);
	CInput::GetInst()->SetKeyCallback("MouseLButton", Key_State::KeyState_Up, this, &CEditorManager::MouseLButtonUp);
	CInput::GetInst()->SetKeyCallback("MouseLButton", Key_State::KeyState_Push, this, &CEditorManager::MouseLButtonPush);

	CInput::GetInst()->CreateKey("MouseRButton", VK_RBUTTON);
	CInput::GetInst()->SetKeyCallback("MouseRButton", Key_State::KeyState_Down, this, &CEditorManager::MouseRButtonDown);
	CInput::GetInst()->SetKeyCallback("MouseRButton", Key_State::KeyState_Push, this, &CEditorManager::MouseRButtonPush);
	CInput::GetInst()->SetKeyCallback("MouseRButton", Key_State::KeyState_Up, this, &CEditorManager::MouseRButtonUp);

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
	// CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->OnViewportCenter();
	// CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->SetViewportRatio(0.2f, 0.2f);

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
	Vector2 CameraLB = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetLeftBottom();

	if (m_DragObj)
	{

		if (!m_MousePush)
		{
			m_MousePush = true;

			m_DragObj->SetStartPos(CInput::GetInst()->GetMousePos() + CameraLB);
		}
		else
		{
			m_MousePush = false;

			m_DragObj->SetEndPos(CInput::GetInst()->GetMousePos() + CameraLB);
		}
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

void CEditorManager::MouseRButtonDown(float DeltaTime)
{
	// Edit Mode가 Object Create 일 때는 해당 위치에 Object Create
	if (m_EditMode == EditMode::CharacterCreate)
	{
		if (m_ObjectHierarchy->IsSpecificObjectSelected() == false)
			return;

		// Object Hierarchy 의 m_SpecificObjectList 에서 선택된 Object 의 이름을 가져온다.
		std::string SelectObjectName = m_ObjectHierarchy->GetSpecificObjectListBox()->GetSelectItem();

		// 생성될 Monster 이름을 Random 하게 만든다.
		std::string NewMonsterName = SelectObjectName + "_" + GetRandomString();

		// 혹시나 이미 만들어진 이름이라면 X
		if (m_ObjectHierarchy->CheckDuplicateObject(NewMonsterName))
			return;

		CGameObject* CreatedObject = nullptr;

		if (strcmp(g_NormalBearName.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CNormalBear>(NewMonsterName);
		}
		else if (strcmp(g_YellowBirdName.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CYellowBird>(NewMonsterName);
		}
		else if (strcmp(g_PurpleBeatlesName.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CPurpleBeatle>(NewMonsterName);
		}
		else if (strcmp(g_MushRoomName.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CMushRoom>(NewMonsterName);
		}
		else if (strcmp(g_Squid.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSquid>(NewMonsterName);
		}
		else if (strcmp(g_BeamMonsterName.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CBeamMonster>(NewMonsterName);
		}
		else if (strcmp(g_FireMonsterName.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CFireMonster>(NewMonsterName);
		}
		else if (strcmp(g_FightMonsterName.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CFightMonster>(NewMonsterName);
		}
		else if (strcmp(g_MiddleHammerName.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CMiddleBossHammer>(NewMonsterName);
		}
		else if (strcmp(g_BossTree.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CBossTree>(NewMonsterName);
		}
		else if (strcmp(g_HPYellowItem.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CHPYellowItem>(NewMonsterName);
		}
		else if (strcmp(g_HPGreenItem.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CHPGreenItem>(NewMonsterName);
		}
		else if (strcmp(g_HPRedItem.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CHPRedItem>(NewMonsterName);
		}
		else if (strcmp(g_Block.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CBlock>(NewMonsterName);
		}
		else if (strcmp(g_SceneChangeStart.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CEffectSceneChangeStar>(NewMonsterName);
		}
		else if (strcmp(g_SceneChangeToGreen2.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CEffectChangeToGreen2>(NewMonsterName);
		}
		else if (strcmp(g_SceneChangeToGreen1.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CEffectChangeToGreen1>(NewMonsterName);
		}
		else if (strcmp(g_SceneChangeToGreen4.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CEffectChangeToGreen4>(NewMonsterName);
		}
		else if (strcmp(g_SceneChangeToGreen5.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CEffectChangeToGreen5>(NewMonsterName);
		}
		else if (strcmp(g_SceneChangeToFloat2_1.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CEffectChangeToFloat2_1>(NewMonsterName);
		}
		else if (strcmp(g_SceneChangeToFloat2_2.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CEffectChangeToFloat2_2>(NewMonsterName);
		}
		else if (strcmp(g_WaterFloatIsland1.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CWaterFloat1>(NewMonsterName);
		}
		else if (strcmp(g_WaterFloatIsland2_2.c_str(), SelectObjectName.c_str()) == 0)
		{
			CreatedObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CWaterFloat2_2>(NewMonsterName);
		}

		if (!CreatedObject)
			return;

		// 현재 마우스 위치에 만들기
		Vector2 Mouse2DPos = CInput::GetInst()->GetMouseWorld2DPos();

		CreatedObject->SetWorldPos(Mouse2DPos.x, Mouse2DPos.y, 1.f);

		// Object Hierarchy 목록에 추가하기
		m_ObjectHierarchy->AddCreatedObject(NewMonsterName.c_str());
	}
	else if (m_EditMode == EditMode::CharacterEdit)
	{
		// 1) 그냥 클릭 시 해당 GameObject 의 정보를 Detail Info 에 세팅해둔다.
		CColliderComponent* ClickedComponent = CSceneManager::GetInst()->GetScene()->GetCollision()->GetMouseCollision();

		if (!ClickedComponent)
			return;

		CGameObject* ClickedObject = ClickedComponent->GetGameObject();

		if (!ClickedObject)
			return;

		// 1) Detail 정보 세팅
		CSceneComponent* SelectRootComponent = ClickedComponent->GetGameObject()->GetRootComponent();

		m_DetailInfoWindow->SetClickedObjectInfo(SelectRootComponent);

		// 2) Show Object 도 해당 위치를 가리키게 세팅해둔다.
		SetSceneEditObjectPos(ClickedObject);

		// 3) ObjectHierarchy 에서도 해당 Name 을 Select 상태로 둔다.
		m_ObjectHierarchy->SetSeletedObjectByName(ClickedObject->GetName());
	}
	else if (m_EditMode == EditMode::LineEdit)
	{
		Vector2 CameraLB = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetLeftBottom();

		CLine* SelectLine = (CLine*)m_LineEditWindow->GetSelectLine();

		if (!SelectLine)
			return;

		// SelectLine->SetEndPos(CInput::GetInst()->GetMouseWorld2DPos() + CameraLB);
		SelectLine->SetStartPos(CInput::GetInst()->GetMousePos() + CameraLB);
	}
}

void CEditorManager::MouseRButtonPush(float DeltaTime)
{
	if (m_EditMode == EditMode::CharacterEdit)
	{
		// Drag 기능을 구현할 것이다.
		CColliderComponent* ClickedComponent = CSceneManager::GetInst()->GetScene()->GetCollision()->GetMouseCollision();

		if (!ClickedComponent)
			return;

		CGameObject* ClickedObject = ClickedComponent->GetGameObject();

		if (!ClickedObject)
			return;

		Vector2 MouseMove  = CInput::GetInst()->GetMouseMove();

		// Click한 녀석 WorldPos 이동
		ClickedObject->AddWorldPos(Vector3(MouseMove.x, MouseMove.y, 0.f));

		SetSceneEditObjectPos(ClickedObject);

		m_DetailInfoWindow->SetTransformInfo(ClickedObject->GetRootComponent());
	}
	else if (m_EditMode == EditMode::LineEdit)
	{
		Vector2 CameraLB = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetLeftBottom();

		CLine* SelectLine = (CLine*)m_LineEditWindow->GetSelectLine();

		if (!SelectLine)
			return;

		// SelectLine->SetEndPos(CInput::GetInst()->GetMouseWorld2DPos() + CameraLB);
		SelectLine->SetEndPos(CInput::GetInst()->GetMousePos() + CameraLB);
	}
}

void CEditorManager::MouseRButtonUp(float DeltaTime)
{
	// 최종 Line 모양 및 WorldPos, 기울기를 결정할 것이다.
	if (m_EditMode == EditMode::LineEdit)
	{
		Vector2 CameraLB = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetLeftBottom();

		CLine* SelectLine = (CLine*)m_LineEditWindow->GetSelectLine();

		if (!SelectLine)
			return;

		Vector3 StartPos = SelectLine->GetStartPos();
		Vector3 WorldScale = SelectLine->GetWorldScale();
		Vector3 EndPos = SelectLine->GetEndPos();

		Line_DrawType DrawType = SelectLine->GetDrawType();

		float XScale = -1.f, YScale = -1.f;

		switch(DrawType)
		{
		case Line_DrawType::RightBottom :
			{
				SelectLine->SetWorldPos(StartPos.x, EndPos.y, 1.f);

				XScale = EndPos.x - StartPos.x;
				YScale = StartPos.y - EndPos.y;
			}
			break;
		case Line_DrawType::RightUp:
			{
				SelectLine->SetWorldPos(StartPos.x, EndPos.y, 1.f);

				XScale = EndPos.x - StartPos.x;
				YScale = StartPos.y - EndPos.y;
			}
			break;
		case Line_DrawType::LeftBottom:
			{
				SelectLine->SetWorldPos(EndPos.x, StartPos.y, 1.f);

				XScale = StartPos.x - EndPos.x;
				YScale = EndPos.y - StartPos.y;
			}
			break;
		case Line_DrawType::LeftUp:
			{
				SelectLine->SetWorldPos(StartPos.x, EndPos.y, 1.f);

				XScale = EndPos.x - StartPos.x;
				YScale = StartPos.y - EndPos.y;
			}
			break;
		}

		SelectLine->SetWorldScale(XScale, YScale, 1.f);
		SelectLine->ResetDrawBoxPos();

		Vector3 FinalStartPos = SelectLine->GetWorldPos();
		Vector3 FinalWorldScale = SelectLine->GetWorldScale();
		Vector3 FinalEndPos = FinalStartPos + FinalWorldScale;

		Vector3 FinalLeftPos = {};
		Vector3 FinalRightPos = {};

		float Slope = -1.f;

		switch (DrawType)
		{
		case Line_DrawType::RightBottom:
		{
			FinalLeftPos = Vector3(FinalStartPos.x, FinalEndPos.y,1.f);
			FinalRightPos = Vector3(FinalEndPos.x, FinalStartPos.y, 1.f);
			Slope = (FinalRightPos.y - FinalLeftPos.y) / (FinalRightPos.x - FinalLeftPos.x);
			SelectLine->SetFinalPosInfo(FinalLeftPos, FinalRightPos, Slope);
		}
		break;
		case Line_DrawType::RightUp:
		{
			FinalLeftPos = Vector3(FinalStartPos.x, FinalEndPos.y, 1.f);
			FinalRightPos = Vector3(FinalEndPos.x, FinalStartPos.y, 1.f);
			Slope = (FinalRightPos.y - FinalLeftPos.y) / (FinalRightPos.x - FinalLeftPos.x);
			SelectLine->SetFinalPosInfo(FinalLeftPos, FinalRightPos, Slope);
		}
		break;
		case Line_DrawType::LeftBottom:
		{
			FinalLeftPos = Vector3(FinalStartPos.x, FinalEndPos.y, 1.f);
			FinalRightPos = Vector3(FinalEndPos.x, FinalStartPos.y, 1.f);
			Slope = (FinalRightPos.y - FinalLeftPos.y) / (FinalRightPos.x - FinalLeftPos.x);
			SelectLine->SetFinalPosInfo(FinalLeftPos, FinalRightPos, Slope);
		}
		break;
		case Line_DrawType::LeftUp:
		{
			FinalLeftPos = Vector3(FinalEndPos.x, FinalStartPos.y, 1.f);
			FinalRightPos = Vector3(FinalStartPos.x, FinalEndPos.y, 1.f);
			Slope = (FinalRightPos.y - FinalLeftPos.y) / (FinalRightPos.x - FinalLeftPos.x);
			SelectLine->SetFinalPosInfo(FinalLeftPos, FinalRightPos, Slope);
		}
		break;
		}

		// 이제 Line Edit Window의 Description에 세팅한다.
		m_LineEditWindow->SetLineDescription(FinalLeftPos, FinalRightPos, Slope);
	}
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

		Vector3 CameraOriginPos = Camera->GetWorldPos();

		m_SpriteWindow->SetCameraPosText(CameraOriginPos.x, CameraOriginPos.y);
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

		Vector3 CameraOriginPos = Camera->GetWorldPos();

		m_SpriteWindow->SetCameraPosText(CameraOriginPos.x, CameraOriginPos.y);
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
	else if (GameObjectTypeID == typeid(CYellowBird).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CYellowBird>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CPurpleBeatle).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CPurpleBeatle>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CNormalBear).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CNormalBear>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CMushRoom).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CMushRoom>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CSquid).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CSquid>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CHPYellowItem).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CHPYellowItem>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CHPGreenItem).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CHPGreenItem>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CHPRedItem).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CHPRedItem>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CBlock).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CBlock>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CBeamMonster).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CBeamMonster>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CFireMonster).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CFireMonster>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CMiddleBossHammer).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CMiddleBossHammer>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CBossTree).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CBossTree>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CFightMonster).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CFightMonster>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CTileMapEmpty).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CTileMapEmpty>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CBackGround).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CBackGround>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CLine).hash_code())
	{
		CLine* Obj = Scene->LoadGameObject<CLine>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CLineContainer).hash_code())
	{
		CLineContainer* Obj = Scene->LoadGameObject<CLineContainer>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CEffectSceneChangeStar).hash_code())
	{
		CEffectSceneChangeStar* Obj = Scene->LoadGameObject<CEffectSceneChangeStar>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CEffectChangeToGreen2).hash_code())
	{
		CEffectChangeToGreen2* Obj = Scene->LoadGameObject<CEffectChangeToGreen2>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CEffectChangeToGreen1).hash_code())
	{
		CEffectChangeToGreen1* Obj = Scene->LoadGameObject<CEffectChangeToGreen1>();
		return Obj;
	}
	else if (GameObjectTypeID == typeid(CEffectChangeToGreen4).hash_code())
	{
	CEffectChangeToGreen4* Obj = Scene->LoadGameObject<CEffectChangeToGreen4>();
	return Obj;
	}
	else if (GameObjectTypeID == typeid(CEffectChangeToGreen5).hash_code())
	{
	CEffectChangeToGreen5* Obj = Scene->LoadGameObject<CEffectChangeToGreen5>();
	return Obj;
	}
	else if (GameObjectTypeID == typeid(CEffectChangeToFloat2_1).hash_code())
	{
	CEffectChangeToFloat2_1* Obj = Scene->LoadGameObject<CEffectChangeToFloat2_1>();
	return Obj;
	}
	else if (GameObjectTypeID == typeid(CEffectChangeToFloat2_2).hash_code())
	{
	CEffectChangeToFloat2_2* Obj = Scene->LoadGameObject<CEffectChangeToFloat2_2>();
	return Obj;
	}
	else if (GameObjectTypeID == typeid(CWaterFloat1).hash_code())
	{
	CWaterFloat1* Obj = Scene->LoadGameObject<CWaterFloat1>();
	return Obj;
	}
	else if (GameObjectTypeID == typeid(CWaterFloat2_2).hash_code())
	{
	CWaterFloat2_2* Obj = Scene->LoadGameObject<CWaterFloat2_2>();
	return Obj;
	}
	
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
		CBackGroundComponent* Component = Obj->LoadComponent<CBackGroundComponent>();

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
