#include "ColliderCircle.h"

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

	m_CBuffer->UpdateCBuffer();

	m_Shader->SetShader();

	m_Mesh->Render();

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
	fwrite(&m_Info, sizeof(CircleInfo), 1, pFile);
	CColliderComponent::Save(pFile);
}

void CColliderCircle::Load(FILE* pFile)
{
	fread(&m_Info, sizeof(CircleInfo), 1, pFile);
	CColliderComponent::Load(pFile);
}

bool CColliderCircle::Collision(CColliderComponent* Dest)
{}

bool CColliderCircle::CollisionMouse(const Vector2& MousePos)
{}
