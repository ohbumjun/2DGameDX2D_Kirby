#include "ColliderCircle.h"

#include "ColliderBox2D.h"
#include "ColliderPixel.h"
#include "../Collision/Collision.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "../Scene/SceneResource.h"

CColliderCircle::CColliderCircle()
{
	SetTypeID<CColliderCircle>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_ColliderType = Collider_Type::Circle;
}

CColliderCircle::CColliderCircle(const CColliderCircle& Circle) :
CColliderComponent(Circle)
{
	m_Info = Circle.m_Info;
}

CColliderCircle::~CColliderCircle()
{}

void CColliderCircle::Start()
{
	CColliderComponent::Start();
}

bool CColliderCircle::Init()
{
	if (!CColliderComponent::Init())
		return false;

	m_Info.Radius = 50.f;

	SetInheritRotZ(true);

	SetWorldScale(m_Info.Radius * 2.f, m_Info.Radius * 2.f, 1.f);

	m_Mesh = m_Scene->GetResource()->FindMesh("Circle");

	return true;
}

void CColliderCircle::Update(float DeltaTime)
{
	CColliderComponent::Update(DeltaTime);
}

void CColliderCircle::PostUpdate(float DeltaTime)
{
	CColliderComponent::PostUpdate(DeltaTime);

	/*
	if (!m_Scene->IsEditMode())
	{
		float ParentWorldScaleX = m_Transform->GetTransformParent()->GetWorldScale().x;
		float RelativeScale = m_Transform->GetRelativeScale().x;

		if (ParentWorldScaleX != 0.f && RelativeScale != 0.f)
		{
			m_Info.Radius = ParentWorldScaleX * RelativeScale * 0.8f;
		}
	}
	*/

	m_Info.Center.x = GetWorldPos().x + m_Offset.x;
	m_Info.Center.y = GetWorldPos().y + m_Offset.y;

	Vector2 Pos[4];

	m_Min.x = m_Info.Center.x - m_Info.Radius;
	m_Min.y = m_Info.Center.y - m_Info.Radius;

	m_Max.x = m_Info.Center.x + m_Info.Radius;
	m_Max.y = m_Info.Center.y + m_Info.Radius;

	m_Info.Min.x = m_Min.x;
	m_Info.Min.y = m_Min.y;

	m_Info.Max.x = m_Max.x;
	m_Info.Max.y = m_Max.y;
}

void CColliderCircle::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderCircle::Render()	   
{
	CColliderComponent::Render();

	Matrix matWorld, matView, matProj, matWVP;

	CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();
	matView = Camera->GetViewMatrix();
	matProj = Camera->GetProjMatrix();
	
	Matrix matScale, matRotate, matTrans;

	matScale.Scaling(Vector3(m_Info.Radius * 2.f, m_Info.Radius * 2.f, 1.f));
	// matRotate.Rotation(GetWorldRot()); --> 원은 특성상 회전시켜줄 필요가 없다. 
	matTrans.Translation(m_Info.Center);

	matWorld = matScale * matRotate * matTrans;

	matWVP = matWorld * matView * matProj;

	matWVP.Transpose();

	m_CBuffer->SetWVP(matWVP);

	if (m_PrevCollisionList.empty())
		m_CBuffer->SetColliderColor(Vector4(0.f, 1.f, 0.f, 1.f));
	else
		m_CBuffer->SetColliderColor(Vector4(1.f, 0.f, 0.f, 1.f));

	if (m_MouseCollision)
		m_CBuffer->SetColliderColor(Vector4(1.f, 0.f, 0.f, 1.f));

	if (m_Enable)
	{
		m_CBuffer->UpdateCBuffer();

		m_Shader->SetShader();

		m_Mesh->Render();
	}
}

void CColliderCircle::PostRender()
{
	CColliderComponent::PostRender();
}

CColliderCircle* CColliderCircle::Clone()
{
	return new CColliderCircle(*this);
}

void CColliderCircle::Save(FILE* pFile)
{
	CColliderComponent::Save(pFile);

	fwrite(&m_Info, sizeof(CircleInfo), 1, pFile);

	int MeshNameLength = (int)m_Mesh->GetName().length();

	fwrite(&MeshNameLength, sizeof(int), 1, pFile);

	fwrite(m_Mesh->GetName().c_str(), sizeof(char), MeshNameLength, pFile);
	
}

void CColliderCircle::Load(FILE* pFile)
{
	CColliderComponent::Load(pFile);

	fread(&m_Info, sizeof(CircleInfo), 1, pFile);

	
	SetInheritRotZ(true); // m_Transform->m_InheritRotz를 true로 세팅하는 것

	SetWorldScale(m_Info.Radius * 2.f, m_Info.Radius * 2.f, 1.f);

	char MeshName[MAX_PATH] = {};

	int MeshNameLength = -1;

	fread(&MeshNameLength, sizeof(int), 1, pFile);

	fread(MeshName, sizeof(char), MeshNameLength, pFile);

	m_Mesh = m_Scene->GetResource()->FindMesh(MeshName);
}

bool CColliderCircle::Collision(CColliderComponent* Dest)
{
	switch (Dest->GetColliderType())
	{
	case Collider_Type::Box_2D :
		return CCollision::CollisionBox2DToCircle((CColliderBox2D*)Dest, this);
	case Collider_Type::Circle :
		return CCollision::CollisionCircleToCircle(this, (CColliderCircle*)Dest);
	case Collider_Type::Pixel:
		return CCollision::CollisionCircleToPixel(this, (CColliderPixel*)Dest);
	}
	return false;
}

bool CColliderCircle::CollisionMouse(const Vector2& MousePos)
{
	CollisionResult DestResult;
	m_MouseCollision = CCollision::CollisionCircleToPoint(m_Result, DestResult, m_Info, MousePos);
	return m_MouseCollision;
}
