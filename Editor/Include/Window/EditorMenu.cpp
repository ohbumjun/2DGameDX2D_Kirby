#include "EditorMenu.h"
#include "IMGUITextInput.h"
#include "IMGUIText.h"
#include "IMGUIComboBox.h"
#include "IMGUISameLine.h"
#include "IMGUIButton.h"
#include "IMGUILabel.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "../EditorManager.h"
#include "ObjectHierarchy.h"
#include "IMGUIListBox.h"
#include "../Object/SpriteEditObject.h"
// Object
#include "../Object/Player2D.h"
#include "../Object/ShowObject.h"
#include "../Object/TileMapEmpty.h"
#include "../Object/BackGround.h"
#include "../Object/LineContainer.h"
#include "../Object/Line.h"
/*
#include "../Object/YellowBird.h"
#include "../Object/PurpleBeatle.h"
#include "../Object/NormalBear.h"
#include "../Object/MushRoom.h"
#include "../Object/HPYellowItem.h"
#include "../Object/HPGreenItem.h"
#include "../Object/HPRedItem.h"
#include "../Object/Block.h"
#include "../Object/BeamMonster.h"
#include "../Object/FireMonster.h"
#include "../Object/FightMonster.h"
#include "../Object/EffectChangeToGreen2.h"
#include "../Object/EffectChangeToGreen1.h"
#include "../Object/EffectChangeToGreen5.h"
#include "../Object/EffectChangeToGreen4.h"
#include "../Object/EffectSceneChangeStar.h"
*/
// Component
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/CameraComponent.h"
#include "Component/TileMapComponent.h"
#include "Component/TileEmptyComponent.h"
#include "Component/BackGroundComponent.h"
#include "PathManager.h"
#include "Engine.h"
// Window 
#include "TileMapWindow.h"
#include "BackGroundWindow.h"
#include "LineEditWindow.h"


CEditorMenu::CEditorMenu()
{
}

CEditorMenu::~CEditorMenu()
{
}

bool CEditorMenu::Init()
{
	CIMGUIWindow::Init();
	
	// ========================================================================

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("Edit Mode", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(115.f);

	m_EditMenuComboBox = AddWidget<CIMGUIComboBox>("Edit Mode", 150.f, 30.f);
	m_EditMenuComboBox->AddItem("Sprite");
	m_EditMenuComboBox->AddItem("Scene");
	m_EditMenuComboBox->AddItem("TileMap");
	m_EditMenuComboBox->AddItem("BackGround");
	m_EditMenuComboBox->AddItem("CharacterCreate");
	m_EditMenuComboBox->AddItem("CharacterEdit");
	m_EditMenuComboBox->AddItem("LineCreate");
	m_EditMenuComboBox->AddItem("LineEdit");
	m_EditMenuComboBox->SetHideName(true);
	m_EditMenuComboBox->SetSelectCallback<CEditorMenu>(this, &CEditorMenu::SelectEditModeCallback);

	// ========================================================================

	Label = AddWidget<CIMGUILabel>("Current Mode", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(115.f);

	m_CurrentEditMenu = AddWidget<CIMGUIText>("Edit Mode Text");
	m_CurrentEditMenu->SetColor(255, 255, 255);
	m_CurrentEditMenu->SetText("- - - ");
	m_CurrentEditMenu->SetSize(150.f, 80.f);

	Label = AddWidget<CIMGUILabel>("", 100.f, 30.f);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.5f, 0.0f);

	// ========================================================================

	Label = AddWidget<CIMGUILabel>("Scene", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(115.f);

	m_SaveSceneButton = AddWidget<CIMGUIButton>("SaveScene", 100.f, 30.f);
	m_SaveSceneButton->SetClickCallback(this, &CEditorMenu::SaveScene);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(220.f);

	m_LoadSceneButton = AddWidget<CIMGUIButton>("LoadScene", 100.f, 30.f);
	m_LoadSceneButton->SetClickCallback(this, &CEditorMenu::LoadScene);

	Label = AddWidget<CIMGUILabel>("", 100.f, 30.f);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.5f, 0.0f);

	// ================================================

	Label = AddWidget<CIMGUILabel>("Object", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(115.f);

	m_ObjectButton = AddWidget<CIMGUIButton>("CreateObj", 100.f, 30.f);
	m_ObjectButton->SetClickCallback(this, &CEditorMenu::CreateNewObject);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(220.f);

	m_ObjectComboBox = AddWidget<CIMGUIComboBox>("SelectObj", 100.f, 30.f);
	m_ObjectComboBox->AddItem("GameObject");
	m_ObjectComboBox->AddItem("Player");
	// m_ObjectComboBox->AddItem("YellowBird");
	// m_ObjectComboBox->AddItem("PurpleBeatles");
	// m_ObjectComboBox->AddItem("NormalBear");
	m_ObjectComboBox->AddItem("TileEmpty");
	m_ObjectComboBox->AddItem("BackGround");
	m_ObjectComboBox->AddItem("LineContainer");
	m_ObjectComboBox->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(325.f);

	m_ObjectNameInput = AddWidget<CIMGUITextInput>("Name", 100.f, 30.f);

	// ================================================
	Label = AddWidget<CIMGUILabel>("", 100.f, 30.f);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(115.f);

	m_SaveObjectButton = AddWidget<CIMGUIButton>("SaveObject", 100.f, 30.f);
	m_SaveObjectButton->SetClickCallback(this, &CEditorMenu::SaveObject);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(220.f);

	m_LoadObjectButton = AddWidget<CIMGUIButton>("LoadObject", 100.f, 30.f);
	m_LoadObjectButton->SetClickCallback(this, &CEditorMenu::LoadObject);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(325.f);

	m_ClearObjectButton = AddWidget<CIMGUIButton>("ClearObject", 100.f, 30.f);
	m_ClearObjectButton->SetClickCallback(this, &CEditorMenu::ClearObject);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(430.f);

	m_DeleteObjectButton = AddWidget<CIMGUIButton>("DeleteObject ", 100.f, 30.f);
	m_DeleteObjectButton->SetClickCallback(this, &CEditorMenu::DeleteObject);

	Label = AddWidget<CIMGUILabel>("", 100.f, 30.f);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.5f, 0.0f);

	// ================================================

	Label = AddWidget<CIMGUILabel>("Component", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(115.f);

	m_ComponentButton = AddWidget<CIMGUIButton>("CreateComp", 100.f, 30.f);
	m_ComponentButton->SetClickCallback(this, &CEditorMenu::CreateNewComponent);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(220.f);

	m_ComponentComboBox = AddWidget<CIMGUIComboBox>("SelectComp", 100.f, 30.f);
	m_ComponentComboBox->AddItem("Sprite");
	m_ComponentComboBox->AddItem("StaticMesh");
	m_ComponentComboBox->AddItem("Widget");
	m_ComponentComboBox->AddItem("ColliderBox2D");
	m_ComponentComboBox->AddItem("ColliderCircle");
	m_ComponentComboBox->AddItem("ColliderPixel");
	m_ComponentComboBox->AddItem("Camera");
	m_ComponentComboBox->AddItem("TileEmpty");
	m_ComponentComboBox->AddItem("BackGround");
	// m_ComponentComboBox->AddItem("TileMapComponent");
	m_ComponentComboBox->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(325.f);

	m_ComponentNameInput = AddWidget<CIMGUITextInput>("ComponentName", 100.f, 30.f);

	// ========================================================================
	/*
	m_ClearComponentButton = AddWidget<CIMGUIButton>("ClearComponent", 80.f, 30.f);
	m_ClearComponentButton->SetClickCallback(this, &CEditorMenu::ClearComponent);
	Line = AddWidget<CIMGUISameLine>("Line");
	*/

	// ========================================================================

	m_PlayButton = AddWidget<CIMGUIButton>("Play", 80.f, 30.f);
	m_PlayButton->SetClickCallback(this, &CEditorMenu::TogglePlay);


	return true;
}

void CEditorMenu::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CEditorMenu::SetEditModeText(const char* Text)
{
	m_CurrentEditMenu->SetText(Text);
}

void CEditorMenu::CreateNewObject()
{
	// Object Type 선택
	if (m_ObjectComboBox->GetSelectIndex() < 0)
		return;

	// Text Input
	if (m_ObjectNameInput->Empty())
		return;

	// 중복 방지
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();
	const std::string NewObjectName = m_ObjectNameInput->GetTextMultibyte();

	if (Hierarchy->GetCreatedObjectListBox()->CheckItem(NewObjectName))
		return;

	// CreateObject_Type enum 의 범위를 벗어나면 X
	if ((CreateObject_Type)m_ObjectComboBox->GetSelectIndex() >= CreateObject_Type::End)
		return;

	switch ((CreateObject_Type)m_ObjectComboBox->GetSelectIndex())
	{
	case CreateObject_Type::GameObject :
		{
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CGameObject>(m_ObjectNameInput->GetTextMultibyte());
		}
		break;
	case CreateObject_Type::Player :
		{
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CPlayer2D>(m_ObjectNameInput->GetTextMultibyte());
		}
		break;
	case CreateObject_Type::TileEmptyObject:
		{
			CSceneManager::GetInst()->GetScene()->CreateGameObject<CTileMapEmpty>(m_ObjectNameInput->GetTextMultibyte());
		}
		break;
	case CreateObject_Type::BackGround:
		{
			CSceneManager::GetInst()->GetScene()->CreateGameObject<CBackGround>(m_ObjectNameInput->GetTextMultibyte());
		}
		break;
	case CreateObject_Type::LineContainer:
		{
			// 이미 해당 LineContainer Type 이 만들어졌는지 검사해야 한다.
			if (CSceneManager::GetInst()->GetScene()->FindGameObjectByTypeID(typeid(CLineContainer).hash_code()))
				return;

			CSceneManager::GetInst()->GetScene()->CreateGameObject<CLineContainer>(m_ObjectNameInput->GetTextMultibyte());
		}
		break;
	}

	/*
	case CreateObject_Type::YellowBird:
		{
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CYellowBird>(m_ObjectNameInput->GetTextMultibyte());
		}
		break;
	case CreateObject_Type::PurpleBeatle:
		{
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CPurpleBeatle>(m_ObjectNameInput->GetTextMultibyte());
		}
		break;
	case CreateObject_Type::NormalBear:
		{
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CNormalBear>(m_ObjectNameInput->GetTextMultibyte());
		}
		break;
	*/

	// Add List To Object List
	Hierarchy->AddCreatedObject(m_ObjectNameInput->GetTextUTF8());

}

void CEditorMenu::CreateNewComponent()
{
	// Component Type 선택
	if (m_ComponentComboBox->GetSelectIndex() < 0)
		return;

	// Text Input
	if (m_ComponentNameInput->Empty())
		return;

	// Object 가 ObjectHierarcy 에서 선택되어 있어야 한다
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();
	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	// 중복 방지
	const std::string NewComponentName = m_ComponentNameInput->GetTextMultibyte();
	if (Hierarchy->GetCreatedComponentListBox()->CheckItem(NewComponentName))
		return;

	// 해당 Object 의 Component 목록에 추가
	const char* ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	switch ((CreateComponent_Type)m_ComponentComboBox->GetSelectIndex())
	{
	case CreateComponent_Type::Sprite :
		{
		Object->CreateComponent<CSpriteComponent>(m_ComponentNameInput->GetTextMultibyte());
		}
		break;
	case CreateComponent_Type::Static:
		{
		Object->CreateComponent<CStaticMeshComponent>(m_ComponentNameInput->GetTextMultibyte());
		}
		break;
	case CreateComponent_Type::Widget:
		{
			Object->CreateComponent<CWidgetComponent>(m_ComponentNameInput->GetTextMultibyte());
		}
		break;
	case CreateComponent_Type::ColliderBox2D:
		{
			Object->CreateComponent<CColliderBox2D>(m_ComponentNameInput->GetTextMultibyte());
		}
		break;
	case CreateComponent_Type::ColliderCircle:
		{
			Object->CreateComponent<CColliderCircle>(m_ComponentNameInput->GetTextMultibyte());
		}
		break;
	case CreateComponent_Type::ColliderPixel:
		{
			Object->CreateComponent<CColliderPixel>(m_ComponentNameInput->GetTextMultibyte());
		}
		break;
	case CreateComponent_Type::Camera:
		{
			Object->CreateComponent<CCameraComponent>(m_ComponentNameInput->GetTextMultibyte());
		}
		break;
	case CreateComponent_Type::TileEmpty:
		{
			/*
			CTileMapComponent* TileMapComponent = (CTileMapComponent*)Object->CreateComponent<CTileMapComponent>(m_ComponentNameInput->GetTextMultibyte());
			CMaterial* TileMapMaterial = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("TileMapMaterial");
			TileMapComponent->SetTileMaterial(TileMapMaterial);
			*/
			Object->CreateComponent<CTileEmptyComponent>(m_ComponentNameInput->GetTextMultibyte());
		}
	break;
	case CreateComponent_Type::BackGround :
		{
			Object->CreateComponent<CBackGroundComponent>(m_ComponentNameInput->GetTextMultibyte());
		}
		break;
	}

	// Root Component에 Add 시킨다 ?

	// Add Item To Component List Box
	Hierarchy->GetCreatedComponentListBox()->AddItem(m_ComponentNameInput->GetTextMultibyte());
}

void CEditorMenu::TogglePlay()
{
	if (CEngine::GetInst()->IsPlay())
		CEngine::GetInst()->SetPlay(false);
	else
		CEngine::GetInst()->SetPlay(true);

}

void CEditorMenu::SaveScene()
{
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->IsEmpty())
		return;

	TCHAR FileFullPath[MAX_PATH] = {};
	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.lpstrFile = FileFullPath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->Path;
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0*.Scene File\0*.scn");
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();

	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		CSceneManager::GetInst()->GetScene()->SaveFullPath(FilePathMultibyte);

		// GameEngine 폴더에도 저장해야 한다.
			// GameEngine 폴더에 저장하기
		std::string ExtraFolderName = ENGINE_SCENE_PATH;

		const PathInfo* EngineSequenceFolder = CPathManager::GetInst()->FindPath(ExtraFolderName);

		// 파일 이름을 뽑아낸다.
		char SavedFileName[MAX_PATH] = {};
		char SavedExt[_MAX_EXT] = {};
		_splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, SavedFileName, MAX_PATH, SavedExt, _MAX_EXT);

		// 최종 GameEngine 경로를 만든다.
		char SavedGameEnginePath[MAX_PATH] = {};
		strcpy_s(SavedGameEnginePath, EngineSequenceFolder->PathMultibyte);
		strcat_s(SavedGameEnginePath, SavedFileName);
		strcat_s(SavedGameEnginePath, SavedExt);

		// 현재 저장되는 경로와 다르다면, GameEngine 쪽에도 저장한다.
		if (strcmp(EngineSequenceFolder->PathMultibyte, FilePathMultibyte) != 0)
		{
			CSceneManager::GetInst()->GetScene()->SaveFullPath(SavedGameEnginePath);
		}
	}
}

void CEditorMenu::LoadScene()
{
	// 기존에 작업중인 사항이 있다면, 즉, ObjectHierarchy에 GameObject, Sprite 목록이 남아있다면
	// Message Box 띄우고 return;
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (Hierarchy->GetCreatedObjectListBox()->GetItemCount() > 0)
	{
		MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Complete Current Work"), TEXT("Data Might Be Lost"), 0);
		return;
	}

	TCHAR LoadFilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.lpstrFile = LoadFilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0*.Scene File\0*.scn");
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->Path;
	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		bool Result = CSceneManager::GetInst()->GetScene()->LoadFullPath(FilePathMultibyte);

		if (!Result)
			return;

		CSceneManager::GetInst()->GetScene()->SetIsEditMode(true);

		// 1) 만약 해당 Component 가 TileMapComponent 라면, Edit Mode 를 Tile 로 바꿔준다.
		/*
		std::list<CSharedPtr<CGameObject>> ObjLists = CSceneManager::GetInst()->GetScene()->GetObjectLists();

		auto iter = ObjLists.begin();
		auto iterEnd = ObjLists.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetRootComponent()->CheckType<CTileEmptyComponent>())
			{
				// 1) Edit mode 수정
				CEditorManager::GetInst()->SetEditMode(EditMode::Tile);
				// 2) TileMap Window 에 TileMapComponent 지정하기
				CEditorManager::GetInst()->GetTileMapWindow()->SetTileMap((CTileEmptyComponent*)(*iter)->GetRootComponent());
			}
		}
		*/
		CEditorManager::GetInst()->SetEditMode(EditMode::Scene);


		// Scene의 Object 목록을 돌면서, Object Hierarchy 에 Add 시키기 위해
		// Object 들의 이름을 가져온다
		std::vector<std::string> vecObjNames;
		CSceneManager::GetInst()->GetScene()->GatherObjectsNames(vecObjNames);

		CIMGUIListBox* ObjectListBox = Hierarchy->GetCreatedObjectListBox();

		size_t NameCount = vecObjNames.size();
		for (size_t i = 0; i < NameCount; i++)
		{
			ObjectListBox->AddItem(vecObjNames[i]);
		}

		// 화면에 Scene Edit Object를 만든다
		CEditorManager::GetInst()->SetSceneEditObject();

		// 가장 처음 녀석의 Component 들을 세팅해둔다.
		Hierarchy->SelectCreatedObject(0, vecObjNames[0].c_str());

		// 현재 선택된 Component의 위치로 세팅한다.
		CShowObject* ShowObject = CEditorManager::GetInst()->GetShowObject();

		std::string ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem();
		CGameObject* SelectedObject = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName.c_str());

		Vector3 ObjectPivot = SelectedObject->GetPivot();
		Vector3 ObjectSize = SelectedObject->GetWorldScale();
		Vector3 ObjectPos = SelectedObject->GetWorldPos();
		Vector3 Pos = ObjectPos - ObjectPivot * ObjectSize;

		Vector2 StartPos = Vector2(Pos.x, Pos.y);
		Vector2 EndPos = Vector2(Pos.x + ObjectSize.x, Pos.y + ObjectSize.y);

		ShowObject->SetStartPos(StartPos);
		ShowObject->SetEndPos(EndPos);

		// Line Object 목록은 LineEdit Window의 Object list 에 추가하기
		std::vector<std::string> vecLineObjNames;
		CSceneManager::GetInst()->GetScene()->GatherSpecificTypeObjectsName<CLine>(vecLineObjNames);

		CIMGUIListBox* LineListBox = CEditorManager::GetInst()->GetLineEditWindow()->GetCreatedLineListBox();

		size_t LineListSize = vecLineObjNames.size();

		for (size_t i = 0; i < LineListSize; i++)
		{
			LineListBox->AddItem(vecLineObjNames[i]);
		}
	}
}

void CEditorMenu::SaveObject()
{
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	TCHAR FileFullPath[MAX_PATH] = {};
	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.lpstrFile = FileFullPath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(OBJECT_PATH)->Path;
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0*.GameObject File\0*.gobj");
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();

	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		std::string ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem();

		CGameObject* TargetObject = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName.c_str());

		TargetObject->SaveFullPath(FilePathMultibyte);

		// GameEngine 폴더에 저장하기
		std::string ExtraFolderName = ENGINE_OBJECT_PATH;

		const PathInfo* EngineSequenceFolder = CPathManager::GetInst()->FindPath(ExtraFolderName);

		// 파일 이름을 뽑아낸다.
		char SavedFileName[MAX_PATH] = {};
		char SavedExt[_MAX_EXT] = {};
		_splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, SavedFileName, MAX_PATH, SavedExt, _MAX_EXT);

		// 최종 GameEngine 경로를 만든다.
		char SavedGameEnginePath[MAX_PATH] = {};
		strcpy_s(SavedGameEnginePath, EngineSequenceFolder->PathMultibyte);
		strcat_s(SavedGameEnginePath, SavedFileName);
		strcat_s(SavedGameEnginePath, SavedExt);

		// 현재 저장되는 경로와 다르다면, GameEngine 쪽에도 저장한다.
		if (strcmp(EngineSequenceFolder->PathMultibyte, FilePathMultibyte) != 0)
		{
			TargetObject->SaveFullPath(SavedGameEnginePath);
		}
	}
}

void CEditorMenu::LoadObject()
{
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();


	TCHAR LoadFilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.lpstrFile = LoadFilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0*.GameObject File\0*.gobj");
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(OBJECT_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		CGameObject* LoadedObject = CSceneManager::GetInst()->GetScene()->LoadGameObject<CGameObject>();

		LoadedObject->LoadFullPath(FilePathMultibyte);


		/*
		< 문제점  > 여기서만 Type 변환을 해주는 것이 아니라
		실제 Object 안에서 Type 변환을 해줘야만 한다 ...
		
		size_t GameObjectTypeID = LoadedObject->GetTypeID();
		
		if (GameObjectTypeID == typeid(CPlayer2D).hash_code())
		{
			LoadedObject = (CPlayer2D*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CYellowBird).hash_code())
		{
			LoadedObject = (CYellowBird*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CPurpleBeatle).hash_code())
		{
			LoadedObject = (CPurpleBeatle*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CNormalBear).hash_code())
		{
			LoadedObject = (CNormalBear*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CMushRoom).hash_code())
		{
			LoadedObject = (CMushRoom*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CHPYellowItem).hash_code())
		{
			LoadedObject = (CHPYellowItem*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CHPGreenItem).hash_code())
		{
			LoadedObject = (CHPGreenItem*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CHPRedItem).hash_code())
		{
			LoadedObject = (CHPRedItem*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CBlock).hash_code())
		{
			LoadedObject = (CBlock*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CBeamMonster).hash_code())
		{
			LoadedObject = (CBeamMonster*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CFireMonster).hash_code())
		{
			LoadedObject = (CFireMonster*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CFightMonster).hash_code())
		{
			LoadedObject = (CFightMonster*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CTileMapEmpty).hash_code())
		{
			LoadedObject = (CTileMapEmpty*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CBackGround).hash_code())
		{
			LoadedObject = (CBackGround*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CEffectSceneChangeStar).hash_code())
		{
			LoadedObject = (CEffectSceneChangeStar*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CEffectChangeToGreen2).hash_code())
		{
			LoadedObject = (CEffectChangeToGreen2*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CEffectChangeToGreen1).hash_code())
		{
			LoadedObject = (CEffectChangeToGreen1*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CEffectChangeToGreen4).hash_code())
		{
			LoadedObject = (CEffectChangeToGreen4*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CEffectChangeToGreen5).hash_code())
		{
			LoadedObject = (CEffectChangeToGreen5*)LoadedObject;
		}
		if (GameObjectTypeID == typeid(CLine).hash_code())
		{
			LoadedObject = (CLine*)LoadedObject;
		}
		else if (GameObjectTypeID == typeid(CLineContainer).hash_code())
		{
			LoadedObject = (CLineContainer*)LoadedObject;
		}
		*/
		

		// 중복 방지 
		if (Hierarchy->GetCreatedObjectListBox()->CheckItem(LoadedObject->GetName()))
		{
			CSceneManager::GetInst()->GetScene()->RemoveDuplicateObject(LoadedObject->GetName());
			return;
		}

		// 이미 TileMapComponent에 해당하는 녀석을 Load 해놓은 상태 + 지금 불러온 녀석이 TileMap 용 Object 라면
		int ObjectNumbers = Hierarchy->GetCreatedObjectListBox()->GetItemCount();

		for (int i = 0; i < ObjectNumbers; i++)
		{
			std::string ObjectName = Hierarchy->GetCreatedObjectListBox()->GetItem(i);

			CGameObject* CheckObject = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName.c_str());

			if (CheckObject->GetRootComponent() &&
				CheckObject->GetRootComponent()->CheckType<CTileMapComponent>())
			{
				SAFE_DELETE(LoadedObject);
				return;
			}
		}

		if (LoadedObject->GetRootComponent())
		{
			// 1) 만일 TileMapComponent를 Load 했다면
			if (LoadedObject->GetRootComponent()->CheckType<CTileEmptyComponent>())
			{
				// 1) Edit mode 수정
				CEditorManager::GetInst()->SetEditMode(EditMode::Tile);

				CTileEmptyComponent* TileComponent = (CTileEmptyComponent*)LoadedObject->GetRootComponent();

				// 2) TileMap Window 에 TileMapComponent 지정하기
				CEditorManager::GetInst()->GetTileMapWindow()->SetTileMap(TileComponent);

				// 3) TileMap 에서의 Edit Mode를 true로 세팅
				TileComponent->SetEditMode(true);

				// 4) World Resolution 세팅
				Vector3 TileSize = TileComponent->GetWorldScale();
				CSceneManager::GetInst()->GetScene()->SetWorldResolution(TileSize.x, TileSize.y);
			};

			// 2) 만일 BackGround Component 를 Load 했다면
			if (LoadedObject->GetRootComponent()->CheckType<CBackGroundComponent>())
			{
				// 1) Edit mode 수정
				CEditorManager::GetInst()->SetEditMode(EditMode::Back);

				// 2) BackGround Window 에 BackGroundComponent 지정하기
				CEditorManager::GetInst()->GetBackGroundWindow()->SetBackGround((CBackGroundComponent*)LoadedObject->GetRootComponent());
			};
			
		}

		// Object Name List에 추가한다
		Hierarchy->GetCreatedObjectListBox()->AddItem(LoadedObject->GetName());

		// 화면에 Scene Edit Object를 만든다
		CEditorManager::GetInst()->SetSceneEditObject();

		// 가장 처음 녀석의 Component 들을 세팅해둔다.
		Hierarchy->GetCreatedObjectListBox()->SetSelectIndex(Hierarchy->GetCreatedObjectListBox()->GetItemCount() - 1);

		// 현재 선택된 Component의 위치로 세팅한다.
		CShowObject* ShowObject = CEditorManager::GetInst()->GetShowObject();

		std::string ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem();
		CGameObject* SelectedObject = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName.c_str());

		Vector3 ObjectPivot = SelectedObject->GetPivot();
		Vector3 ObjectSize = SelectedObject->GetWorldScale();
		Vector3 ObjectPos = SelectedObject->GetWorldPos();
		Vector3 Pos = ObjectPos - ObjectPivot * ObjectSize;

		Vector2 StartPos = Vector2(Pos.x, Pos.y);
		Vector2 EndPos = Vector2(Pos.x + ObjectSize.x, Pos.y + ObjectSize.y);

		ShowObject->SetStartPos(StartPos);
		ShowObject->SetEndPos(EndPos);

		/*
		아래의 과정은 각 Scene 에서의 Change 과정에서 세팅한다. 
		
		 if (LoadedObject->CheckType<CLine>())
		{
			// Line 을 Load 한 경우, 현재 Scene 에 Line Container가 존재하는지 확인하고
		// 존재한다면, 해당 Line Container 의 자식으로 넣어주고
			CGameObject* FoundLineContainer = CSceneManager::GetInst()->GetScene()->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());
			
			if (FoundLineContainer)
			{
				FoundLineContainer->AddChildGameObject(LoadedObject);
			}
			else
			{
				// 없다면, 새롭게 Line Container Game Object 을 만들어준 다음
				// Line Container 자식으로 넣어준다.
				CLineContainer* LineContainer = CSceneManager::GetInst()->GetScene()->CreateGameObject<CLineContainer>("LineContainer");

				LineContainer->AddChildGameObject((CLine*)LoadedObject);

				Hierarchy->GetCreatedObjectListBox()->AddItem(LineContainer->GetName());
			}
		}
		 */

		
	}
}

void CEditorMenu::ClearComponent()
{
	// Object가 선택된 상황이어야 한다.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	// 만약 Player를 선택했다면 메세지 띄우고 막기 
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str());
	if (Object->GetTypeID() == typeid(CPlayer2D).hash_code())
	{
		MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Cannot Clear Player Components"), TEXT("Message"), MB_OK);
		return;
	}

	// 메세지 박스로 물어본다
	if (MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Clear Components ?"), TEXT("Question"), MB_YESNO) == IDYES)
	{
		// 실제 Object 내의 Scene Component 목록들을 비워준다.
		Object->ClearSceneComponents();

		// Hierarchy 에서도 비워준다.
		CEditorManager::GetInst()->GetObjectHierarchy()->GetCreatedComponentListBox()->Clear();
	}
}

/*
void CEditorMenu::DeleteComponent()
{
	// Object가 선택된 상황이어야 한다.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	// Component도 세팅된 상태여야 한다.
	if (Hierarchy->GetCreatedComponentListBox()->GetSelectIndex() < 0)
		return;

	// 메세지 박스로 물어본다
	if (MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Delete Component?"), TEXT("Question"), MB_YESNO) == IDYES)
	{
		// Root Component 일 경우 어떻게 처리할 수 있을까 ?
		// 우선 Root Component 면 지울 수 없게 세팅해준다.

		// 실제 Object 내의 Scene Component 목록들을 비워준다.
		CGameObject* SelectObject = CSceneManager::GetInst()->GetScene()->FindGameObject(Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str());
		SelectObject->DeleteComponent(Hierarchy->GetCreatedComponentListBox()->GetSelectItem());

		// Hierarchy 에서도 비워준다.
		CEditorManager::GetInst()->GetObjectHierarchy()->GetCreatedComponentListBox()->Clear();
	}
}
*/

void CEditorMenu::ClearObject()
{
	// 메세지 박스로 물어본다
	if (MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Delete All Objects ?"), TEXT("Question"), MB_YESNO) == IDYES)
	{
		// 실제 모든 GameObject 를 지운다. 
		CSceneManager::GetInst()->GetScene()->ClearGameObjects();

		// CObjectHierarchy 에서도 해당 정보들을 지운다.
		CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

		Hierarchy->GetCreatedObjectListBox()->Clear();
		Hierarchy->GetCreatedComponentListBox()->Clear();
	}
}

void CEditorMenu::DeleteObject()
{
	CIMGUIListBox* CreatedObjListBox = CEditorManager::GetInst()->GetObjectHierarchy()->GetCreatedObjectListBox();
	CIMGUIListBox* CreatedComponentListBox = CEditorManager::GetInst()->GetObjectHierarchy()->GetCreatedComponentListBox();

	// 선택된 Object가 존재해야 한다.
	if (CreatedObjListBox->GetSelectIndex() < 0)
		return;

	// Scene 의 ObjList 에서도 지워야 하고
	std::string SelectObjectName = CreatedObjListBox->GetSelectItem();
	int SelectIndex = CreatedObjListBox->GetSelectIndex();

	CSceneManager::GetInst()->GetScene()->DeleteGameObject(SelectObjectName);

	// 현재 List Box 내 이름 목록에서도 지워야 한다.
	CreatedObjListBox->DeleteItem(SelectIndex);
	CreatedComponentListBox->Clear();

	// 첫번째 목록을 선택 목록으로 둔다.
	if (CreatedObjListBox->IsEmpty() == false)
	{
		CreatedObjListBox->SetSelectIndex(0);
		
		CGameObject* NewSelectedObject = CSceneManager::GetInst()->GetScene()->FindGameObject(CreatedObjListBox->GetSelectItem().c_str());

		std::vector<FindComponentName> ComponentNameList = {};
		NewSelectedObject->GetAllSceneComponentsName(ComponentNameList);

		size_t Size = ComponentNameList.size();

		for (size_t i = 0; i < Size; i++)
		{
			CreatedComponentListBox->AddItem(ComponentNameList[i].Name);
		}
	}

	// Line Object일 경우, LineEditWindow 상에서 생성된 Line Object 목록에서도 지워줘야 한다.
	int DeleteLineIndex = CEditorManager::GetInst()->GetLineEditWindow()->GetCreatedLineListBox()->FindItemIndex(SelectObjectName);


	if (DeleteLineIndex != -1)
	{
		CEditorManager::GetInst()->GetLineEditWindow()->GetCreatedLineListBox()->DeleteItem(DeleteLineIndex);

		CEditorManager::GetInst()->GetLineEditWindow()->GetCreatedLineListBox()->SetSelectIndex(CEditorManager::GetInst()->GetLineEditWindow()->GetCreatedLineListBox()->GetItemCount() - 1);
	}
}

void CEditorMenu::SelectEditModeCallback(int Index, const char* EditModeText)
{
	m_CurrentEditMenu->SetText(EditModeText);

	CEditorManager::GetInst()->SetEditMode((EditMode)Index);
}
