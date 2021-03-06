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
	// Object Type ????
	if (m_ObjectComboBox->GetSelectIndex() < 0)
		return;

	// Text Input
	if (m_ObjectNameInput->Empty())
		return;

	// ???? ????
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();
	const std::string NewObjectName = m_ObjectNameInput->GetTextMultibyte();

	if (Hierarchy->GetCreatedObjectListBox()->CheckItem(NewObjectName))
		return;

	// CreateObject_Type enum ?? ?????? ???????? X
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
			// ???? ???? LineContainer Type ?? ???????????? ???????? ????.
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
	// Component Type ????
	if (m_ComponentComboBox->GetSelectIndex() < 0)
		return;

	// Text Input
	if (m_ComponentNameInput->Empty())
		return;

	// Object ?? ObjectHierarcy ???? ???????? ?????? ????
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();
	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	// ???? ????
	const std::string NewComponentName = m_ComponentNameInput->GetTextMultibyte();
	if (Hierarchy->GetCreatedComponentListBox()->CheckItem(NewComponentName))
		return;

	// ???? Object ?? Component ?????? ????
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

	// Root Component?? Add ?????? ?

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
	OpenFile.lpstrFilter = TEXT("????????\0*.*\0*.Scene File\0*.scn");
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();

	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		CSceneManager::GetInst()->GetScene()->SaveFullPath(FilePathMultibyte);

		// GameEngine ???????? ???????? ????.
			// GameEngine ?????? ????????
		std::string ExtraFolderName = ENGINE_SCENE_PATH;

		const PathInfo* EngineSequenceFolder = CPathManager::GetInst()->FindPath(ExtraFolderName);

		// ???? ?????? ????????.
		char SavedFileName[MAX_PATH] = {};
		char SavedExt[_MAX_EXT] = {};
		_splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, SavedFileName, MAX_PATH, SavedExt, _MAX_EXT);

		// ???? GameEngine ?????? ??????.
		char SavedGameEnginePath[MAX_PATH] = {};
		strcpy_s(SavedGameEnginePath, EngineSequenceFolder->PathMultibyte);
		strcat_s(SavedGameEnginePath, SavedFileName);
		strcat_s(SavedGameEnginePath, SavedExt);

		// ???? ???????? ?????? ????????, GameEngine ?????? ????????.
		if (strcmp(EngineSequenceFolder->PathMultibyte, FilePathMultibyte) != 0)
		{
			CSceneManager::GetInst()->GetScene()->SaveFullPath(SavedGameEnginePath);
		}
	}
}

void CEditorMenu::LoadScene()
{
	// ?????? ???????? ?????? ??????, ??, ObjectHierarchy?? GameObject, Sprite ?????? ??????????
	// Message Box ?????? return;
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
	OpenFile.lpstrFilter = TEXT("????????\0*.*\0*.Scene File\0*.scn");
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

		// 1) ???? ???? Component ?? TileMapComponent ????, Edit Mode ?? Tile ?? ????????.
		/*
		std::list<CSharedPtr<CGameObject>> ObjLists = CSceneManager::GetInst()->GetScene()->GetObjectLists();

		auto iter = ObjLists.begin();
		auto iterEnd = ObjLists.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetRootComponent()->CheckType<CTileEmptyComponent>())
			{
				// 1) Edit mode ????
				CEditorManager::GetInst()->SetEditMode(EditMode::Tile);
				// 2) TileMap Window ?? TileMapComponent ????????
				CEditorManager::GetInst()->GetTileMapWindow()->SetTileMap((CTileEmptyComponent*)(*iter)->GetRootComponent());
			}
		}
		*/
		CEditorManager::GetInst()->SetEditMode(EditMode::Scene);


		// Scene?? Object ?????? ??????, Object Hierarchy ?? Add ?????? ????
		// Object ???? ?????? ????????
		std::vector<std::string> vecObjNames;
		CSceneManager::GetInst()->GetScene()->GatherObjectsNames(vecObjNames);

		CIMGUIListBox* ObjectListBox = Hierarchy->GetCreatedObjectListBox();

		size_t NameCount = vecObjNames.size();
		for (size_t i = 0; i < NameCount; i++)
		{
			ObjectListBox->AddItem(vecObjNames[i]);
		}

		// ?????? Scene Edit Object?? ??????
		CEditorManager::GetInst()->SetSceneEditObject();

		// ???? ???? ?????? Component ???? ??????????.
		Hierarchy->SelectCreatedObject(0, vecObjNames[0].c_str());

		// ???? ?????? Component?? ?????? ????????.
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

		// Line Object ?????? LineEdit Window?? Object list ?? ????????
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
	OpenFile.lpstrFilter = TEXT("????????\0*.*\0*.GameObject File\0*.gobj");
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();

	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		std::string ObjectName = Hierarchy->GetCreatedObjectListBox()->GetSelectItem();

		CGameObject* TargetObject = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName.c_str());

		TargetObject->SaveFullPath(FilePathMultibyte);

		// GameEngine ?????? ????????
		std::string ExtraFolderName = ENGINE_OBJECT_PATH;

		const PathInfo* EngineSequenceFolder = CPathManager::GetInst()->FindPath(ExtraFolderName);

		// ???? ?????? ????????.
		char SavedFileName[MAX_PATH] = {};
		char SavedExt[_MAX_EXT] = {};
		_splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, SavedFileName, MAX_PATH, SavedExt, _MAX_EXT);

		// ???? GameEngine ?????? ??????.
		char SavedGameEnginePath[MAX_PATH] = {};
		strcpy_s(SavedGameEnginePath, EngineSequenceFolder->PathMultibyte);
		strcat_s(SavedGameEnginePath, SavedFileName);
		strcat_s(SavedGameEnginePath, SavedExt);

		// ???? ???????? ?????? ????????, GameEngine ?????? ????????.
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
	OpenFile.lpstrFilter = TEXT("????????\0*.*\0*.GameObject File\0*.gobj");
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(OBJECT_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		CGameObject* LoadedObject = CSceneManager::GetInst()->GetScene()->LoadGameObject<CGameObject>();

		LoadedObject->LoadFullPath(FilePathMultibyte);


		/*
		< ??????  > ???????? Type ?????? ?????? ???? ??????
		???? Object ?????? Type ?????? ???????? ???? ...
		
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
		

		// ???? ???? 
		if (Hierarchy->GetCreatedObjectListBox()->CheckItem(LoadedObject->GetName()))
		{
			CSceneManager::GetInst()->GetScene()->RemoveDuplicateObject(LoadedObject->GetName());
			return;
		}

		// ???? TileMapComponent?? ???????? ?????? Load ?????? ???? + ???? ?????? ?????? TileMap ?? Object ????
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
			// 1) ???? TileMapComponent?? Load ??????
			if (LoadedObject->GetRootComponent()->CheckType<CTileEmptyComponent>())
			{
				// 1) Edit mode ????
				CEditorManager::GetInst()->SetEditMode(EditMode::Tile);

				CTileEmptyComponent* TileComponent = (CTileEmptyComponent*)LoadedObject->GetRootComponent();

				// 2) TileMap Window ?? TileMapComponent ????????
				CEditorManager::GetInst()->GetTileMapWindow()->SetTileMap(TileComponent);

				// 3) TileMap ?????? Edit Mode?? true?? ????
				TileComponent->SetEditMode(true);

				// 4) World Resolution ????
				Vector3 TileSize = TileComponent->GetWorldScale();
				CSceneManager::GetInst()->GetScene()->SetWorldResolution(TileSize.x, TileSize.y);
			};

			// 2) ???? BackGround Component ?? Load ??????
			if (LoadedObject->GetRootComponent()->CheckType<CBackGroundComponent>())
			{
				// 1) Edit mode ????
				CEditorManager::GetInst()->SetEditMode(EditMode::Back);

				// 2) BackGround Window ?? BackGroundComponent ????????
				CEditorManager::GetInst()->GetBackGroundWindow()->SetBackGround((CBackGroundComponent*)LoadedObject->GetRootComponent());
			};
			
		}

		// Object Name List?? ????????
		Hierarchy->GetCreatedObjectListBox()->AddItem(LoadedObject->GetName());

		// ?????? Scene Edit Object?? ??????
		CEditorManager::GetInst()->SetSceneEditObject();

		// ???? ???? ?????? Component ???? ??????????.
		Hierarchy->GetCreatedObjectListBox()->SetSelectIndex(Hierarchy->GetCreatedObjectListBox()->GetItemCount() - 1);

		// ???? ?????? Component?? ?????? ????????.
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
		?????? ?????? ?? Scene ?????? Change ???????? ????????. 
		
		 if (LoadedObject->CheckType<CLine>())
		{
			// Line ?? Load ?? ????, ???? Scene ?? Line Container?? ?????????? ????????
		// ??????????, ???? Line Container ?? ???????? ????????
			CGameObject* FoundLineContainer = CSceneManager::GetInst()->GetScene()->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());
			
			if (FoundLineContainer)
			{
				FoundLineContainer->AddChildGameObject(LoadedObject);
			}
			else
			{
				// ??????, ?????? Line Container Game Object ?? ???????? ????
				// Line Container ???????? ????????.
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
	// Object?? ?????? ?????????? ????.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	// ???? Player?? ?????????? ?????? ?????? ???? 
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str());
	if (Object->GetTypeID() == typeid(CPlayer2D).hash_code())
	{
		MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Cannot Clear Player Components"), TEXT("Message"), MB_OK);
		return;
	}

	// ?????? ?????? ????????
	if (MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Clear Components ?"), TEXT("Question"), MB_YESNO) == IDYES)
	{
		// ???? Object ???? Scene Component ???????? ????????.
		Object->ClearSceneComponents();

		// Hierarchy ?????? ????????.
		CEditorManager::GetInst()->GetObjectHierarchy()->GetCreatedComponentListBox()->Clear();
	}
}

/*
void CEditorMenu::DeleteComponent()
{
	// Object?? ?????? ?????????? ????.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || Hierarchy->GetCreatedObjectListBox()->GetSelectIndex() < 0)
		return;

	// Component?? ?????? ???????? ????.
	if (Hierarchy->GetCreatedComponentListBox()->GetSelectIndex() < 0)
		return;

	// ?????? ?????? ????????
	if (MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Delete Component?"), TEXT("Question"), MB_YESNO) == IDYES)
	{
		// Root Component ?? ???? ?????? ?????? ?? ?????? ?
		// ???? Root Component ?? ???? ?? ???? ??????????.

		// ???? Object ???? Scene Component ???????? ????????.
		CGameObject* SelectObject = CSceneManager::GetInst()->GetScene()->FindGameObject(Hierarchy->GetCreatedObjectListBox()->GetSelectItem().c_str());
		SelectObject->DeleteComponent(Hierarchy->GetCreatedComponentListBox()->GetSelectItem());

		// Hierarchy ?????? ????????.
		CEditorManager::GetInst()->GetObjectHierarchy()->GetCreatedComponentListBox()->Clear();
	}
}
*/

void CEditorMenu::ClearObject()
{
	// ?????? ?????? ????????
	if (MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Delete All Objects ?"), TEXT("Question"), MB_YESNO) == IDYES)
	{
		// ???? ???? GameObject ?? ??????. 
		CSceneManager::GetInst()->GetScene()->ClearGameObjects();

		// CObjectHierarchy ?????? ???? ???????? ??????.
		CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

		Hierarchy->GetCreatedObjectListBox()->Clear();
		Hierarchy->GetCreatedComponentListBox()->Clear();
	}
}

void CEditorMenu::DeleteObject()
{
	CIMGUIListBox* CreatedObjListBox = CEditorManager::GetInst()->GetObjectHierarchy()->GetCreatedObjectListBox();
	CIMGUIListBox* CreatedComponentListBox = CEditorManager::GetInst()->GetObjectHierarchy()->GetCreatedComponentListBox();

	// ?????? Object?? ???????? ????.
	if (CreatedObjListBox->GetSelectIndex() < 0)
		return;

	// Scene ?? ObjList ?????? ?????? ????
	std::string SelectObjectName = CreatedObjListBox->GetSelectItem();
	int SelectIndex = CreatedObjListBox->GetSelectIndex();

	CSceneManager::GetInst()->GetScene()->DeleteGameObject(SelectObjectName);

	// ???? List Box ?? ???? ?????????? ?????? ????.
	CreatedObjListBox->DeleteItem(SelectIndex);
	CreatedComponentListBox->Clear();

	// ?????? ?????? ???? ???????? ????.
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

	// Line Object?? ????, LineEditWindow ?????? ?????? Line Object ?????????? ???????? ????.
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
