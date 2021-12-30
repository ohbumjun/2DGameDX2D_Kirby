#include "ColliderBox2D.h"
#include "../Collision/Collision.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "../Scene/Scene.h";
#include "../Scene/SceneResource.h"

CColliderBox2D::CColliderBox2D()
{
	SetTypeID<CColliderBox2D>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_ColliderType = Collider_Type::Box_2D;
}

CColliderBox2D::CColliderBox2D(const CColliderBox2D& Collider) : CColliderComponent(Collider)
{
	m_Info = Collider.m_Info;
}

CColliderBox2D::~CColliderBox2D()
{
}

void CColliderBox2D::Start()
{
	CColliderComponent::Start();
}

bool CColliderBox2D::Init()
{
	if (!CColliderComponent::Init())
		return false;

	m_Info.Axis[0] = Vector2(1.f, 0.f);
	m_Info.Axis[1] = Vector2(0.f, 1.f);
	m_Info.Length = Vector2(50.f, 50.f);

	// 부모의 Z 회전에 영향을 받게 세팅한다. 
	SetInheritRotZ(true);
	SetWorldScale(m_Info.Length.x * 2.f, m_Info.Length.y * 2.f, 1.f);

	m_Mesh = m_Scene->GetResource()->FindMesh("Box2D");

	return true;
}

void CColliderBox2D::Update(float DeltaTime)
{
	CColliderComponent::Update(DeltaTime);
}

void CColliderBox2D::PostUpdate(float DeltaTime)
{
	CColliderComponent::PostUpdate(DeltaTime);

	// Center는 WorldPos 로 부터의 상대적 위치로 잡을 것이다. 이를 위해 Offset 정보를 세팅해준 것이다. 
	m_Info.Center.x = GetWorldPos().x + m_Offset.x;
	m_Info.Center.y = GetWorldPos().y + m_Offset.y;

	m_Info.Axis[0].x = GetWorldAxis(AXIS_X).x;
	m_Info.Axis[0].y = GetWorldAxis(AXIS_X).y;

	m_Info.Axis[1].x = GetWorldAxis(AXIS_Y).x;
	m_Info.Axis[1].y = GetWorldAxis(AXIS_Y).y;

	Vector2 Pos[4] = {};

	// 4개의 꼭짓점을 다 세팅하고 나서
	// 그 중에서 min을 구할 것이다.
	// 왼쪽 위 
	Pos[0] = m_Info.Center - m_Info.Axis[0] * m_Info.Length.x + m_Info.Axis[1] * m_Info.Length.y;
	// 오른쪽 위 
	Pos[1] = m_Info.Center + m_Info.Axis[0] * m_Info.Length.x + m_Info.Axis[1] * m_Info.Length.y;
	// 왼쪽 아래 
	Pos[2] = m_Info.Center - m_Info.Axis[0] * m_Info.Length.x - m_Info.Axis[1] * m_Info.Length.y;
	// 오른쪽 아래
	Pos[3] = m_Info.Center + m_Info.Axis[0] * m_Info.Length.x - m_Info.Axis[1] * m_Info.Length.y;

	m_Min.x = Pos[0].x;
	m_Min.y = Pos[0].y;

	m_Max.x = Pos[0].x;
	m_Max.y = Pos[0].y;

	for (int i = 1; i < 4; i++)
	{
		if (Pos[i].x < m_Min.x)
			m_Min.x = Pos[i].x;
		if (Pos[i].x > m_Max.x)
			m_Max.x = Pos[i].x;

		if (Pos[i].y < m_Min.y)
			m_Min.y = Pos[i].y;
		if (Pos[i].y > m_Max.y)
			m_Max.y = Pos[i].y;
	}

	m_Info.Min.x = m_Min.x;
	m_Info.Min.y = m_Min.y;

	m_Info.Max.x = m_Max.x;
	m_Info.Max.y = m_Max.y;

}

void CColliderBox2D::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderBox2D::Render()
{
	CColliderComponent::Render();

	Matrix matWorld, matProj, matWVP;

	// XMMatrixOrthographicOffCenterLH : 왼쪽 좌표 시스템에 대한 한 사용자 지정 직교 투영 행렬을 만든다.
	// ViewLeft   : Minimum x-value of view volume
	// ViewRight : Maximum x-value of view volume
	// ViewBottom : Maximum y-value of view volume
	// ViewTop      : Maximum y-value of view volume
	// NearZ : Distance to the near Clipping Plane
	// Farz    : Distance to the far Clipping plane
	// 리턴 값 : custom orthogonal projection matrix = 사용자 지정 직교 투영 행렬 변환 ( World 공간에서 카메라 공간으로의 변환) 
	matProj = XMMatrixOrthographicOffCenterLH(0.f, 1280.f, 0.f, 720.f, 0.f, 1000.f);

	Matrix matScale, matRot, matTrans;

	matScale.Scaling(m_Info.Length.x * 2.f, m_Info.Length.y * 2.f, 1.f);
	matRot.Rotation(GetWorldRot());
	matTrans.Translation(m_Info.Center);

	matWorld = matScale * matRot * matTrans;

	matWVP = matWorld * matProj;

	// Matrix를 CBuffer에 넘겨줄 때 Transpos해서 넘겨줘야 한다.
	matWVP.Transpose();

	m_CBuffer->SetWVP(matWVP);

	// 현재 충돌중인 목록이 없다면, 초록색으로 그린다
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

void CColliderBox2D::PostRender()
{
	CColliderComponent::PostRender();

	
}

CColliderComponent* CColliderBox2D::Clone()
{
	return new CColliderBox2D(*this);
}

void CColliderBox2D::CheckCollision()
{
	CColliderComponent::CheckCollision();
}

void CColliderBox2D::Save(FILE* pFile)
{
	CColliderComponent::Save(pFile);
	fwrite(&m_Info, sizeof(Box2DInfo), 1, pFile);
}

void CColliderBox2D::Load(FILE* pFile)
{
	CColliderComponent::Load(pFile);
	fread(&m_Info, sizeof(Box2DInfo), 1, pFile);
}

bool CColliderBox2D::Collision(CColliderComponent* Dest)
{
	switch (Dest->GetColliderType())
	{
	case Collider_Type::Box_2D :
		return CCollision::CollisionBox2DToBox2D(this, (CColliderBox2D*)Dest);
	}
	return true;
}

bool CColliderBox2D::CollisionMouse(const Vector2& MousePos)
{
	CollisionResult Result;
	m_MouseCollision = CCollision::CollisionBox2DToPoint(m_MouseResult, Result, m_Info, MousePos);
	return m_MouseCollision;
}
