#include "DragObject.h"
#include "Component/StaticMeshComponent.h"
#include "Render/RenderManager.h"
#include "Resource/Material/Material.h"
#include "../Window/SpriteEditWindow.h"
#include "../EditorManager.h"
#include "SpriteEditObject.h"

CDragObject::CDragObject()
{
}

CDragObject::CDragObject(const CDragObject& Obj) :
	CGameObject(Obj)
{
	m_MeshComponent = dynamic_cast<CStaticMeshComponent*>(FindComponent("Mesh"));
}

CDragObject::~CDragObject()
{
}

void CDragObject::SetStartPos(const Vector2& Pos)
{
	m_StartPos = Pos;
	m_MeshComponent->SetWorldPos(m_StartPos.x, m_StartPos.y, 1.f);

	CSpriteEditObject* SpriteEditObject = CEditorManager::GetInst()->GetSpriteWindow()->GetSpriteEditObject();
	if (SpriteEditObject)
	{
		CTexture* Texture = SpriteEditObject->GetSpriteComponent()->GetTexture();
		Vector2 StartPos = m_StartPos;
		StartPos.y = (float)Texture->GetHeight() - StartPos.y;

		bool ReverseMode = CEditorManager::GetInst()->GetSpriteWindow()->IsReverseMode();

		if (ReverseMode)
		{
			CEditorManager::GetInst()->GetSpriteWindow()->SetSpriteCurrentFrameImageStart(StartPos.x * -1.f, StartPos.y);
			CEditorManager::GetInst()->GetSpriteWindow()->SetFrameStartText(StartPos.x * -1.f, StartPos.y);
		}
		else
		{
			CEditorManager::GetInst()->GetSpriteWindow()->SetSpriteCurrentFrameImageStart(StartPos.x, StartPos.y);
			CEditorManager::GetInst()->GetSpriteWindow()->SetFrameStartText(StartPos.x, StartPos.y);
		}

	}
}

void CDragObject::SetEndPos(const Vector2& Pos)
{
	m_EndPos = Pos;
	m_MeshComponent->SetWorldScale(m_EndPos.x - m_StartPos.x, m_EndPos.y - m_StartPos.y, 1.f);

	CSpriteEditObject* SpriteEditObject = CEditorManager::GetInst()->GetSpriteWindow()->GetSpriteEditObject();
	if (SpriteEditObject)
	{
		CTexture* Texture = SpriteEditObject->GetSpriteComponent()->GetTexture();
		Vector2 EndPos = m_EndPos;

		EndPos.y = (float)Texture->GetHeight() - EndPos.y;

		bool ReverseMode = CEditorManager::GetInst()->GetSpriteWindow()->IsReverseMode();

		if (ReverseMode)
		{
			CEditorManager::GetInst()->GetSpriteWindow()->SetSpriteCurrentFrameImageEnd(EndPos.x * -1.f, EndPos.y);
			CEditorManager::GetInst()->GetSpriteWindow()->SetFrameEndText(EndPos.x * -1.f, EndPos.y);
		}
		else
		{
			CEditorManager::GetInst()->GetSpriteWindow()->SetSpriteCurrentFrameImageEnd(EndPos.x, EndPos.y);
			CEditorManager::GetInst()->GetSpriteWindow()->SetFrameEndText(EndPos.x, EndPos.y);
		}
	}
}

bool CDragObject::Init()
{
	if (!CGameObject::Init())
		return false;

	m_MeshComponent = CreateComponent<CStaticMeshComponent>("Mesh");
	SetRootComponent(m_MeshComponent);

	// Drag Layer에 세팅하기 
	m_MeshComponent->SetLayerName("DragLayer");
	CRenderManager::GetInst()->AddRenderList(m_MeshComponent);

	m_MeshComponent->SetMesh("FrameRect");
	m_MeshComponent->GetMaterial()->SetShader("PosMeshShader");
	// m_MeshComponent->SetPivot(0.001f, 0.001f, 0.f);
	m_MeshComponent->SetBaseColor(1.f, 0.f, 0.f, 1.f);

	return true;
}

void CDragObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CDragObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CDragObject* CDragObject::Clone()
{
	return new CDragObject(*this);
}
