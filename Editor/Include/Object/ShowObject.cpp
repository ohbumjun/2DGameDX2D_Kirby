#include "ShowObject.h"
#include <Component/StaticMeshComponent.h>
#include "Resource/Material/Material.h"
#include "Render/RenderManager.h"

CShowObject::CShowObject() :
	m_MeshComponent(nullptr)
{
	SetTypeID<CShowObject>();
}

CShowObject::CShowObject(const CShowObject& obj) : CGameObject(obj)
{
	m_MeshComponent = (CStaticMeshComponent*)FindComponent("ShowMeshComponent");
}

CShowObject::~CShowObject()
{}


void CShowObject::SetStartPos(const ::Vector2& Pos)
{
	m_StartPos = Pos;
	m_MeshComponent->SetWorldPos(m_StartPos.x, m_StartPos.y, 1.f);
}

void CShowObject::SetEndPos(const ::Vector2& Pos)
{
	m_EndPos = Pos;
	m_MeshComponent->SetWorldScale(m_EndPos.x - m_StartPos.x, m_EndPos.y - m_StartPos.y, 1.f);
	m_MeshComponent->SetMeshSize(m_EndPos.x - m_StartPos.x, m_EndPos.y - m_StartPos.y, 0.f);
}

bool CShowObject::Init()
{
	if (!CGameObject::Init())
		return false;
	m_MeshComponent = CreateComponent<CStaticMeshComponent>("ShowMeshComponent");
	SetRootComponent(m_MeshComponent);

	// EditorManger 에서 만들어진 상태로 시작 
	m_MeshComponent->SetLayerName("SceneEditLayer");
	CRenderManager::GetInst()->AddRenderList(m_MeshComponent);

	m_MeshComponent->SetMesh("FrameRect");
	m_MeshComponent->GetMaterial()->SetShader("PosMeshShader");

	// Pivot 대로 제대로 세팅되지 않음
	// m_MeshComponent->SetPivot(0.01f, 0.01f, 0.f);
	m_MeshComponent->SetBaseColor(1.f, 1.f, 0.f, 1.f);

	return true;
}


CShowObject* CShowObject::Clone()
{
	return new CShowObject(*this);
}
