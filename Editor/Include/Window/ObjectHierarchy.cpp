#include "ObjectHierarchy.h"
#include "BackGroundWindow.h"
#include "IMGUIManager.h"
#include "TileMapWindow.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "GameObject/GameObject.h"
#include "../Object/ShowObject.h"
#include "../Object/LineContainer.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../EditorManager.h"
#include "DetailInfoWindow.h"
#include "LineEditWindow.h"
#include "Component/TileMapComponent.h"
#include "Component/TileEmptyComponent.h"
#include "Component/BackGroundComponent.h"

CObjectHierarchy::CObjectHierarchy()
{
}

CObjectHierarchy::~CObjectHierarchy()
{
}

bool CObjectHierarchy::Init()
{
	CIMGUIWindow::Init();

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("CreatedObjectList", 200.f, 30.f);
	Label->SetColor(0, 0, 255);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("CreatedComponentList", 200.f, 30.f);
	Label->SetColor(0, 0, 255);

	// ========================================================================

	m_CreatedObjectListBox = AddWidget<CIMGUIListBox>("CreatedObjectList", 200.f, 100.f);
	m_CreatedObjectListBox->SetHideName(true);
	m_CreatedObjectListBox->SetPageItemCount(10);
	m_CreatedObjectListBox->SetSelectCallback(this, &CObjectHierarchy::SelectCreatedObject);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_CreatedComponentListBox = AddWidget<CIMGUIListBox>("CreatedComponentList", 200.f, 100.f);
	m_CreatedComponentListBox->SetHideName(true);
	m_CreatedComponentListBox->SetPageItemCount(10);
	m_CreatedComponentListBox->SetSelectCallback(this, &CObjectHierarchy::SelectCreatedComponent);


	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);

	// ========================================================================

	Label = AddWidget<CIMGUILabel>("ObjectList", 200.f, 30.f);
	Label->SetColor(0, 0, 255);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Specific Object List", 200.f, 30.f);
	Label->SetColor(0, 0, 255);

	// ========================================================================

	m_ObjectList = AddWidget<CIMGUIListBox>("ObjectList", 200.f, 100.f);
	m_ObjectList->SetHideName(true);
	m_ObjectList->SetPageItemCount(12);
	m_ObjectList->SetSelectCallback(this, &CObjectHierarchy::SeeObjectList);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_SpecificObjectList = AddWidget<CIMGUIListBox>("Specific Object List", 200.f, 100.f);
	m_SpecificObjectList->SetHideName(true);
	m_SpecificObjectList->SetPageItemCount(12);
	m_SpecificObjectList->SetSelectCallback(this, &CObjectHierarchy::SeeSpecificObjectList);

	// ========================================================================

	// m_mapVecObject ????
	std::vector<std::string>  vecNormalMonsters = { g_NormalBearName,
		g_PurpleBeatlesName, g_YellowBirdName, g_MushRoomName, g_Squid, g_Kabu, g_Fish, g_Frog,
		g_Chicken, g_Pig, g_Scary};

	std::vector<std::string>  vecAbilityMonsters = { g_BeamMonsterName, g_FireMonsterName,
	g_FightMonsterName, g_BombMonsterName, g_SwordMonsterName };

	std::vector<std::string>  vecEffectObjects = { g_SceneChangeStarToGreen3, g_SceneChangeToGreen2,
		g_SceneChangeToGreen1, g_SceneChangeToGreen4 , g_SceneChangeToGreen5,
		g_SceneChangeToFloat2_1, g_SceneChangeToFloat2_2, g_SceneChangeToFloat3, g_SceneChangeToFloat4,
		g_SceneChangeToFloat6, g_SceneChangeToFloat7, 
		g_WaterFloatIsland1, g_WaterFloatIsland2_2, g_SceneChangeToDyna2 };

	std::vector<std::string>  vecItemObjects = { g_HPYellowItem, g_HPGreenItem, g_HPRedItem,
		g_Block, g_Ladder, g_Awl };

	std::vector<std::string>  vecMiddleBossObjects = { g_MiddleHammerName, g_MiddleScissorName };

	std::vector<std::string>  vecBossObjects = { g_BossTree , g_BossPenguin, g_BossDyna};

	m_mapVecObject.insert(std::make_pair("NormalMonster", vecNormalMonsters));
	m_mapVecObject.insert(std::make_pair("AbilityMonster", vecAbilityMonsters));
	m_mapVecObject.insert(std::make_pair("EffectObject", vecEffectObjects));
	m_mapVecObject.insert(std::make_pair("ItemObject", vecItemObjects));
	m_mapVecObject.insert(std::make_pair("MiddleBoss", vecMiddleBossObjects));
	m_mapVecObject.insert(std::make_pair("Boss", vecBossObjects));

	// m_mapVecObject ?? Key ???? m_ObjectList ?? ????
	for (const auto & kv : m_mapVecObject)
	{
		m_ObjectList->AddItem(kv.first);
	}

	// Normal Monster ?? m_ObjectList ???? ?????? ?????? ????
	m_ObjectList->SetSelectIndex(0);

	// m_SpecificObjectList ?? Normal Monster ?? ???? ???? ????
	for (size_t i = 0; i < vecBossObjects.size(); i++)
	{
		m_SpecificObjectList->AddItem(vecBossObjects[i]);
	}

	return true;
}

void CObjectHierarchy::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CObjectHierarchy::SetSeletedObjectByName(const std::string& Name)
{
	int Index = m_CreatedObjectListBox->FindItemIndex(Name);

	if (Index == -1)
		return;

	m_CreatedObjectListBox->SetSelectIndex(Index);
}

void CObjectHierarchy::AddCreatedObject(const char* ObjectName)
{
	m_CreatedObjectListBox->AddItem(ObjectName);
}

void CObjectHierarchy::SelectCreatedObject(int Index, const char* ObjectName)
{
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;

	// ?????? Object ???? ???? Scene Component ?????? ????????
	m_SelectObject = Object;

	// ???? ???? ????????
	m_CreatedComponentListBox->Clear();

	// ???? ???? ?????? ???? Root Component ?? ?????? 
	std::vector<FindComponentName> vecComponentsName;
	m_SelectObject->GetAllSceneComponentsName(vecComponentsName);

	// ???? ?????? ?????? ?????? ????.
	m_CreatedObjectListBox->SetSelectIndex(Index);

	for (size_t i = 0; i < vecComponentsName.size(); i++)
	{
		m_CreatedComponentListBox->AddItem(vecComponentsName[i].Name);
	}

	// ???? ?????? Component?? ?????? ?????? ????????.
	m_CreatedComponentListBox->SetSelectIndex(0);

	// TileMap ???? BackGround ?? ???????? 
	// Object ?? Pos, Rot, Scale x,y,z ?????? DetailWindow ???? ??????????.
	if (m_SelectObject->GetRootComponent() &&
		!m_SelectObject->GetRootComponent()->CheckType<CTileEmptyComponent>() &&
		!m_SelectObject->GetRootComponent()->CheckType<CBackGroundComponent>())
	{
		CDetailInfoWindow* DetailWindow = CEditorManager::GetInst()->GetDetailWindow();
		DetailWindow->SetClickedObjectInfo(m_SelectObject->GetRootComponent());

		CEditorManager::GetInst()->SetSceneEditObject();

		// ?????? ShowObject ?????? Object?? Root Component ?????? ????
		CShowObject* ShowObject = CEditorManager::GetInst()->GetShowObject();

		CEditorManager::GetInst()->SetSceneEditObjectPos(ShowObject);

		/*
		Vector3 ObjectPivot = m_SelectObject->GetPivot();
		Vector3 ObjectSize = m_SelectObject->GetWorldScale();
		Vector3 ObjectPos = m_SelectObject->GetWorldPos();
		Vector3 Pos = ObjectPos - ObjectPivot * ObjectSize;

		Vector2 StartPos = Vector2(Pos.x, Pos.y);
		Vector2 EndPos = Vector2(Pos.x + ObjectSize.x, Pos.y + ObjectSize.y);

		ShowObject->SetStartPos(StartPos);
		ShowObject->SetEndPos(EndPos);
		*/
	}
	else if (m_SelectObject->CheckType<CLineContainer>())
	{
		EditMode CurrentEditMode = CEditorManager::GetInst()->GetEditMode();

		if (CurrentEditMode != EditMode::LineCreate && CurrentEditMode != EditMode::LineEdit)
		{
			CEditorManager::GetInst()->SetEditMode(EditMode::LineEdit);

			CEditorManager::GetInst()->GetLineEditWindow()->SetLineContainer(m_SelectObject);

			if (m_SelectObject->GetChildCount() > 0)
			{
				CEditorManager::GetInst()->SetSceneEditObjectPos(m_SelectObject->GetChildObject(0));
			}
		}
		else
		{
			// ???? ???????? Line Container?? ???? ?????? ?????? ???? ??????
			// Show Object?? ????????.
			// ?????? ???? ?????? ?????? Line Edit Window?? ????????.
			CEditorManager::GetInst()->GetLineEditWindow()->SetLineContainer(m_SelectObject);

			if (m_SelectObject->GetChildCount() > 0)
			{
				CEditorManager::GetInst()->SetSceneEditObjectPos(m_SelectObject->GetChildObject(0));
			}
		}
	}
}

void CObjectHierarchy::SeeObjectList(int Index, const char* ObjectName)
{
	m_SpecificObjectList->Clear();

	std::vector<std::string> vecObjectLists = m_mapVecObject[ObjectName];

	for (size_t i = 0; i < vecObjectLists.size(); i++)
	{
		m_SpecificObjectList->AddItem(vecObjectLists[i]);
	}
}

void CObjectHierarchy::SeeSpecificObjectList(int Index, const char* ComponentName)
{
	// ???????? ?????? 
}

bool CObjectHierarchy::CheckDuplicateObject(const std::string& ObjName)
{
	return m_CreatedObjectListBox->CheckItem(ObjName);
}

void CObjectHierarchy::SelectCreatedComponent(int Index, const char* ComponentName)
{
	if (!m_SelectObject)
		return;

	m_SelectComponent = (CSceneComponent*)m_SelectObject->FindComponent(ComponentName);

	if (!m_SelectComponent)
		return;

	CTileMapWindow* TileMapWindow = (CTileMapWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("TileMapWindow");

	CBackGroundWindow* BackGroundWindow = (CBackGroundWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("BackGroundWindow");

	// Tile Map Component ???? 
	if (TileMapWindow && m_SelectComponent->CheckType<CTileEmptyComponent>())
	{
		// TileMapWindow->SetTileMap((CTileEmptyComponent*)m_SelectComponent);
		TileMapWindow->SetTileMap((CTileEmptyComponent*)m_SelectComponent);

		// Edit Mode?? Tile Edit ?????? ????????
		CEditorManager::GetInst()->SetEditMode(EditMode::Tile);

		// 4) World Resolution ????
		Vector3 TileSize = m_SelectComponent->GetWorldScale();
		CSceneManager::GetInst()->GetScene()->SetWorldResolution(TileSize.x, TileSize.y);
	}
	// BackGround Component ????
	else if (BackGroundWindow && m_SelectComponent->CheckType<CBackGroundComponent>())
	{
		BackGroundWindow->SetBackGround((CBackGroundComponent*)m_SelectComponent);

		// Edit Mode?? Tile Edit ?????? ????????
		CEditorManager::GetInst()->SetEditMode(EditMode::Back);
	}
	/*
	else
	{
		TileMapWindow->SetTileMap(nullptr);
	}
	*/
}
