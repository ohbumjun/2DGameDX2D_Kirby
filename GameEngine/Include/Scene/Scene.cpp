#include "Scene.h"
#include "SceneCollision.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ViewPort.h"
#include "NavigationManager.h"
#include "../PathManager.h"

class CPlayer2D;

CScene::CScene()  :
	m_Change(false)
{
	m_Mode     = new CSceneMode;
	m_Resource = new CSceneResource;
	m_Collision = new CSceneCollision;
	m_CameraManager = new CCameraManager;
	m_ViewPort = new CViewPort;
	m_NavManager = new CNavigationManager;

	m_Mode->m_Scene     = this;
	m_Resource->m_Scene = this;
	m_Collision->m_Scene = this;
	m_CameraManager->m_Scene = this;
	m_ViewPort->m_Scene = this;
	m_NavManager->m_Scene = this;
	
	m_Start = false;
	m_Collision->Init();
	m_CameraManager->Init();
	m_ViewPort->Init();
	m_NavManager->Init();

	Resolution RS = CEngine::GetInst()->GetResolution();

	m_WorldResolution = Vector2((float)RS.Width, (float)RS.Height);
}

CScene::~CScene()
{
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_ViewPort);
	SAFE_DELETE(m_Resource);
	SAFE_DELETE(m_Collision);
	SAFE_DELETE(m_NavManager);
}

void CScene::Start()
{
	m_Mode->Start();

	auto iter    = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	m_Collision->Start();
	m_CameraManager->Start();
	m_ViewPort->Start();
	m_NavManager->Start();
	m_Start = true;
	
	// 카메라 Component가 세팅되어 있다면
	// 카메라 매니저의 메인 카메라로 세팅해둔다
	if (m_Mode->GetPlayerObject())
	{
		CCameraComponent* Camera = m_Mode->GetPlayerObject()->FindComponentByType<CCameraComponent>();

		if (Camera)
		{
			m_CameraManager->SetCurrentCamera(Camera);

			Resolution RS = CEngine::GetInst()->GetResolution();

			float LeftEnd = m_Mode->GetPlayerObject()->GetWorldPos().x - 0.5f * (float)RS.Width;
			float BottomEnd = m_Mode->GetPlayerObject()->GetWorldPos().y - 0.5f * (float)RS.Height;

			if (BottomEnd + RS.Height > GetWorldResolution().y)
			{
				BottomEnd = GetWorldResolution().y - RS.Height;
			}

			Vector3 CameraPos = Vector3(
				LeftEnd,
				BottomEnd,
				0.f);

			// Float2_1 같은 경우에는 왼쪽 최상단에서 시작한다.
			// 이 경우, 제일 왼쪽 위 까지 세팅한다.

			Camera->SetWorldPos(CameraPos);


		}
	}
}

void CScene::Update(float DeltaTime)
{
	m_Mode->Update(DeltaTime);

	auto iter    = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter    = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		if ((*iter)->IsStop())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);
		++iter;
	}

	m_ViewPort->Update(DeltaTime);
	m_CameraManager->Update(DeltaTime);
	m_NavManager->Update(DeltaTime);
}

void CScene::PostUpdate(float DeltaTime)
{
	m_Mode->PostUpdate(DeltaTime);

	auto iter    = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter    = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}
		if ((*iter)->IsStop())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}

	// 충돌체들을 충돌 영역에 포함시킨다
	iter	   = m_ObjList.begin();
	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->AddCollision();
	}

	// 포함된 충돌체들을 이용해서 충돌처리를 진행한다.
	m_Collision->Collision(DeltaTime);

	m_ViewPort->PostUpdate(DeltaTime);

	m_CameraManager->PostUpdate(DeltaTime);
}

void CScene::SaveFullPath(const char* FullPath)
{
	/*
	CSceneResource*                    m_Resource; --> 왜 저장안해주는 것일까 ??
	std::list<CSharedPtr<CGameObject>> m_ObjList;
	bool                               m_Start;
	 */
	FILE* pFile = nullptr;
	fopen_s(&pFile, FullPath, "wb");
	if (!pFile)
		return;

	size_t TypeID = m_Mode->GetTypeID();
	fwrite(&TypeID, sizeof(size_t), 1, pFile);

	// SceneEdit Object가 있을 경우, 저장을 하면 안된다.
	// DeleteGameObject("SceneEditObject");

	fwrite(&m_WorldResolution, sizeof(Vector2), 1, pFile);

	size_t ObjectCount = m_ObjList.size();

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		// if ((*iter)->CheckType<CShowObject>())
		if ((*iter)->GetName() == OBJECTNAME_SHOWOBJECT)
		{
			ObjectCount -= 1;
			break;
		}
	}

	fwrite(&ObjectCount, sizeof(size_t), 1, pFile);

	iter = m_ObjList.begin();
	iterEnd = m_ObjList.end();

	for (;iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == "SceneEditObject")
			continue;

		size_t ObjectTypeID = (*iter)->GetTypeID();

		fwrite(&ObjectTypeID, sizeof(size_t), 1, pFile);

		(*iter)->Save(pFile);
	}

	fclose(pFile);
}

bool CScene::LoadFullPath(const char* FullPath)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, FullPath, "rb");

	if (!pFile)
		return false;

	m_ObjList.clear();

	size_t TypeID;
	fread(&TypeID, sizeof(size_t), 1, pFile);
	CSceneManager::GetInst()->CallCreateSceneModeFunc(this, TypeID);

	fread(&m_WorldResolution, sizeof(Vector2), 1, pFile);

	size_t ObjectCount;
	fread(&ObjectCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < ObjectCount; i++)
	{
		size_t ObjectTypeID = -1;

		fread(&ObjectTypeID, sizeof(size_t), 1, pFile);

		CGameObject* Object = CSceneManager::GetInst()->CallCreateObjectFunc(this, ObjectTypeID);

		if (!Object)
		{
			assert("No Object Created");
			MessageBox(CEngine::GetInst()->GetWindowHandle(), 
			TEXT("No Object Created"), TEXT("No Proper Function To Create Object"), 0);
			return false;
			// continue;
		}

		// Scene 세칭
		Object->SetScene(this);

		Object->Load(pFile);
	}

	// 다시 Object List를 돌면서, Object 자신의 Parent 세팅을 해준다.
	{
		auto iter = m_ObjList.begin();
		auto iterEnd = m_ObjList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->m_ParentName.length() > 0)
			{
				CGameObject* Parent = FindGameObject((*iter)->m_ParentName.c_str());

				// 부모가 존재하지 않는다면
				if (!Parent)
					continue;

				// 이미 자식으로 추가되어 있다면
				if (Parent->FindChildGameObject((*iter)))
					continue;

				(*iter)->m_Parent = Parent;
				Parent->AddChildGameObject((*iter));
			}
		}
	}

	{
		auto iter = m_ObjList.begin();
		auto iterEnd = m_ObjList.end();

		for (; iter != iterEnd;++iter)
		{
			// 만약 Scene Change 과정에서 이미 Player가 세팅되어 있다면
			// Player2D Type은 Load 하지 않는다. --> ClientManager 코드에서 살펴보기 
			// if (CSceneManager::GetStaticPlayerInfo() && TypeID == m_PlayerTypeID)
			//	continue;
			if (CSceneManager::GetStaticPlayerInfo() && (*iter)->GetTypeID() == CSceneManager::GetStaticPlayerInfo()->GetTypeID())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				break;
			}
		}
	}
	
	if (m_Start)
	{
		// 모든 Object 를 돌면서, Start 함수를 실행해준다.
		// ex) 충돌체 목록들을 SceneCollision List 에 추가하기 
		auto iter = m_ObjList.begin();
		auto iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			(*iter)->Start();
			++iter;
		}
	}


	fclose(pFile);

	return true;
}

bool CScene::Load(const char* FileName, const std::string& PathName)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	char FullPath[MAX_PATH] = {};

	if (Path)
		strcpy_s(FullPath, Path->PathMultibyte);

	strcat_s(FullPath, FileName);

	return LoadFullPath(FullPath);
}

void CScene::ClearGameObjects()
{
	m_ObjList.clear();
}

void CScene::DeleteGameObject(const std::string& Name)
{
	auto iter      = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
		{
			(*iter)->Destroy();

			// m_ObjList.erase(iter);

			break;
		}
	}
}

void CScene::DeletePlayerFromScene()
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	// 하나를 찾아내고
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTypeID() == CSceneManager::GetStaticPlayerInfo()->GetTypeID())
		{
			iter = m_ObjList.erase(iter);
			return;
		}
	}
}

void CScene::RemoveDuplicateObject(const std::string& Name)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();
	auto iterFound = iter;

	// 하나를 찾아내고
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
		{
			iterFound = ++iter;
			break;
		}
	}

	// 이후 부터를 검사해서 중복 제거
	for (; iterFound != iterEnd;)
	{
		if ((*iterFound)->GetName() == Name)
		{
			iterFound = m_ObjList.erase(iterFound);
			iterEnd = m_ObjList.end();
			continue;
		}
		++iterFound;
	}
}

void CScene::SetStopEnableObjectsExceptPlayer(size_t TypeID, bool Enable)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTypeID() == TypeID)
			continue;

		(*iter)->SetStop(Enable);
	}
}

CGameObject* CScene::FindGameObject(const char* ObjectName) const
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == ObjectName)
			return (*iter);
	}
	return nullptr;
}

CGameObject* CScene::FindGameObjectByTypeID(size_t TypeID) const
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_TypeID == TypeID)
			return (*iter);
	}
	return nullptr;
}


void CScene::GatherObjectsNames(std::vector<std::string>& vecObjNames)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		vecObjNames.push_back((*iter)->GetName());
	}
}

void CScene::SetAutoChange(bool Change)
{
	m_Change = Change;
}
