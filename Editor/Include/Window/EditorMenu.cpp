#include "EditorMenu.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "IMGUISameLine.h"
#include "IMGUIButton.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "../EditorManager.h"
#include "ObjectHierarchy.h"
#include "IMGUIListBox.h"
#include "../Object/SpriteEditObject.h"
#include "../Object/Player2D.h"
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
#include "TileMapWindow.h"
#include "../Object/ShowObject.h"


CEditorMenu::CEditorMenu()
{
}

CEditorMenu::~CEditorMenu()
{
}

bool CEditorMenu::Init()
{
	CIMGUIWindow::Init();

	m_ObjectComboBox = AddWidget<CIMGUIComboBox>("ObjectComboBox", 150.f, 30.f);
	m_ObjectComboBox->AddItem("GameObject");
	m_ObjectComboBox->AddItem("Player");
	m_ObjectComboBox->AddItem("YellowBird");
	m_ObjectComboBox->AddItem("BeamMonster");
	m_ObjectComboBox->SetHideName(true);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectNameInput = AddWidget<CIMGUITextInput>("ObjectName", 80.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectButton = AddWidget<CIMGUIButton>("CreateObject", 120.f, 30.f);
	m_ObjectButton->SetClickCallback(this, &CEditorMenu::CreateNewObject);

	// ========================================================================

	m_ComponentComboBox = AddWidget<CIMGUIComboBox>("ComponentComboBox", 150.f, 30.f);
	m_ComponentComboBox->AddItem("SpriteComponent");
	m_ComponentComboBox->AddItem("StaticMeshComponent");
	m_ComponentComboBox->AddItem("WidgetComponent");
	m_ComponentComboBox->AddItem("ColliderBox2D");
	m_ComponentComboBox->AddItem("ColliderCircle");
	m_ComponentComboBox->AddItem("ColliderPixel");
	m_ComponentComboBox->AddItem("CameraComponent");
	m_ComponentComboBox->AddItem("TileEmptyComponent");
	m_ComponentComboBox->AddItem("BackGroundComponent");
	// m_ComponentComboBox->AddItem("TileMapComponent");
	m_ComponentComboBox->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentNameInput = AddWidget<CIMGUITextInput>("ComponentName", 80.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentButton = AddWidget<CIMGUIButton>("CreateComponent", 120.f, 30.f);
	m_ComponentButton->SetClickCallback(this, &CEditorMenu::CreateNewComponent);

	// ========================================================================

	m_SaveSceneButton = AddWidget<CIMGUIButton>("SaveScene", 80.f, 30.f);
	m_SaveSceneButton->SetClickCallback(this, &CEditorMenu::SaveScene);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_LoadSceneButton = AddWidget<CIMGUIButton>("LoadScene", 80.f, 30.f);
	m_LoadSceneButton->SetClickCallback(this, &CEditorMenu::LoadScene);

	// ========================================================================

	m_SaveObjectButton = AddWidget<CIMGUIButton>("SaveObject", 80.f, 30.f);
	m_SaveObjectButton->SetClickCallback(this, &CEditorMenu::SaveObject);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_LoadObjectButton = AddWidget<CIMGUIButton>("LoadObject", 80.f, 30.f);
	m_LoadObjectButton->SetClickCallback(this, &CEditorMenu::LoadObject);

	// ========================================================================

	m_ClearComponentButton = AddWidget<CIMGUIButton>("ClearComponent", 80.f, 30.f);
	m_ClearComponentButton->SetClickCallback(this, &CEditorMenu::ClearComponent);
	Line = AddWidget<CIMGUISameLine>("Line");


	m_ClearObjectButton = AddWidget<CIMGUIButton>("ClearObject", 80.f, 30.f);
	m_ClearObjectButton->SetClickCallback(this, &CEditorMenu::ClearObject);
	Line = AddWidget<CIMGUISameLine>("Line");

	m_DeleteObjectButton = AddWidget<CIMGUIButton>("DeleteObject ", 80.f, 30.f);
	m_DeleteObjectButton->SetClickCallback(this, &CEditorMenu::ClearComponent);

	/*
	m_DeleteComponentButton = AddWidget<CIMGUIButton>("DeleteComponent ", 80.f, 30.f);
	m_DeleteComponentButton->SetClickCallback(this, &CEditorMenu::DeleteComponent);
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

	if (Hierarchy->GetObjectListBox()->CheckItem(NewObjectName))
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
	}

	// Add List To Object List
	Hierarchy->AddObject(m_ObjectNameInput->GetTextUTF8());

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
	if (!Hierarchy || Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
		return;

	// 중복 방지
	const std::string NewComponentName = m_ComponentNameInput->GetTextMultibyte();
	if (Hierarchy->GetComponentListBox()->CheckItem(NewComponentName))
		return;

	// 해당 Object 의 Component 목록에 추가
	const char* ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem().c_str();
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
		/*
		CTileMapComponent* TileMapComponent = (CTileMapComponent*)Object->CreateComponent<CTileMapComponent>(m_ComponentNameInput->GetTextMultibyte());
		CMaterial* TileMapMaterial = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("TileMapMaterial");
		TileMapComponent->SetTileMaterial(TileMapMaterial);
		*/
		Object->CreateComponent<CBackGroundComponent>(m_ComponentNameInput->GetTextMultibyte());
	}
	}

	// Root Component에 Add 시킨다 ?

	// Add Item To Component List Box
	Hierarchy->GetComponentListBox()->AddItem(m_ComponentNameInput->GetTextMultibyte());
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
	if (!Hierarchy || Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
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
	}
}

void CEditorMenu::LoadScene()
{
	// 기존에 작업중인 사항이 있다면, 즉, ObjectHierarchy에 GameObject, Sprite 목록이 남아있다면
	// Message Box 띄우고 return;
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();
	if (Hierarchy->GetObjectListBox()->GetItemCount() > 0)
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

		// 1) 만약 해당 Component 가 TileMapComponent 라면, Edit Mode 를 Tile 로 바꿔준다.
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
		/*
		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetRootComponent()->CheckType<CTileMapComponent>())
			{
				// 1) Edit mode 수정
				CEditorManager::GetInst()->SetEditMode(EditMode::Tile);
				// 2) TileMap Window 에 TileMapComponent 지정하기
				CEditorManager::GetInst()->GetTileMapWindow()->SetTileMap((CTileMapComponent*)(*iter)->GetRootComponent());
			}
		}
		*/


		// Scene의 Object 목록을 돌면서, Object Hierarchy 에 Add 시키기 위해
		// Object 들의 이름을 가져온다
		std::vector<std::string> vecObjNames;
		CSceneManager::GetInst()->GetScene()->GatherObjectsNames(vecObjNames);

		CIMGUIListBox* ObjectListBox = Hierarchy->GetObjectListBox();

		size_t NameCount = vecObjNames.size();
		for (size_t i = 0; i < NameCount; i++)
		{
			// Scene Edit Object의 경우는 더해주지 않는다
			// if (vecObjNames[i] == "SceneEditObject")
			// 	continue;

			ObjectListBox->AddItem(vecObjNames[i]);
		}

		// 화면에 Scene Edit Object를 만든다
		CEditorManager::GetInst()->SetSceneEditObject();

		// 가장 처음 녀석의 Component 들을 세팅해둔다.
		Hierarchy->SelectObject(0, vecObjNames[0].c_str());

		// 현재 선택된 Component의 위치로 세팅한다.
		CShowObject* ShowObject = CEditorManager::GetInst()->GetShowObject();

		std::string ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem();
		CGameObject* SelectedObject = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName.c_str());

		Vector3 ObjectPivot = SelectedObject->GetPivot();
		Vector3 ObjectSize = SelectedObject->GetWorldScale();
		Vector3 ObjectPos = SelectedObject->GetWorldPos();
		Vector3 Pos = ObjectPos - ObjectPivot * ObjectSize;

		Vector2 StartPos = Vector2(Pos.x, Pos.y);
		Vector2 EndPos = Vector2(Pos.x + ObjectSize.x, Pos.y + ObjectSize.y);

		ShowObject->SetStartPos(StartPos);
		ShowObject->SetEndPos(EndPos);
	}
}

void CEditorMenu::SaveObject()
{
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
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

		std::string ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem();

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


		// 중복 방지 
		if (Hierarchy->GetObjectListBox()->CheckItem(LoadedObject->GetName()))
		{
			CSceneManager::GetInst()->GetScene()->RemoveDuplicateObject(LoadedObject->GetName());
			return;
		}

		// 이미 TileMapComponent에 해당하는 녀석을 Load 해놓은 상태 + 지금 불러온 녀석이 TileMap 용 Object 라면
		int ObjectNumbers = Hierarchy->GetObjectListBox()->GetItemCount();

		for (int i = 0; i < ObjectNumbers; i++)
		{
			std::string ObjectName = Hierarchy->GetObjectListBox()->GetItem(i);

			CGameObject* CheckObject = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName.c_str());

			if (CheckObject->GetRootComponent()->CheckType<CTileMapComponent>())
			{
				SAFE_DELETE(LoadedObject);
				return;
			}
		}

		// 만일 TileMapComponent를 Load 했다면
		if (LoadedObject->GetRootComponent())
		{
			if (LoadedObject->GetRootComponent()->CheckType<CTileEmptyComponent>())
			{
				// 1) Edit mode 수정
				CEditorManager::GetInst()->SetEditMode(EditMode::Tile);

				CTileEmptyComponent* TileComponent = (CTileEmptyComponent*)LoadedObject->GetRootComponent();

				// 2) TileMap Window 에 TileMapComponent 지정하기
				CEditorManager::GetInst()->GetTileMapWindow()->SetTileMap(TileComponent);

				// 3) TileMap 에서의 Edit Mode를 true로 세팅
				TileComponent->SetEditMode(true);

			};
			/*
			if (LoadedObject->GetRootComponent()->CheckType<CTileMapComponent>())
			{
				// 1) Edit mode 수정
				CEditorManager::GetInst()->SetEditMode(EditMode::Tile);

				// 2) TileMap Window 에 TileMapComponent 지정하기
				CEditorManager::GetInst()->GetTileMapWindow()->SetTileMap((CTileMapComponent*)LoadedObject->GetRootComponent());

			};
			*/
		}

		// Object Name List에 추가한다
		Hierarchy->GetObjectListBox()->AddItem(LoadedObject->GetName());

		// 화면에 Scene Edit Object를 만든다
		CEditorManager::GetInst()->SetSceneEditObject();

		// 가장 처음 녀석의 Component 들을 세팅해둔다.
		Hierarchy->GetObjectListBox()->SetSelectIndex(Hierarchy->GetObjectListBox()->GetItemCount() - 1);

		// 현재 선택된 Component의 위치로 세팅한다.
		CShowObject* ShowObject = CEditorManager::GetInst()->GetShowObject();

		std::string ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem();
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

		// 1) 만약 해당 Component 가 TileMapComponent 라면, Edit Mode 를 Tile 로 바꿔준다.
		std::list<CSharedPtr<CGameObject>> ObjLists = CSceneManager::GetInst()->GetScene()->GetObjectLists();

		auto iter = ObjLists.begin();
		auto iterEnd = ObjLists.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetRootComponent()->CheckType<CTileMapComponent>())
			{
				// 1) Edit mode 수정
				CEditorManager::GetInst()->SetEditMode(EditMode::Tile);
				// 2) TileMap Window 에 TileMapComponent 지정하기
				CEditorManager::GetInst()->GetTileMapWindow()->SetTileMap((CTileMapComponent*)(*iter)->GetRootComponent());
			}
		}


		// Scene의 Object 목록을 돌면서, Object Hierarchy 에 Add 시키기 위해
		// Object 들의 이름을 가져온다
		std::vector<std::string> vecObjNames;
		CSceneManager::GetInst()->GetScene()->GatherObjectsNames(vecObjNames);

		CIMGUIListBox* ObjectListBox = Hierarchy->GetObjectListBox();

		size_t NameCount = vecObjNames.size();
		for (size_t i = 0; i < NameCount; i++)
		{
			// Scene Edit Object의 경우는 더해주지 않는다
			// if (vecObjNames[i] == "SceneEditObject")
			// 	continue;

			ObjectListBox->AddItem(vecObjNames[i]);
		}

		// 화면에 Scene Edit Object를 만든다
		CEditorManager::GetInst()->SetSceneEditObject();

		// 가장 처음 녀석의 Component 들을 세팅해둔다.
		Hierarchy->SelectObject(0, vecObjNames[0].c_str());

		// 현재 선택된 Component의 위치로 세팅한다.
		CShowObject* ShowObject = CEditorManager::GetInst()->GetShowObject();

		std::string ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem();
		CGameObject* SelectedObject = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName.c_str());

		Vector3 ObjectPivot = SelectedObject->GetPivot();
		Vector3 ObjectSize = SelectedObject->GetWorldScale();
		Vector3 ObjectPos = SelectedObject->GetWorldPos();
		Vector3 Pos = ObjectPos - ObjectPivot * ObjectSize;

		Vector2 StartPos = Vector2(Pos.x, Pos.y);
		Vector2 EndPos = Vector2(Pos.x + ObjectSize.x, Pos.y + ObjectSize.y);

		ShowObject->SetStartPos(StartPos);
		ShowObject->SetEndPos(EndPos);
	*/
	}
}

void CEditorMenu::ClearComponent()
{
	// Object가 선택된 상황이어야 한다.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
		return;

	// 만약 Player를 선택했다면 메세지 띄우고 막기 
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(Hierarchy->GetObjectListBox()->GetSelectItem().c_str());
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
		CEditorManager::GetInst()->GetObjectHierarchy()->GetComponentListBox()->Clear();
	}
}

void CEditorMenu::DeleteComponent()
{
	// Object가 선택된 상황이어야 한다.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
		return;

	// Component도 세팅된 상태여야 한다.
	if (Hierarchy->GetComponentListBox()->GetSelectIndex() < 0)
		return;

	// 메세지 박스로 물어본다
	if (MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Delete Component?"), TEXT("Question"), MB_YESNO) == IDYES)
	{
		// Root Component 일 경우 어떻게 처리할 수 있을까 ?
		// 우선 Root Component 면 지울 수 없게 세팅해준다.

		// 실제 Object 내의 Scene Component 목록들을 비워준다.
		CGameObject* SelectObject = CSceneManager::GetInst()->GetScene()->FindGameObject(Hierarchy->GetObjectListBox()->GetSelectItem().c_str());
		SelectObject->DeleteComponent(Hierarchy->GetComponentListBox()->GetSelectItem());

		// Hierarchy 에서도 비워준다.
		CEditorManager::GetInst()->GetObjectHierarchy()->GetComponentListBox()->Clear();
	}
}

void CEditorMenu::ClearObject()
{
	// 메세지 박스로 물어본다
	if (MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Delete All Objects ?"), TEXT("Question"), MB_YESNO) == IDYES)
	{
		// 실제 모든 GameObject 를 지운다. 
		CSceneManager::GetInst()->GetScene()->ClearGameObjects();

		// CObjectHierarchy 에서도 해당 정보들을 지운다.
		CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

		Hierarchy->GetObjectListBox()->Clear();
		Hierarchy->GetComponentListBox()->Clear();
	}
}

void CEditorMenu::DeleteObject()
{
	// 선택된 Object가 존재해야 한다.
}
